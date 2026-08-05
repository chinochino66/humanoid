# L2 InMoov頭部 軸・サーボ割当

版数: v1.2
改訂日: 2026-08-06
準拠文書: 器創造計画 2026-2033（改訂基準日 2026-08-03）
系統タグ: L（InMoov 100%学習機・非商用。L5で凍結）
変更履歴:
- v1 2026-08-04 旧H-Phase体系文書 `docs/archive/2026-08-03_servo_comparison_hphase.md` から、L2 InMoov頭部の軸割当のみを分離して新規作成。
  - **決定1に従い、首サーボ（neckRotate／neckUpDown）の発注保留を解除した。** 旧文書は将来の咀嚼顎質量（`FutureJaw_MassEnvelope_v0`）確定まで首サーボを確定しないとしていたが、咀嚼機構はM系（2034年以降）であり搭載先はR系独自頭部である。L機はL5で凍結される学習機であり咀嚼顎を載せないため、L機の首サーボ選定を咀嚼顎質量確定でブロックする根拠が無い。解除の詳細な経緯・知財上の根拠は `docs/decisions/2026-08-03_lrsm_migration.md` を参照。
  - R系軸（upperLip／cheek×2／eyebrow×2／forehead×2、決定2）とS系軸（throatGate／tasteSensorLift）はここに含めない。R系は `docs/R/reserved_axes.md`、S系は `docs/S/bench_axes_S3S4.md` を参照。
  - forehead軸の綴り誤り（forhead→forehead）を修正。R系は白紙CADの新規設計であり、InMoov公式パーツ名の誤字を踏襲する理由がないため。
  - **目の軸数の誤りを修正（重要）。**一次情報 `inmoov.fr/eyes-i2/`（取得日2026-08-04）を確認した結果、目はeyeX/eyeYの2軸共通軸ではなく、**眼球4軸＋瞼4軸＝計8軸（左右独立、ソフトウェア同期）**であると判明した。対象軸を7軸→11軸へ拡張し、InMoov公式指定のJX PDI-1109MGを候補として追加した。経緯は `docs/decisions/2026-08-04_eye_axis_count_correction.md` を参照。

---

## 0. 非商用条件と出典

L2は **InMoov公式頭部（i2Head）の設計に100%準拠**して製作する。InMoovの公式設計は **Attribution-NonCommercial** ライセンス（出典: inmoov.fr）で公開されており、本書が扱う軸・寸法・サーボ選定条件は非商用の学習・研究目的に限る。

L系はL5到達をもって凍結し、以降の開発を行わない。L系の成果物（形状・寸法・STL・メッシュ・本書の軸割当）をR系（独自商用頭部）へ転用しない。知財上の根拠の詳細は `docs/decisions/2026-08-03_lrsm_migration.md` を参照。

参考: inmoov.fr/default-hardware-map ／ inmoov.fr/neck-and-jaw ／ inmoov.fr/headi2 ／ inmoov.fr/eyes-i2

## 1. 対象軸（公式InMoov頭部のみ、計11軸）

L2の対象は公式InMoov頭部の軸のみとする。表情軸（R系、2027年R5-R9）と味覚・喉ゲート軸（S系ベンチ）はここに含めない。

**目の軸数（8軸）について**：`inmoov.fr/eyes-i2/`（取得日2026-08-04）により、i2Eyesは片目あたり4サーボ（main baseに2個＝眼球X/Y、side baseに2個＝上瞼・下瞼）、両目で計8サーボと判明した。左右は機械的に連動せず、MyRobotLab側のソフトウェア同期で揃える設計である。詳細は `docs/decisions/2026-08-04_eye_axis_count_correction.md` を参照。

