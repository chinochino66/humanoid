/*
 * ServoCenter.ino — サーボ中央出し・可動域確認治具
 *
 * 系統タグ: S（安全・制御。L系実機の組立に使用するが、コード自体はInMoov非由来）
 * 作成日  : 2026-08-06
 *
 * ============================================================
 *  重要：本ファイルは「再作成版」であり、L-Phase 1 の成果物ではない
 * ============================================================
 *  手順書が L-Phase 1 の成果物として挙げる ServoCenter.ino は紛失した。
 *  本ファイルはリポジトリの現行設計（PCA9685 ×2 構成）に合わせて
 *  2026-08-06 に新規に書き起こしたものであり、実機で未検証である。
 *
 *  - L1 当時のコードとは構成が異なる（当時はArduino直結の可能性が高い）
 *  - 本ファイルをもって L-Phase 1 の完了記録としない
 *  - 実機で動作確認できた時点で、その日付と結果をここへ追記すること
 *
 *  参照: docs/OPEN_ISSUES.md #38（L0/L1成果物のリポジトリ未収録）
 *
 * ============================================================
 *  ハードウェア構成
 * ============================================================
 *  MCU        : Arduino Mega 2560（第2層）
 *  PCA9685 #1 : I2C 0x40 — レールA 5.8V（目・瞼／R系予約／tasteSensorLift）
 *  PCA9685 #2 : I2C 0x41 — レールB 5.0V（首・顎／throatGate）
 *  OE         : D4（両基板共通配線。HIGH = 全ch出力停止）
 *  I2C        : D20=SDA / D21=SCL
 *
 *  参照: docs/S/mcu_pinmap.md, docs/S/i2c_address_map.md,
 *        docs/L/channel_map_L.md, docs/S/power_tree.md §8
 *
 * ============================================================
 *  安全規則（コードで強制しているもの）
 * ============================================================
 *  1. 起動時は必ず OE=HIGH（出力停止）から始める      … power_tree §8
 *  2. 中立値を全chへ書き込んでから OE=LOW にする       … power_tree §8
 *  3. 同時に駆動するのは 1軸のみ                       … 首と顎の同時駆動禁止
 *                                                       レールA同時ストール制限 (#10)
 *  4. PCA9685 の All Call（0x70）を初期化時に無効化    … OPEN_ISSUES #7
 *  5. パルス幅は 1000-2000us の範囲外を受け付けない
 *
 * ============================================================
 *  安全規則（コードでは強制できない。人間が守るもの）
 * ============================================================
 *  - ポンプ・高トルクサーボを Arduino の 5V 端子から給電しない。
 *    必ずレールA/B（PCA9685のV+端子）から給電する。       … 手順書 L-Phase 1
 *  - GNDは Arduino / PCA9685 / サーボ電源で必ず共通にする。
 *  - 中央出しは「サーボを機構へ取り付ける前」に単体で行う。
 *    取り付けた状態で 1500us を送ると、機構が可動域外へ回る恐れがある。
 *  - 顎（jaw）は中立1500usではなく閉位置0°を基準とする特殊軸。
 *    本スケッチでは駆動対象から除外している。
 *
 * ============================================================
 *  依存ライブラリ
 * ============================================================
 *  Adafruit PWM Servo Driver Library (v2.4.0 以降)
 *  Arduino IDE: ライブラリマネージャで "Adafruit PWM Servo Driver" を検索
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ------------------------------------------------------------
// 定数
// ------------------------------------------------------------
const uint8_t  PIN_PCA_OE      = 4;          // 両基板共通OE。HIGH=出力停止
const uint8_t  ADDR_PCA1       = 0x40;
const uint8_t  ADDR_PCA2       = 0x41;

const uint16_t SERVO_FREQ_HZ   = 50;         // アナログサーボ標準
const uint32_t PCA_OSC_HZ      = 27000000UL; // PCA9685内蔵発振器の実測代表値

const uint16_t NEUTRAL_US      = 1500;       // 中立（未校正の標準値）
const uint16_t US_HARD_MIN     = 1000;       // これ未満は受け付けない
const uint16_t US_HARD_MAX     = 2000;       // これ超過は受け付けない
const uint16_t US_STEP         = 10;         // +/- で動かす刻み

const uint8_t  PCA_MODE1_REG   = 0x00;
const uint8_t  PCA_ALLCALL_BIT = 0x01;       // MODE1 bit0

// ------------------------------------------------------------
// 軸テーブル（docs/L/channel_map_L.md と一致させること）
// ------------------------------------------------------------
enum SysTag : uint8_t { SYS_L, SYS_R, SYS_S, SYS_RSV };

struct Axis {
  uint8_t     board;      // 1 = 0x40, 2 = 0x41
  uint8_t     ch;
  const char* name;
  SysTag      sys;
  bool        wired2026;  // 2026年に物理接続してよい軸か
  const char* note;
};

const Axis AXES[] PROGMEM = {
  // --- PCA9685 #1 (0x40) / レールA 5.8V ---
  { 1,  0, "upperLip",         SYS_R, false, "R系2027" },
  { 1,  1, "cheekRight",       SYS_R, false, "R系2027" },
  { 1,  2, "cheekLeft",        SYS_R, false, "R系2027" },
  { 1,  3, "eyebrowRight",     SYS_R, false, "R系2027" },
  { 1,  4, "eyebrowLeft",      SYS_R, false, "R系2027" },
  { 1,  5, "foreheadRight",    SYS_R, false, "R系2027" },
  { 1,  6, "foreheadLeft",     SYS_R, false, "R系2027" },
  { 1,  7, "eyeLeftX",         SYS_L, false, "サーボ未発注" },
  { 1,  8, "eyeLeftY",         SYS_L, false, "サーボ未発注" },
  { 1,  9, "eyeRightX",        SYS_L, false, "サーボ未発注" },
  { 1, 10, "eyeRightY",        SYS_L, false, "サーボ未発注" },
  { 1, 11, "tasteSensorLift",  SYS_S, true,  "MG92B在庫あり" },
  { 1, 12, "eyelidLeftUpper",  SYS_L, false, "サーボ未発注" },
  { 1, 13, "eyelidLeftLower",  SYS_L, false, "サーボ未発注" },
  { 1, 14, "eyelidRightUpper", SYS_L, false, "サーボ未発注" },
  { 1, 15, "eyelidRightLower", SYS_L, false, "サーボ未発注" },
  // --- PCA9685 #2 (0x41) / レールB 5.0V ---
  { 2,  0, "neckRotate",       SYS_L, false, "HS-805BB未発注" },
  { 2,  1, "neckUpDown",       SYS_L, false, "HS-805BB未発注" },
  { 2,  2, "jaw",              SYS_L, false, "閉位置基準/型番未選定" },
  { 2,  3, "throatGate",       SYS_S, true,  "MG92B在庫あり/通常閉" },
  { 2,  4, "rollNeckRight",    SYS_RSV, false, "採用未定" },
  { 2,  5, "rollNeckLeft",     SYS_RSV, false, "採用未定" }
};
const uint8_t AXIS_COUNT = sizeof(AXES) / sizeof(Axis);

// ------------------------------------------------------------
// グローバル状態
// ------------------------------------------------------------
Adafruit_PWMServoDriver pca1(ADDR_PCA1);
Adafruit_PWMServoDriver pca2(ADDR_PCA2);

bool     outputEnabled = false;  // OE の状態（false = HIGH = 停止）
int8_t   activeBoard   = -1;     // 現在駆動中の軸（-1 = なし）
int8_t   activeCh      = -1;
uint16_t activeUs      = NEUTRAL_US;

char     lineBuf[40];
uint8_t  lineLen = 0;

// ------------------------------------------------------------
// 低レベル
// ------------------------------------------------------------
Adafruit_PWMServoDriver* boardOf(uint8_t board) {
  return (board == 1) ? &pca1 : &pca2;
}

/*
 * All Call（0x70）を無効化する。
 * 工場出荷時 MODE1 の ALLCALL ビットが 1 のため、2枚の基板が
 * 両方 0x70 に応答してしまう。誤って 0x70 へ書くと両基板の
 * サーボが同時に動く。 → OPEN_ISSUES #7
 */
