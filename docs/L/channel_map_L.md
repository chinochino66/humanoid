# L2 InMoov頭部 チャンネル表

版数: v1.4
改訂日: 2026-08-06
準拠文書: 器創造計画 2026-2033（改訂基準日 2026-08-03）
系統タグ: L（InMoov 100%学習機・非商用。L5で凍結）
変更履歴:
- v1 2026-08-04 旧H-Phase体系文書 `docs/archive/2026-08-03_channel_map_hphase.md` から、L系の軸のみを分離して新規作成。R系軸（upperLip/cheek×2/eyebrow×2/forehead×2）・S系軸（tasteSensorLift/throatGate）を1行も含めない。首サーボ（neckRotate/neckUpDown）は決定1により発注保留を解除した状態で記載（`docs/L/servo_assignment_L.md` 参照）。顎の試験角度15°とハードコード上限20°の区別を維持した。
  - forehead軸の綴り誤り（forhead→forehead）を修正。R系は白紙CADの新規設計であり、InMoov公式パーツ名の誤字を踏襲する理由がないため。
  - **目の軸数の誤りを修正（重要）。**一次情報 `inmoov.fr/eyes-i2/`（取得日2026-08-04）を確認した結果、目はeyeX/eyeYの2軸共通軸ではなく、**眼球4軸＋瞼4軸＝計8軸（左右独立、ソフトウェア同期）**であると判明した。L系チャンネル数を7ch→11chへ修正した。経緯は `docs/decisions/2026-08-04_eye_axis_count_correction.md` を参照。

**本書に含めないもの**：R系軸（upperLip／cheekRight／cheekLeft／eyebrowRight／eyebrowLeft／foreheadRight／foreheadLeft）は `docs/R/reserved_axes.md`、S系軸（tasteSensorLift／throatGate）とESP32・安全ロジック等は `docs/S/bench_axes_S3S4.md` を参照。

---

## 1. L系チャンネル一覧（計11ch）

**目の軸数について**：`inmoov.fr/eyes-i2/`（取得日2026-08-04）の一次情報により、i2Eyesの部品表は「i2Eyes printed parts for 1 Right Eye」＝片目分の記載であり、片目あたり4サーボ（main baseに2個＝眼球X/Y、side baseに2個＝上瞼・下瞼）、両目で計8サーボと判明した。i2Eyesは**左右独立サーボであり、機械的な連動ではなくMyRobotLab側のソフトウェア同期**で左右を揃える設計である（作者Gael Langevinの記載）。旧版の「eyeX/eyeY共通2軸」という記述は誤りだった。経緯は `docs/decisions/2026-08-04_eye_axis_count_correction.md` を参照。