| 軸名 | 要求トルク帯 | 備考 |
|---|---|---|
| eyeLeftX | micro級（〜2.5kg·cm） | 左目眼球水平。左右独立サーボ |
| eyeLeftY | micro級（〜2.5kg·cm） | 左目眼球垂直 |
| eyeRightX | micro級（〜2.5kg·cm） | 右目眼球水平 |
| eyeRightY | micro級（〜2.5kg·cm） | 右目眼球垂直 |
| eyelidLeftUpper | micro級（〜2.5kg·cm） | 左上瞼。皮膚アンカー荷重なし |
| eyelidLeftLower | micro級（〜2.5kg·cm） | 左下瞼 |
| eyelidRightUpper | micro級（〜2.5kg·cm） | 右上瞼 |
| eyelidRightLower | micro級（〜2.5kg·cm） | 右下瞼 |
| jaw | 中型（10〜20kg·cm） | 試験角度15°／ハードコード上限20°。首と同時駆動禁止 |
| neckRotate | 大型 | InMoov公式指定 HS-805BB ×2 |
| neckUpDown | 大型 | InMoov公式指定 HS-805BB ×2 |

## 2. 候補型番と5V時トルク（レールB=5.0V前提）

**選定条件**：レールBは5.0V固定のため、6V公称値ではなく **5.0V時のトルクで評価する**。5.0V時の値が公式データシートに無い機種は「未確認」とし、線形補間する場合はその旨を明記する。数値の出典は `docs/reference/servo_datasheets.md` を参照。

### 2.1 目・瞼（eyeLeftX／eyeLeftY／eyeRightX／eyeRightY／eyelidLeftUpper／eyelidLeftLower／eyelidRightUpper／eyelidRightLower）

**目の軸数（8軸）と型番の確定**：`inmoov.fr/eyes-i2/`（取得日2026-08-04）により、目は片目4サーボ（眼球X/Y・上瞼・下瞼）×左右＝計8軸、InMoov公式指定サーボは**JX PDI-1109MG**（「4 servos JX PDI 1109MG」と明記）であることが確認できた。同ページは併せて**「安価なSG90は避けろ、長持ちしない（avoid using cheap SG90, they won't last long）」**と明記している。経緯は `docs/decisions/2026-08-04_eye_axis_count_correction.md` を参照。

| 軸 | 候補型番 | 5V時トルク | 要求トルク帯との比較 | トルク余裕率（5V時トルク÷要求トルク上限） |
|---|---|---|---|---|
| eyeLeftX | JX PDI-1109MG（公式指定、未発注） | 約2.25kg·cm（4.8V/6.0V公式値間の線形補間、未実測。`docs/reference/servo_datasheets.md` §2） | 要求上限2.5kg·cmに対し約90% | ＿＿（L3で頭部・可動部の実荷重確定後に記入） |
| eyeLeftY | JX PDI-1109MG（同上） | 同上 | 同上 | ＿＿（同上） |
| eyeRightX | JX PDI-1109MG（同上） | 同上 | 同上 | ＿＿（同上） |
| eyeRightY | JX PDI-1109MG（同上） | 同上 | 同上 | ＿＿（同上） |
| eyelidLeftUpper | JX PDI-1109MG（同上） | 同上 | 同上 | ＿＿（同上） |
| eyelidLeftLower | JX PDI-1109MG（同上） | 同上 | 同上 | ＿＿（同上） |
| eyelidRightUpper | JX PDI-1109MG（同上） | 同上 | 同上 | ＿＿（同上） |
| eyelidRightLower | JX PDI-1109MG（同上） | 同上 | 同上 | ＿＿（同上） |

**参考（在庫MG90S）**：MG90Sは金属ギアで「安価なSG90」そのものではないが、公式指定はJX PDI-1109MGである。MG90Sの5V時推定トルクは約1.84kg·cm（要求上限比約74%、`docs/reference/servo_datasheets.md` §2）で、JX PDI-1109MG（約90%）より余裕が小さい。**在庫MG90S×4本では8軸に対し4本不足する。**

**瞼へのSG92R使用について（警告あり）**：外部の部品表（「蓮冥頭部 開発手順書 Version 2.0」）は瞼へSG92Rの充当を示しているが、SG92RはTowerPro自身が「New version of SG90」と説明する製品であり、InMoov公式の「avoid using cheap SG90」という警告が直接該当し得る。瞼はまぶたの開閉を頻繁に繰り返す高デューティ軸であり、寿命リスクは軽視できない。**SG92R使用の可否は未確定とし、`docs/OPEN_ISSUES.md` #33（影響度：中）へ登録する。**