bool disableAllCall(uint8_t addr) {
  Wire.beginTransmission(addr);
  Wire.write(PCA_MODE1_REG);
  if (Wire.endTransmission() != 0) return false;

  if (Wire.requestFrom(addr, (uint8_t)1) != 1) return false;
  uint8_t mode1 = Wire.read();

  mode1 &= ~PCA_ALLCALL_BIT;

  Wire.beginTransmission(addr);
  Wire.write(PCA_MODE1_REG);
  Wire.write(mode1);
  return (Wire.endTransmission() == 0);
}

void setOutputEnabled(bool on) {
  outputEnabled = on;
  digitalWrite(PIN_PCA_OE, on ? LOW : HIGH);
  Serial.print(F(">> OE = "));
  Serial.println(on ? F("LOW（出力ON）") : F("HIGH（出力停止）"));
}

// 全chへ中立パルスを「書き込むだけ」。OEがHIGHなら実際には出力されない。
void writeNeutralAll() {
  for (uint8_t ch = 0; ch < 16; ch++) {
    pca1.writeMicroseconds(ch, NEUTRAL_US);
    pca2.writeMicroseconds(ch, NEUTRAL_US);
  }
  Serial.print(F(">> 全32chへ中立 "));
  Serial.print(NEUTRAL_US);
  Serial.println(F("us を書き込んだ"));
}

