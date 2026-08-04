# S3/S4 ベンチ軸表（喉ゲート・味覚リフト・ポンプ）

版数: v1
改訂日: 2026-08-04
準拠文書: 器創造計画 2026-2033（改訂基準日 2026-08-03）
系統タグ: S（安全・制御・知覚。頭部外ベンチ機能）
変更履歴:
- v1 2026-08-04 旧H-Phase体系文書 `docs/archive/2026-08-03_channel_map_hphase.md` から、throatGate／tasteSensorLift／ペリスタポンプ／漏水・満水・トレー検知／味覚JSONスキーマ／ESP32ノード構成と、固定安全ロジックのうちS系該当分を分離して新規作成。§6で「担当文書未確定」としていた残りの内容は `docs/S/i2c_address_map.md`・`docs/S/safety_logic.md`・`docs/S/mcu_pinmap.md`・`docs/R/reserved_axes.md` へ移送し、§6を移送先を示す表へ更新した。
  - §1のMG92B在庫に関する記述を修正。「別途調達が必要」という断定を撤回し、R系予備2個の充当（案a）と別途調達（案b）の2案併記へ変更した（`docs/R/reserved_axes.md`・`docs/OPEN_ISSUES.md` #13 と整合）。

**頭部外ベンチ。頭部の表に混ぜない。** throatGateとtasteSensorLiftは2026年時点では電気的に頭部のPCA9685基板（PCA9685 #1 ch11、PCA9685 #2 ch3）に接続されるが、系統としてはS系（頭部外ベンチ機能）として扱い、`docs/L/channel_map_L2.md`（L系）には含めない。

---

## 1. ベンチ軸チャンネル（計2ch、頭部PCA基板に接続）

| ボードアドレス | CH | 軸名 | 系統 | サーボ型番 | レール | 状態 | 備考 |
|---|---|---|---|---|---|---|---|
| 0x40（PCA9685 #1） | 11 | tasteSensorLift | S | MG92B | レールA（5.8V） | 未実装 | 味覚プレート昇降 |
| 0x41（PCA9685 #2） | 3 | throatGate | S | MG92B | レールB（5.0V） | 未実装 | **通常時クローズ。非通電時も閉じる機構が必要**（`docs/OPEN_ISSUES.md` 登録対象） |

**在庫上の注意**：`docs/R/reserved_axes.md` により、現在庫のMG92B×5個は2027年R系向け（3軸分＋予備2個）と整理されている。上表の2軸（tasteSensorLift／throatGate）を合わせるとR系3軸＋S系2軸＝計5軸となり、**数量上はR系の予備2個をS系へ充てれば足りる。**「別途調達が必要」と断定していた記述は撤回する。実装前に以下いずれかの方針を選択すること（§7未決事項）：

- 案(a) R系の予備2個をS系ベンチ軸（tasteSensorLift／throatGate）へ充てる。R系は予備なしとなる。
- 案(b) S系ベンチ軸用に別途2個調達し、R系の予備2個は維持する。

## 2. ペリスタポンプ（Arduino Mega、第2層）

| ピン | 機能 | 方向 | 状態 | 備考 |
|---|---|---|---|---|
| D5 | drainPump（MOSFET ch1） | OUT/PWM | 実装中 | ソフトタイマで連続駆動時間を制限 |
| D6 | foodTransferPump（MOSFET ch2） | OUT/PWM | 将来 | — |
| D7 | washPump（MOSFET ch3） | OUT/PWM | 将来 | — |

電力側の詳細（MOSFET・フライバックダイオード・電流予算）は `docs/S/power_tree.md` §2・§7を参照。

## 3. 漏水・満水・トレー検知（Arduino Mega、第2層）

| ピン | 機能 | 方向 | 備考 |
|---|---|---|---|
| D2 | mouthTrayDetect | IN（PULLUP） | トレー装着スイッチ。未装着＝口内系コマンド拒否 |
| D3 | tankFullSensor | IN（PULLUP） | 回収タンク満水 |
| A0 | 漏水センサー1 | IN | 口内トレー直下 |
| A1 | 漏水センサー2 | IN | 頭蓋内・電装区画 |
| A2 | 漏水センサー3 | IN | 回収タンク周辺 |

## 4. 固定安全ロジック（S系該当分・ベンチ関連のみ）

**AI・LLMに委譲しない層。**以下はベンチ（漏水・満水・トレー・ポンプ）に直接関わる条件のみを掲載する。E-STOP・ハートビート・通信断・過熱・BMS低電圧・待機タイムアウト等の頭部全体に関わる安全ロジックは、本書の対象外。`docs/S/safety_logic.md` を参照（§6参照）。

| # | 条件 | 検知 | 固定動作 | 担当層 |
|---|---|---|---|---|
| 1 | 漏水 | A0〜A2 閾値超え | **喉ゲートを閉位置へ駆動 → 完了確認 → OE=HIGH → 全ポンプOFF → アクチュエータ電源遮断** | 第3層 |
| 2 | トレー未装着 | D2 | 口内系コマンド（JAW_OPEN含む）拒否 | 第2層 |
| 3 | タンク満水 | D3 | 食品受入停止。DRAIN系以外の口内系拒否 | 第2層 |
| 4 | ポンプ連続駆動超過 | ソフトタイマ | 強制OFF＋詰まり疑いフラグ | 第2層 |