**要検討事項**：5.0V時のJX PDI-1109MG推定トルク（約2.25kg·cm、補間値）は要求上限（2.5kg·cm）の約90%であり、比較的余裕は小さい。目・瞼は皮膚アンカー荷重が無い低負荷軸のため実用上は許容範囲と考えられるが、**実測値ではなく補間値である点に注意し、L3で実測トルクを確認すること。**発注数量は8軸分（在庫MG90S×4を使う場合でも追加4本、公式指定JX PDI-1109MGへ統一する場合は8本）を確保すること（`docs/OPEN_ISSUES.md` #31）。

### 2.2 顎（jaw）

| 候補型番 | 5V時トルク | 要求トルク帯（中型10〜20kg·cm）との比較 |
|---|---|---|
| MG996R（安価案） | 約9.67kg·cm（4.8V/6.0V公式値間の線形補間、未実測。`docs/reference/servo_datasheets.md` §2） | **要求下限10kg·cmを下回る。**6V公称値（11kg·cm）では枠内に見えるが、レールBの実運用電圧5.0Vでは要求を満たさない可能性がある |
| DS3218MG（余裕案） | 19kg·cm（5.0V、当該リスティング直接記載。`docs/reference/servo_datasheets.md` §2） | 要求帯（10〜20kg·cm）の上限に近く、5.0V動作でも十分な余裕がある |

**選定の目安**：レールBが5.0V固定である以上、5.0V時のトルクが未確認・補間値のMG996Rより、5.0V時トルクが直接データシートに記載されているDS3218MGの方が選定リスクは低い。**最終決定はL3で顎モジュールの実荷重（人工歯・舌パドル等を含まない現行仕様）を確定してから行う。** 本書時点では両案を候補として残す。

### 2.3 首（neckRotate／neckUpDown）

| 候補型番 | 5V時トルク | 判定 |
|---|---|---|
| HS-805BB ×2（InMoov公式指定） | 約20.6kg·cm（4.8V/6.0V公式値間の線形補間、未実測。`docs/reference/servo_datasheets.md` §2） | 選定条件「5.0V時に20kg·cm以上」を満たす（補間値ベース、僅差） |

**決定1により発注保留を解除する。** L系は公式InMoov部品を100%スケールで作る学習機であり、形状・質量が公式と同一になるため、公式指定のHS-805BB×2（24.7kg·cm@6V）が選定根拠として成立する。頭部質量の実測（`docs/OPEN_ISSUES.md` へ登録）を待つ必要はない。

ただし上表の5.0V時トルク約20.6kg·cmは**線形補間値であり実測ではない**。判定が僅差（要求20kg·cm以上に対し約20.6kg·cm）であるため、**発注前に可能であれば実機トルク計測、または少なくとも実運用電圧5.0Vでのメーカー確認を推奨する。**

| 項目 | 内容 |
|---|---|
| トルク余裕率 | ＿＿（L3で頭部質量・重心実測、首の回転軸から重心までの距離を確定後に記入。`docs/OPEN_ISSUES.md` の「頭部質量・重心の実測」を参照） |
| ホルダ整合 | ＿＿（NeckServoHolderV2 / SkullServoFixV5の実寸とサーボ外形の照合をL3で行う） |

## 3. 首サーボ保留解除の記録（決定1）

旧文書（`docs/archive/2026-08-03_servo_comparison_hphase.md` §6）は、将来の咀嚼顎の質量ダミー（`FutureJaw_MassEnvelope_v0`）確定まで首サーボを発注しないとしていた。この保留理由の記述は本書には引き継がない。

**解除の根拠**：
- 咀嚼機構はM系（2034年以降）であり、搭載先はR系独自頭部である
- L機はL5で凍結される学習機であり、咀嚼顎を載せない
- したがって、L機の首サーボ選定を咀嚼顎質量確定でブロックする理由が無い