// 現在駆動中の軸のパルスを止める（サーボは脱力する）
void releaseActive() {
  if (activeBoard < 0) return;
  boardOf(activeBoard)->setPWM(activeCh, 0, 0);
  Serial.print(F(">> 解放: board"));
  Serial.print(activeBoard);
  Serial.print(F(" ch"));
  Serial.println(activeCh);
  activeBoard = -1;
  activeCh    = -1;
}

// ------------------------------------------------------------
// 軸テーブル参照
// ------------------------------------------------------------
int8_t findAxis(uint8_t board, uint8_t ch) {
  for (uint8_t i = 0; i < AXIS_COUNT; i++) {
    Axis a;
    memcpy_P(&a, &AXES[i], sizeof(Axis));
    if (a.board == board && a.ch == ch) return i;
  }
  return -1;
}

void printAxisList() {
  Serial.println(F("--------------------------------------------------"));
  Serial.println(F("B CH  軸名               系統 2026接続 備考"));
  Serial.println(F("--------------------------------------------------"));
  for (uint8_t i = 0; i < AXIS_COUNT; i++) {
    Axis a;
    memcpy_P(&a, &AXES[i], sizeof(Axis));
    Serial.print(a.board);
    Serial.print(' ');
    if (a.ch < 10) Serial.print(' ');
    Serial.print(a.ch);
    Serial.print(F("  "));
    Serial.print(a.name);
    for (uint8_t p = strlen(a.name); p < 19; p++) Serial.print(' ');
    switch (a.sys) {
      case SYS_L:   Serial.print(F("L    ")); break;
      case SYS_R:   Serial.print(F("R    ")); break;
      case SYS_S:   Serial.print(F("S    ")); break;
      default:      Serial.print(F("予約 ")); break;
    }
    Serial.print(a.wired2026 ? F("可      ") : F("不可    "));
    Serial.println(a.note);
  }
  Serial.println(F("--------------------------------------------------"));
}