**#1の順序が重要。**先にOE=HIGHにすると喉ゲートを閉じられなくなる。**閉じてから止める。**

## 5. ESP32 味覚センサノード

| ピン | 機能 | 備考 |
|---|---|---|
| GPIO21/22 | SDA/SCL | ADS1115（0x48）、TCS34725（0x29） |
| ADS1115 A0 | pHモジュール | 校正液3点（4.00/6.86/9.18）で較正 |
| ADS1115 A1 | TDS（KS0429） | — |
| ADS1115 A2 | VOC | I²C型なら空けてバス接続 |
| ADS1115 A3 | 予備 | — |
| GPIO4 | 防水温度センサー | DS18B20系ならOneWire＋4.7kΩ。**型式要確認** |
| UART | 第1層へ味覚メッセージ送信 | 将来CAN/RS-485へ統合可 |

I²Cアドレス（ESP32側）：0x29（TCS34725、固定）／0x48（ADS1115、ADDR→GND）／0x49（ADS1115増設予約、ADDR→VDD）

**TasteCell_v1は交換式とし、将来の脂質/高分子膜センサーアレイを追加できる電極・流路空間を確保する。**

### 味覚JSONスキーマ v2

```json
{
  "ts": 0,
  "raw":        { "ph_mv": 0, "tds_mv": 0, "temp_c": 0, "rgb": [0,0,0], "voc_mv": 0 },
  "calibrated": { "ph": 0.0, "ec_us_cm": 0, "temp_c": 0.0 },
  "estimated":  { "sourness": 0.0, "saltiness": 0.0, "sweetness": 0.0, "aroma": 0.0 },
  "confidence": 0.0,
  "flags":      { "sensor_fault": false, "uncalibrated": false, "rinse_required": false }
}
```

**raw / calibrated / estimated / confidence を分離する。**pHを酸味、導電率をイオン量、VOCを香りの手掛かりとして扱い、**五味そのものと断定しない。**

## 6. 本書の範囲外の内容の移送先

旧文書 `docs/archive/2026-08-03_channel_map_hphase.md` には、本書の範囲（頭部外ベンチのS3/S4該当分）を超える内容が含まれていた。以下の3文書へ分離済み。

| 内容 | 移送先 |
|---|---|
| I²Cアドレスマップの全体像（PCA9685×2・INA219×2を含む頭部全体のバス構成、All Callアドレス重複の扱い） | `docs/S/i2c_address_map.md` |
| Arduino Megaピン表のうち、ベンチに直接関わらないもの（PCA_OE、jawModuleID bit0/1、安全系ハートビート出力・入力、E-STOP状態、基板温度センサー、CAN/RS-485予約ピン、USB保守リンク） | `docs/S/mcu_pinmap.md` |
| 固定安全ロジックのうち頭部全体に関わるもの（E-STOP押下、レール過電流、過熱、通信断、ハートビート停止、BMS低電圧、モジュールID不明、待機タイムアウト） | `docs/S/safety_logic.md` |
| 表情プリセット（`neutral`／`angry`／`suspicious`／`thin_smile`／`taste_dislike`／`taste_approve`） | `docs/R/reserved_axes.md` §4（軸名のみ。具体値は書かない） |
| 顎モジュールモード（`EXPRESSION_JAW`／`TASTE_ONLY`／`MASTICATION_BENCH`／`MASTICATION_INSTALLED`） | `docs/R/reserved_axes.md` §4（モード名のみ。具体値は書かない） |

## 7. 未決事項

- [ ] tasteSensorLift／throatGate用MG92Bの配分方針決定（案a: R系予備2個を充当／案b: 別途2個調達。§1参照）
- [ ] 喉ゲートの非通電時閉機構（ばね等）
- [ ] 防水温度センサーの型式確定

---

## 変更履歴

- **v1 2026-08-04** 旧H-Phase体系文書からS3/S4ベンチ該当分のみを分離して新規作成。頭部全体に関わる安全・制御ロジックは範囲外として`docs/S/i2c_address_map.md`・`docs/S/safety_logic.md`・`docs/S/mcu_pinmap.md`・`docs/R/reserved_axes.md`へ移送し、§6を移送先を示す表へ更新した。

## 参照一覧

| 資料 | URL | 取得日 |
|---|---|---|
| `docs/R/reserved_axes.md` | ローカルリポジトリ内 | 2026-08-04 |
| `docs/S/power_tree.md` | ローカルリポジトリ内 | 2026-08-04 |
| `docs/S/i2c_address_map.md` | ローカルリポジトリ内 | 2026-08-04 |
| `docs/S/safety_logic.md` | ローカルリポジトリ内 | 2026-08-04 |
| `docs/S/mcu_pinmap.md` | ローカルリポジトリ内 | 2026-08-04 |
| 旧文書 `docs/archive/2026-08-03_channel_map_hphase.md` | ローカルリポジトリ内 | 2026-08-03 |