詳細な意思決定の経緯は `docs/decisions/2026-08-03_lrsm_migration.md` を参照。

## 4. 顎の角度リミット

| 項目 | 値 | 根拠 |
|---|---|---|
| 試験手順 | 閉口 → 5° → 10° → **15°** | InMoov公式注記に基づく段階的試験 |
| ハードコード上限 | **20°** | InMoov公式注記（超過で破損リスク） |
| 同時駆動 | 首との同時駆動を禁止 | レールB電流予算（`docs/S/power_tree.md` §6.1） |

**試験は15°まで。上限20°はソフトウェア側の絶対リミット。**両者を混同しないこと。機械ストッパーとソフトウェア制限の両方を働かせる。

## 5. 未決事項

- [ ] jaw候補（MG996R／DS3218MG）の最終決定。§2.2の通り5.0V時トルクの余裕を優先するならDS3218MGが有力
- [ ] neckRotate／neckUpDownの5V時トルク実測（発注前に可能であれば）
- [ ] 頭部質量・重心の実測（`docs/OPEN_ISSUES.md` 登録）
- [ ] NeckServoHolderV2 / SkullServoFixV5の実寸とサーボ外形の照合
- [ ] 目・瞼8軸（JX PDI-1109MG候補）の5V時トルク実測（補間値からの乖離確認）
- [ ] 目サーボ4本の追加調達（在庫MG90S×4では8軸に不足。`docs/OPEN_ISSUES.md` #31）
- [ ] 左右独立サーボのソフトウェア同期の実装（`docs/OPEN_ISSUES.md` #32）
- [ ] 瞼へのSG92R使用可否の判断（公式がSG90級を非推奨。`docs/OPEN_ISSUES.md` #33）

---

## 変更履歴

- **v1 2026-08-04** 旧H-Phase体系文書からL2軸割当のみを分離して新規作成。決定1に従い首サーボの発注保留を解除。5.0V時トルクの評価によりjaw候補の再検討事項を明記。
- **v1.1 2026-08-04** `inmoov.fr/eyes-i2/` の一次情報により目の軸数の誤りを発見・修正。対象軸を7軸→11軸へ拡張し、公式指定JX PDI-1109MGを候補として追加。在庫MG90S×4本では8軸に不足する旨と、瞼へのSG92R使用に対する公式警告（SG90級非推奨）を明記した。経緯は `docs/decisions/2026-08-04_eye_axis_count_correction.md` を参照。
- **v1.2 2026-08-06** `docs/L/servo_assignment_L2.md` から `docs/L/servo_assignment_L.md` へリネーム。中身がL2〜L5相当のL系全体の設計表であり、フェーズ番号を名前に含めると工程範囲の誤読を招くため、命名規則に従いフェーズ番号を外した。リポジトリ全体の相互参照リンクを追従させた。

## 参照一覧

| 資料 | URL | 取得日 |
|---|---|---|
| InMoov Default Hardware Map（Neck/RotHead: HS-805BB×2 or PDI-HV2060MG、Jaw: HK15298B or PDI-6221MG(180°)の記載を確認） | https://inmoov.fr/default-hardware-map/ | 2026-08-04 |
| InMoov Neck and Jaw | https://inmoov.fr/neck-and-jaw/ | 2026-08-04 |
| InMoov Head i2 | https://inmoov.fr/headi2/ | 2026-08-04 |
| InMoov Eyes i2（目の軸数8ch・JX PDI-1109MG指定・SG90級非推奨の警告の根拠） | https://inmoov.fr/eyes-i2/ | 2026-08-04 |
| `docs/decisions/2026-08-04_eye_axis_count_correction.md` | ローカルリポジトリ内 | 2026-08-04 |
| `docs/reference/servo_datasheets.md` | ローカルリポジトリ内 | 2026-08-04 |
| `docs/decisions/2026-08-03_lrsm_migration.md` | ローカルリポジトリ内 | 2026-08-04 |
| 旧文書 `docs/archive/2026-08-03_servo_comparison_hphase.md` | ローカルリポジトリ内 | 2026-08-03 |