// ------------------------------------------------------------
// コマンド処理
// ------------------------------------------------------------
void printHelp() {
  Serial.println();
  Serial.println(F("=== ServoCenter コマンド ==="));
  Serial.println(F("  ?              このヘルプ"));
  Serial.println(F("  l              軸一覧を表示"));
  Serial.println(F("  i              I2Cスキャン"));
  Serial.println(F("  e              出力ON  (OE=LOW)"));
  Serial.println(F("  d              出力OFF (OE=HIGH)"));
  Serial.println(F("  n              全chへ中立を書き込む（出力はしない）"));
  Serial.println(F("  c <B> <CH>     指定軸を中立1500usで駆動（他軸は解放）"));
  Serial.println(F("  p <B> <CH> <US> 指定軸を任意パルスで駆動"));
  Serial.println(F("  +              現在軸を +10us"));
  Serial.println(F("  -              現在軸を -10us"));
  Serial.println(F("  r              現在軸を解放（脱力）"));
  Serial.println(F("  s              全停止（解放 + OE=HIGH）"));
  Serial.println(F("  B=1 → 0x40 / B=2 → 0x41"));
  Serial.println();
}

void i2cScan() {
  Serial.println(F(">> I2Cスキャン開始"));
  uint8_t found = 0;
  for (uint8_t addr = 0x08; addr <= 0x77; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print(F("   0x"));
      if (addr < 16) Serial.print('0');
      Serial.print(addr, HEX);
      if (addr == 0x40) Serial.print(F("  PCA9685 #1"));
      if (addr == 0x41) Serial.print(F("  PCA9685 #2"));
      if (addr == 0x44) Serial.print(F("  INA219 #A"));
      if (addr == 0x45) Serial.print(F("  INA219 #B"));
      if (addr == 0x70) Serial.print(F("  All Call（無効化に失敗している）"));
      Serial.println();
      found++;
    }
  }
  Serial.print(F(">> "));
  Serial.print(found);
  Serial.println(F(" 個検出"));
}

void driveAxis(uint8_t board, uint8_t ch, uint16_t us) {
  if (board != 1 && board != 2) {
    Serial.println(F("!! B は 1 か 2"));
    return;
  }
  if (ch > 15) {
    Serial.println(F("!! CH は 0-15"));
    return;
  }
  if (us < US_HARD_MIN || us > US_HARD_MAX) {
    Serial.print(F("!! パルスは "));
    Serial.print(US_HARD_MIN);
    Serial.print(F("-"));
    Serial.print(US_HARD_MAX);
    Serial.println(F("us の範囲"));
    return;
  }
  if (!outputEnabled) {
    Serial.println(F("!! OEがHIGH（出力停止）。先に e を実行しろ"));
    return;
  }

  int8_t idx = findAxis(board, ch);
  if (idx >= 0) {
    Axis a;
    memcpy_P(&a, &AXES[idx], sizeof(Axis));
    if (!a.wired2026) {
      Serial.print(F("!! 警告: "));
      Serial.print(a.name);
      Serial.print(F(" は2026年時点で物理接続対象外（"));
      Serial.print(a.note);
      Serial.println(F("）"));
      Serial.println(F("   ベンチ単体での中央出しなら続行してよい"));
    }
    if (a.sys == SYS_R) {
      Serial.println(F("!! 警告: R系軸は2026年に配線・給電しない規則（決定2）"));
    }
  } else {
    Serial.println(F("!! 軸テーブルに無いチャンネル"));
  }

  // 単軸駆動ルール：別の軸が動いていたら先に解放する
  if (activeBoard >= 0 && !(activeBoard == (int8_t)board && activeCh == (int8_t)ch)) {
    releaseActive();
  }

  boardOf(board)->writeMicroseconds(ch, us);
  activeBoard = board;
  activeCh    = ch;
  activeUs    = us;

  Serial.print(F(">> board"));
  Serial.print(board);
  Serial.print(F(" ch"));
  Serial.print(ch);
  Serial.print(F(" ← "));
  Serial.print(us);
  Serial.println(F("us"));
}

