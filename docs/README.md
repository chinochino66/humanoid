# docs

版数: v1.2
改訂日: 2026-08-06
準拠文書: 器創造計画 2026-2033（改訂基準日 2026-08-03）
系統タグ: 全系統共通（メタ文書）
変更履歴:
- 2026-08-04 新規作成（旧H-Phase体系からL/R/S/M体系への移行に伴う）
- 2026-08-04 ディレクトリ構成表を更新。コミット5で追加した3文書（i2c_address_map.md／safety_logic.md／mcu_pinmap.md）を追加し、docs/decisions/配下のファイル名を明記。全13文書（本ファイル含む）が構成表から辿れる状態にした。
- **2026-08-05** 「手順書成果物名との対応」セクションを新設。蓮冥頭部 開発手順書 Version 2.0（2026-08-03）の成果物名と本リポジトリ文書との対応を明記した。未作成の4件を `docs/OPEN_ISSUES.md` へ影響度「中」で新規登録（#34〜#37）。
- **v1.2 2026-08-06** ディレクトリ構成表の `channel_map_L2.md` 説明を「目・瞼・首・顎の7ch」から「眼球4軸・瞼4軸・首2軸・顎1軸の計11ch」へ修正（目の軸数訂正の反映漏れ）。

## 体系概要 L/R/S/M

| 系統 | 内容 |
|---|---|
| L | InMoov 100%学習機（非商用。L5で凍結。R系へ形状・型・STLを移さない） |
| R | 独自商用頭部（白紙CADから新規設計。2027年R1-R9） |
| S | 安全・制御・知覚（InMoov由来ではない。R系へ再利用可） |
| M | 実咀嚼・搬送・洗浄・排出（2034年以降） |

## ディレクトリ構成

```
docs/
  README.md                        本ファイル。L/R/S/M体系とディレクトリ構成の説明、知財境界の明示
  decisions/                       意思決定の記録
    2026-08-03_lrsm_migration.md   H-Phase体系からL/R/S/M体系への移行決定記録（経緯・理由・知財根拠）
  archive/                         旧H-Phase体系の退避文書（参照専用、編集禁止）
  OPEN_ISSUES.md                   未確認事項一覧（影響度別に横断管理）
  L/                                L系（InMoov学習機、非商用）
    servo_assignment_L2.md         L2頭部の軸とサーボ割当。首サーボ発注保留の解除を記録
    channel_map_L2.md              L2頭部チャンネル表（眼球4軸・瞼4軸・首2軸・顎1軸の計11ch）
    failure_log/
      scale_94.md                  94%スケール不採用の失敗記録
  R/                                R系（独自商用頭部、2027年〜）
    reserved_axes.md               R系予約軸表（表情7軸）と在庫（MG92B／SG92R）の紐付け
  S/                                S系（安全・制御・知覚）
    power_tree.md                  電源系統図。レール定義・保護部品・稼働時間計算
    bench_axes_S3S4.md             S3/S4ベンチ軸表（喉ゲート・味覚リフト・ポンプ・漏水検知）
    i2c_address_map.md             I²Cアドレスマップ全体。PCA9685のAll Callアドレス重複の扱い
    safety_logic.md                頭部全体の固定安全ロジック（E-STOP等8条件）。第3層未実装のリスクを記録
    mcu_pinmap.md                  Arduino Megaピン表全体
  reference/                        系統非依存の仕様表
    servo_datasheets.md            サーボ諸元表（軸割当を含まない、メーカー一次情報で確認済み）
```

## 知財境界の明示

L系はInMoov公式設計（Attribution-NonCommercial、出典 inmoov.fr）に100%準拠した非商用学習機であり、形状・寸法・STL・メッシュはInMoov由来のものをそのまま使用する。L5到達をもって凍結し、以降L系の開発は行わない。

R系はL系と物理的・データ的に独立した白紙CADからの新規設計であり、L系のInMoov由来の形状・寸法・メッシュへの参照を一切持ち込まない。独自商用頭部として2027年に着手する（R1-R9）。

S系（安全・制御・知覚）はInMoov非由来であり、L系・R系いずれにも再利用可能。

M系（実咀嚼・搬送・洗浄・排出、2034年以降）はR系頭部への実装を前提とする。

## 旧H-Phase → 新体系 置換対応表

| 旧（H-Phase） | 新（L/R/S/M） |
|---|---|
| H0-H1 | L0-L1 |
| H2-H5 | L2-L5（InMoov 100%学習機・非商用） |
| H6-H10 | R5-R9 + S1-S4（独自商用頭部・2027年） |
| H11-H13 | S3-S6（味覚ベンチ・安全・オンボードAI） |
| H14 | R10 |
| H15-H24 | M0-M9（2034年以降） |

旧文書は `docs/archive/` に退避済み（参照専用）。移行の経緯・判断根拠は [`docs/decisions/2026-08-03_lrsm_migration.md`](decisions/2026-08-03_lrsm_migration.md) を参照。

## 手順書成果物名との対応

準拠文書：蓮冥頭部 開発手順書 Version 2.0（2026-08-03）

手順書が定義する成果物名と、本リポジトリ内の対応文書の紐付けを示す。未作成の成果物は `docs/OPEN_ISSUES.md` で影響度別に管理する。

| 手順書成果物名 | 対応文書 |
|---|---|
| S_PowerArchitecture_v1 | `docs/S/power_tree.md` |
| S_HeadBus_Pinout_v1 | `docs/S/mcu_pinmap.md` |
| S_IndependentSafetyController_v1 | `docs/S/safety_logic.md`（要求のみ。実装未選定） |
| S_PowerCut_TestReport_v1 | 未作成（実機試験後） |
| L_NeckRotate_Baseline_v1 | 未作成（L2実機試験の記録） |
| L2_PartsScale_Record_v1 | 未作成 |
| L2_Wiring_and_Stop_Test_v1 | 未作成 |

**未作成の4件（S_PowerCut_TestReport_v1／L_NeckRotate_Baseline_v1／L2_PartsScale_Record_v1／L2_Wiring_and_Stop_Test_v1）は、L2完了ゲートに必要な成果物であるため `docs/OPEN_ISSUES.md` へ影響度「中」で登録済み（#34〜#37）。**

## 参照一覧

- 器創造計画 2026-2033（改訂基準日 2026-08-03）
- 蓮冥頭部 開発手順書 Version 2.0（2026-08-03） 取得日: 2026-08-03
- 蓮冥頭部 H-Phase 0-24 改訂手順書（2026-08-03改訂、旧体系） 取得日: 2026-08-03