| ボードアドレス | CH | 軸名 | 系統 | サーボ型番 | レール | 中立パルス(µs) | 下限 | 上限 | 機械可動域(deg) | ソフト制限 | 方向反転 | 延長ケーブル長 | コネクタ | 状態 | 備考 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 0x40（PCA9685 #1） | 7 | eyeLeftX | L | JX PDI-1109MG（InMoov公式指定、未発注）。在庫MG90S×4は8軸中4軸分のみで不足 | レールA（5.8V） | 1500µs（標準値、未校正） | 未実測 | 未実測 | 未確認（`docs/reference/servo_datasheets.md` §4参照） | 未定 | 未定 | 未定 | JR（JX PDI-1109MG準拠、要確認） | 未実装（型番未確定・追加調達要） | 左目眼球水平。左右独立サーボ、ソフトウェア同期（機械連動ではない）。出典: inmoov.fr/eyes-i2/ |
| 0x40（PCA9685 #1） | 8 | eyeLeftY | L | 同上 | レールA（5.8V） | 1500µs（標準値、未校正） | 未実測 | 未実測 | 未確認 | 未定 | 未定 | 未定 | JR（同上） | 未実装（同上） | 左目眼球垂直 |
| 0x40（PCA9685 #1） | 9 | eyeRightX | L | 同上 | レールA（5.8V） | 1500µs（標準値、未校正） | 未実測 | 未実測 | 未確認 | 未定 | 未定 | 未定 | JR（同上） | 未実装（同上） | 右目眼球水平 |
| 0x40（PCA9685 #1） | 10 | eyeRightY | L | 同上 | レールA（5.8V） | 1500µs（標準値、未校正） | 未実測 | 未実測 | 未確認 | 未定 | 未定 | 未定 | JR（同上） | 未実装（同上） | 右目眼球垂直 |
| 0x40（PCA9685 #1） | 12 | eyelidLeftUpper | L | JX PDI-1109MG（公式指定・#33解決済み） | レールA（5.8V） | 1500µs（標準値、未校正） | 未実測 | 未実測 | 未確認 | 未定 | 未定 | 未定 | JR（同上） | 未実装（同上） | 左上瞼。公式はSG90級（安価版）を「長持ちしない」として非推奨。SG92R充当は却下（#33） |
| 0x40（PCA9685 #1） | 13 | eyelidLeftLower | L | JX PDI-1109MG（公式指定・#33解決済み） | レールA（5.8V） | 1500µs（標準値、未校正） | 未実測 | 未実測 | 未確認 | 未定 | 未定 | 未定 | JR（同上） | 未実装（同上） | 左下瞼 |
| 0x40（PCA9685 #1） | 14 | eyelidRightUpper | L | JX PDI-1109MG（公式指定・#33解決済み） | レールA（5.8V） | 1500µs（標準値、未校正） | 未実測 | 未実測 | 未確認 | 未定 | 未定 | 未定 | JR（同上） | 未実装（同上） | 右上瞼 |
| 0x40（PCA9685 #1） | 15 | eyelidRightLower | L | JX PDI-1109MG（公式指定・#33解決済み） | レールA（5.8V） | 1500µs（標準値、未校正） | 未実測 | 未実測 | 未確認 | 未定 | 未定 | 未定 | JR（同上） | 未実装（同上） | 右下瞼 |
| 0x41（PCA9685 #2） | 0 | neckRotate | L | HS-805BB ×2（決定1により発注保留解除、未発注。`docs/L/servo_assignment_L.md` §2.3参照） | レールB（5.0V） | 1500µs（標準値、未校正） | 未実測 | 未実測 | 小角度から実測開始 | 単軸駆動ルール適用 | 未定 | 未定 | 未定（公式HS-805BB準拠） | 未実装（未発注） | 首と顎の同時駆動禁止。SkullServoFixV5のポケット実測（65.7×29.3、2026-08-06）がHS-805BB実寸（65.6×29.6）と一致し、公式i2HeadチュートリアルもSkullServoFixとHS-805BBの組み合わせを明記しているため、**HS-805BBで確定**（`docs/L/print_record_L.md` §3）。ServoAdapter/ServoHornAdapterを追加印刷すればJX PDI-6221MGでも代替可能（公式i2Headチュートリアル記載）。 |
| 0x41（PCA9685 #2） | 1 | neckUpDown | L | HS-805BB ×2（同上） | レールB（5.0V） | 1500µs（標準値、未校正） | 90-5°から開始（未実測） | 90+5°から開始（未実測） | 90±5°から実測開始 | 単軸駆動ルール適用 | 未定 | 未定 | 未定 | 未実装（未発注） | 首と顎の同時駆動禁止。**サーボ未確定（2026-08-06）。** 当初HS-805BBで確定としたが撤回した。保持部品 NeckServoHolderV2 の外形は 55.0 × 58.5 × 22.0 mm であり、HS-805BB（65.6 × 29.6 × 57.4 mm）は物理的に収まらない。STL上のポケット内寸を確認して規格を特定する必要がある。`docs/OPEN_ISSUES.md` #39 で継続管理する。 |
| 0x41（PCA9685 #2） | 2 | jaw | L | JX PDI-6221MG（公式指定・180°。`docs/L/servo_assignment_L.md` §2.2参照） | レールB（5.0V） | 該当なし（閉位置を0°基準とする） | 閉＝0° | **試験15°／ハードコード上限20°** | 0〜20° | **20°をソフトウェアの絶対リミットとしてハードコードする** | 未定 | 未定 | 未定 | 未実装（選定中） | 首と同時駆動禁止。機械ストッパーとソフトウェア制限の両方を併用 |

**顎の角度の区別**：試験手順は閉口→5°→10°→**15°**まで。**ハードコード上限20°**はソフトウェア側の絶対リミットであり、試験角度と混同しないこと（`docs/L/servo_assignment_L.md` §4）。

## 2. 空きチャンネル集計

| ボードアドレス | CH | 内容 | 備考 |
|---|---|---|---|
| 0x40（PCA9685 #1） | 0-6（7ch） | R系予約 | `docs/R/reserved_axes.md` 参照。2026年は文書上の予約のみで未接続 |
| 0x40（PCA9685 #1） | 7-10（4ch） | **L系（本書、眼球4軸）** | 上記§1参照 |
| 0x40（PCA9685 #1） | 11（1ch） | S系予約（tasteSensorLift） | `docs/S/bench_axes_S3S4.md` 参照 |
| 0x40（PCA9685 #1） | 12-15（4ch） | **L系（本書、瞼4軸）** | 上記§1参照。目の軸数訂正（8軸）により、旧「予備（独立視線化v2・唾液系の増設枠）」を消費した |
| 0x41（PCA9685 #2） | 0-2（3ch） | **L系（本書）** | 上記§1参照 |
| 0x41（PCA9685 #2） | 3（1ch） | S系予約（throatGate） | `docs/S/bench_axes_S3S4.md` 参照 |
| 0x41（PCA9685 #2） | 4-5（2ch） | 予約（系統未確定） | rollNeckRight／rollNeckLeft。InMoov公式のi2首傾げオプション。採用時にL系かR系か決定する |
| 0x41（PCA9685 #2） | 6-15（10ch） | 予備 | 唾液系など将来枠 |

**PCA9685 #1（0x40）は16ch全てが割当済みで空きゼロとなった。**目の軸数訂正（4ch→8ch）により、旧予備枠（ch12-15）を瞼4軸へ充てたため。将来の増設枠（独立視線化v2・唾液系等）はPCA9685 #1では確保できない。増設が必要になった場合は別基板の追加を検討すること。