void nudge(int16_t delta) {
  if (activeBoard < 0) {
    Serial.println(F("!! 駆動中の軸が無い。先に c か p を実行しろ"));
    return;
  }
  int32_t next = (int32_t)activeUs + delta;
  if (next < US_HARD_MIN || next > US_HARD_MAX) {
    Serial.println(F("!! 範囲外。これ以上動かさない"));
    return;
  }
  driveAxis(activeBoard, activeCh, (uint16_t)next);
}

void handleLine(char* s) {
  while (*s == ' ') s++;
  if (*s == '\0') return;

  char cmd = *s;
  s++;

  switch (cmd) {
    case '?': printHelp();      break;
    case 'l': printAxisList();  break;
    case 'i': i2cScan();        break;
    case 'e': setOutputEnabled(true);  break;
    case 'd': releaseActive(); setOutputEnabled(false); break;
    case 'n': writeNeutralAll();break;
    case 'r': releaseActive();  break;
    case '+': nudge(US_STEP);   break;
    case '-': nudge(-US_STEP);  break;
    case 's':
      releaseActive();
      setOutputEnabled(false);
      Serial.println(F(">> 全停止"));
      break;
    case 'c': {
      int b = -1, ch = -1;
      if (sscanf(s, "%d %d", &b, &ch) == 2) {
        driveAxis((uint8_t)b, (uint8_t)ch, NEUTRAL_US);
      } else {
        Serial.println(F("!! 書式: c <B> <CH>   例: c 2 0"));
      }
      break;
    }
    case 'p': {
      int b = -1, ch = -1, us = -1;
      if (sscanf(s, "%d %d %d", &b, &ch, &us) == 3) {
        driveAxis((uint8_t)b, (uint8_t)ch, (uint16_t)us);
      } else {
        Serial.println(F("!! 書式: p <B> <CH> <US>   例: p 2 0 1450"));
      }
      break;
    }
    default:
      Serial.println(F("!! 不明なコマンド。? でヘルプ"));
      break;
  }
}

// ------------------------------------------------------------
// setup / loop
// ------------------------------------------------------------
void setup() {
  // --- 規則1: 何よりも先に OE を HIGH にする ---
  pinMode(PIN_PCA_OE, OUTPUT);
  digitalWrite(PIN_PCA_OE, HIGH);
  outputEnabled = false;

  Serial.begin(115200);
  while (!Serial) { ; }

  Serial.println();
  Serial.println(F("=================================================="));
  Serial.println(F(" ServoCenter.ino  (再作成版 2026-08-06 / 実機未検証)"));
  Serial.println(F("=================================================="));
  Serial.println(F(" OE=HIGH で起動した。出力は停止している。"));

  Wire.begin();

  pca1.begin();
  pca1.setOscillatorFrequency(PCA_OSC_HZ);
  pca1.setPWMFreq(SERVO_FREQ_HZ);

  pca2.begin();
  pca2.setOscillatorFrequency(PCA_OSC_HZ);
  pca2.setPWMFreq(SERVO_FREQ_HZ);

  // --- 規則4: All Call を無効化する（OPEN_ISSUES #7）---
  bool ok1 = disableAllCall(ADDR_PCA1);
  bool ok2 = disableAllCall(ADDR_PCA2);
  Serial.print(F(" All Call無効化: #1="));
  Serial.print(ok1 ? F("OK") : F("失敗"));
  Serial.print(F(" / #2="));
  Serial.println(ok2 ? F("OK") : F("失敗"));
  if (!ok1 || !ok2) {
    Serial.println(F(" !! 0x70への同時応答が残っている。配線とアドレスを確認しろ"));
  }

  // --- 規則2: 中立を書き込んでおく（OEはHIGHのまま）---
  writeNeutralAll();

  Serial.println();
  Serial.println(F(" 準備完了。? でヘルプ。"));
  Serial.println(F(" 出力するには e を打つこと。"));
  Serial.println();
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      lineBuf[lineLen] = '\0';
      handleLine(lineBuf);
      lineLen = 0;
    } else if (lineLen < sizeof(lineBuf) - 1) {
      lineBuf[lineLen++] = c;
    }
  }
}
