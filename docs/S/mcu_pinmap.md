# Arduino Mega ピン表（第2層）

版数: v1
改訂日: 2026-08-04
準拠文書: 器創造計画 2026-2033（改訂基準日 2026-08-03）
系統タグ: S（安全・制御・知覚。頭部第2層マイコンのピン割当全体）
変更履歴:
- v1 2026-08-04 `docs/S/bench_axes_S3S4.md` §6で「担当文書未確定」としていたArduino Megaピン表の全体像を新規文書として分離。ベンチ関連ピン（D2/D3/D5-D7/A0-A2）は `docs/S/bench_axes_S3S4.md` §2・§3を正とし、本書では相互参照に留めた。

**本書の位置づけ**：Arduino Mega（第2層）の全ピン割当を一覧する。ベンチ関連（漏水・満水・トレー検知、ペリスタポンプ）の詳細仕様は `docs/S/bench_axes_S3S4.md` を正とする。I²Cアドレスの詳細は `docs/S/i2c_address_map.md`、安全ロジックの詳細は `docs/S/safety_logic.md` を参照。

---

## 1. ピン一覧

| ピン | 機能 | 方向 | 詳細の所在 | 備考 |
|---|---|---|---|---|
| D2 | mouthTrayDetect | IN（PULLUP） | `docs/S/bench_axes_S3S4.md` §3 | トレー装着スイッチ。未装着＝口内系コマンド拒否 |
| D3 | tankFullSensor | IN（PULLUP） | `docs/S/bench_axes_S3S4.md` §3 | 回収タンク満水 |
| D4 | **PCA_OE**（PCA9685 #1・#2共通） | OUT | 本書（下記） | HIGH=全サーボ出力停止。起動時HIGH、中立書込後にLOW。両基板へ同一配線 |
| D5 | drainPump（MOSFET ch1） | OUT/PWM | `docs/S/bench_axes_S3S4.md` §2 | ソフトタイマで連続駆動時間を制限 |
| D6 | foodTransferPump（MOSFET ch2） | OUT/PWM | `docs/S/bench_axes_S3S4.md` §2 | 将来 |
| D7 | washPump（MOSFET ch3） | OUT/PWM | `docs/S/bench_axes_S3S4.md` §2 | 将来 |
| D8 | **jawModuleID bit0** | IN（PULLUP） | 本書（下記） | 顎モジュール識別 |
| D9 | **jawModuleID bit1** | IN（PULLUP） | 本書（下記） | 〃（00=表情顎／01=咀嚼顎／11=未装着） |
| D10 | **安全系ハートビート出力** | OUT | `docs/S/safety_logic.md` §1 #5 | 第3層のWDTへ。停止すれば遮断 |
| D11 | **安全系ステータス入力** | IN | `docs/S/safety_logic.md` §1 | 第3層からの遮断状態通知 |
| D18 | E-STOP状態（NO接点） | IN（PULLUP） | `docs/S/safety_logic.md` §1 #1 | 状態通知のみ。**遮断は第3層が物理的に行う** |
| A0 | 漏水センサー1 | IN | `docs/S/bench_axes_S3S4.md` §3 | 口内トレー直下 |
| A1 | 漏水センサー2 | IN | `docs/S/bench_axes_S3S4.md` §3 | 頭蓋内・電装区画 |
| A2 | 漏水センサー3 | IN | `docs/S/bench_axes_S3S4.md` §3 | 回収タンク周辺 |
| A3 | **基板温度センサー** | IN | `docs/S/safety_logic.md` §1 #3 | 過熱監視 |
| D20/D21 | SDA/SCL | I²C | `docs/S/i2c_address_map.md` | PCA9685×2＋INA219×2 |
| （予約）D14/D15 | **Serial3 → CAN or RS-485** | — | 本書（下記） | 第1層との通信。ECI-v1 |
| USB Serial0 | 保守用リンク | — | 本書（下記） | **通常運転経路に含めない**。開発・更新・ログ回収のみ |

## 2. 本書固有ピンの詳細

### D4 — PCA_OE（両基板共通）

PCA9685 #1・#2の出力イネーブル（OE）ピンは、Megaの単一ピンD4から両基板へ同一配線される。HIGHで両基板とも全チャンネル出力停止、LOWで出力再開。起動シーケンス（`docs/S/power_tree.md` §8）でHIGHから開始し、全chへ中立値書込後にLOWへ移行する。

この配線により、両基板の出力停止・再開は**I²Cを介さずハードウェアレベルで既に同時制御できている**。`docs/S/i2c_address_map.md` §3で検討したPCA9685のAll Callアドレス（I²C経由の同時書き込み）とは独立した仕組みである。

### D8/D9 — jawModuleID bit0/bit1

顎モジュールの種別を2bitで識別する（00=表情顎／01=咀嚼顎／11=未装着／10=未定義）。`docs/S/safety_logic.md` §1 #7「モジュールID不明」の検知に用いる。モジュールIDに応じた具体的な制限値の切替ロジック（顎モジュールモード）は `docs/R/reserved_axes.md` へモード名のみ申し送った（角度・パルス幅等の具体値はInMoov由来の可能性があるため本書・申し送り先のいずれにも記載しない）。

### D14/D15（予約） — Serial3

第1層（胴体内AIコンピュータ）との通信を、将来CANまたはRS-485トランシーバ経由でSerial3に接続する予約枠。トランシーバの選定は未決（§3）。現況はUSBシリアル+I²Cで代行している（`docs/S/power_tree.md` §4）。

### USB Serial0

開発・更新・ログ回収専用。**通常運転の依存関係にノートPCを含めない**という設計方針（`docs/S/power_tree.md` §0）により、通常運転の経路には含めない。

## 3. 未決事項

- [ ] 第3層マイコンの選定とピン割当（D10/D11の対向ピンを含む）
- [ ] CAN / RS-485 トランシーバの選定（ECI-v1、D14/D15）
- [ ] 基板温度センサーの実装位置と閾値（`docs/S/safety_logic.md` §3）

---

## 変更履歴

- **v1 2026-08-04** `docs/S/bench_axes_S3S4.md` から分離して新規作成。ベンチ関連ピンはそちらを正として相互参照に留めた。

## 参照一覧

| 資料 | URL | 取得日 |
|---|---|---|
| `docs/S/bench_axes_S3S4.md` | ローカルリポジトリ内 | 2026-08-04 |
| `docs/S/i2c_address_map.md` | ローカルリポジトリ内 | 2026-08-04 |
| `docs/S/safety_logic.md` | ローカルリポジトリ内 | 2026-08-04 |
| `docs/S/power_tree.md` | ローカルリポジトリ内 | 2026-08-04 |
| `docs/R/reserved_axes.md` | ローカルリポジトリ内 | 2026-08-04 |
| 旧文書 `docs/archive/2026-08-03_channel_map_hphase.md` | ローカルリポジトリ内 | 2026-08-03 |