## 3. レール別の同時動作最大電流（参照）

電力予算・電流上限の一次情報は `docs/S/power_tree.md` を正とする。本書ではL系チャンネルに関係する範囲のみ要約する。

| レール | 2026年のL系負荷 | 定常 | ピーク | 出典 |
|---|---|---|---|---|
| レールA（5.8V） | eyeLeftX／eyeLeftY／eyeRightX／eyeRightY／eyelidLeftUpper／eyelidLeftLower／eyelidRightUpper／eyelidRightLower（8ch） | 〜0.6〜1.0A | 現実的ピーク〜1.4A（2軸同時ストール想定）／理論上限5.6A（8軸同時ストール、要ソフト制限） | `docs/S/power_tree.md` §6.1 |
| レールB（5.0V） | neckRotate／neckUpDown／jaw（3ch。首とjawは同時駆動禁止） | 0.5〜1.5A | ストール時〜3A | `docs/S/power_tree.md` §6.1 |

**レールAの理論上限（8軸同時ストール5.6A）はLM2596の実用2A枠を大きく超える（約2.8倍）ため、同時ストールは最大2軸までとするソフトウェア制限が前提となる。**目の軸数訂正（4ch→8ch）により超過幅が倍増した。詳細は `docs/S/power_tree.md` §6.1・`docs/OPEN_ISSUES.md` #10 を参照。

## 4. 未決事項

- [ ] 首サーボ確定後、ch0(neckRotate)／ch1(neckUpDown)の角度リミットを実測で埋める
- [ ] jaw選定確定。公式指定JX PDI-6221MGとJawPistonの取付穴ピッチの照合が未実施（`docs/L/servo_assignment_L.md` §2.2、`docs/OPEN_ISSUES.md` #11）
- [ ] パルスmin/max・中立パルス実測欄をキャリブレーション時に記入
- [ ] コネクタ・延長ケーブル長の確定
- [ ] rollNeckRight／rollNeckLeft（PCA2 ch4-5）を採用する場合の系統決定（L系かR系か）
- [ ] 目サーボ4本の追加調達（在庫MG90S×4では8軸に不足。`docs/OPEN_ISSUES.md` #31）
- [ ] 左右独立サーボのソフトウェア同期の実装（`docs/OPEN_ISSUES.md` #32）

---

## 変更履歴

- **v1 2026-08-04** 旧H-Phase体系文書からL系チャンネルのみを分離して新規作成。空きチャンネル集計とレール別電流予算の参照を追加。
- **v1.1 2026-08-04** `inmoov.fr/eyes-i2/` の一次情報により目の軸数の誤りを発見・修正。L系チャンネル数を7ch→11chへ拡張し、空きチャンネル集計・レール別電流予算を再計算した。経緯は `docs/decisions/2026-08-04_eye_axis_count_correction.md` を参照。
- **v1.2 2026-08-06** `docs/L/channel_map_L2.md` から `docs/L/channel_map_L.md` へリネーム。中身がL2〜L5相当のL系全体の設計表であり、フェーズ番号を名前に含めると工程範囲の誤読を招くため、命名規則に従いフェーズ番号を外した。リポジトリ全体の相互参照リンクを追従させた。
- **v1.3 2026-08-06** ch0(neckRotate)／ch1(neckUpDown)の備考へNeckServoHolderV2ポケット実測（2026-08-06）によるHS-805BB確定を追記した。ch2(jaw)のサーボ型番をMG996R／DS3218MGからJX PDI-6221MG（公式指定・180°）へ変更。ch12-15（瞼4軸）をJX PDI-1109MG（公式指定・#33解決済み）で確定した。
- **v1.4 2026-08-06** v1.3のHS-805BB確定の根拠が部品名の取り違えであったため訂正。実測した部品はNeckServoHolderV2ではなくSkullServoFixV5であった。ch0(neckRotate)はSkullServoFixV5実測により**HS-805BBで確定**のまま維持。ch1(neckUpDown)はHS-805BB確定を撤回し**サーボ未確定**へ変更した（NeckServoHolderV2の外形55.0×58.5×22.0mmにHS-805BBが収まらないため。`docs/OPEN_ISSUES.md` #39）。

## 参照一覧

| 資料 | URL | 取得日 |
|---|---|---|
| `docs/L/servo_assignment_L.md` | ローカルリポジトリ内 | 2026-08-04 |
| `docs/reference/servo_datasheets.md` | ローカルリポジトリ内 | 2026-08-04 |
| `docs/S/power_tree.md` | ローカルリポジトリ内 | 2026-08-04 |
| InMoov Eyes i2（目の軸数8ch確認の根拠） | https://inmoov.fr/eyes-i2/ | 2026-08-04 |
| `docs/decisions/2026-08-04_eye_axis_count_correction.md` | ローカルリポジトリ内 | 2026-08-04 |
| 旧文書 `docs/archive/2026-08-03_channel_map_hphase.md` | ローカルリポジトリ内 | 2026-08-03 |
