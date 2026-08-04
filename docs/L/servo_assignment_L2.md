# L2 InMoov頭部 軸・サーボ割当

版数: v1
改訂日: 2026-08-04
準拠文書: 器創造計画 2026-2033（改訂基準日 2026-08-03）
系統タグ: L（InMoov 100%学習機・非商用。L5で凍結）
変更履歴:
- v1 2026-08-04 旧H-Phase体系文書 `docs/archive/2026-08-03_servo_comparison_hphase.md` から、L2 InMoov頭部の軸割当のみを分離して新規作成。
  - **決定1に従い、首サーボ（neckRotate／neckUpDown）の発注保留を解除した。** 旧文書は将来の咀嚼顎質量（`FutureJaw_MassEnvelope_v0`）確定まで首サーボを確定しないとしていたが、咀嚼機構はM系（2034年以降）であり搭載先はR系独自頭部である。L機はL5で凍結される学習機であり咀嚼顎を載せないため、L機の首サーボ選定を咀嚼顎質量確定でブロックする根拠が無い。解除の詳細な経緯・知財上の根拠は `docs/decisions/2026-08-03_lrsm_migration.md` を参照。
  - R系軸（upperLip／cheek×2／eyebrow×2／forhead×2、決定2）とS系軸（throatGate／tasteSensorLift）はここに含めない。R系は `docs/R/reserved_axes.md`、S系は `docs/S/bench_axes_S3S4.md` を参照。

---

## 0. 非商用条件と出典

L2は **InMoov公式頭部（i2Head）の設計に100%準拠**して製作する。InMoovの公式設計は **Attribution-NonCommercial** ライセンス（出典: inmoov.fr）で公開されており、本書が扱う軸・寸法・サーボ選定条件は非商用の学習・研究目的に限る。

L系はL5到達をもって凍結し、以降の開発を行わない。L系の成果物（形状・寸法・STL・メッシュ・本書の軸割当）をR系（独自商用頭部）へ転用しない。知財上の根拠の詳細は `docs/decisions/2026-08-03_lrsm_migration.md` を参照。

参考: inmoov.fr/default-hardware-map ／ inmoov.fr/neck-and-jaw ／ inmoov.fr/headi2

## 1. 対象軸（公式InMoov頭部のみ、計7軸）

L2の対象は公式InMoov頭部の軸のみとする。表情軸（R系、2027年R5-R9）と味覚・喉ゲート軸（S系ベンチ）はここに含めない。

| 軸名 | 要求トルク帯 | 備考 |
|---|---|---|
| eyeX | micro級（〜2.5kg·cm） | 両目左右共通軸（i2Eyesは左右連動） |
| eyeY | micro級（〜2.5kg·cm） | 両目上下共通軸 |
| eyelidLeft | micro級（〜2.5kg·cm） | 皮膚アンカー荷重なし |
| eyelidRight | micro級（〜2.5kg·cm） | 皮膚アンカー荷重なし |
| jaw | 中型（10〜20kg·cm） | 試験角度15°／ハードコード上限20°。首と同時駆動禁止 |
| neckRotate | 大型 | InMoov公式指定 HS-805BB ×2 |
| neckUpDown | 大型 | InMoov公式指定 HS-805BB ×2 |

## 2. 候補型番と5V時トルク（レールB=5.0V前提）

**選定条件**：レールBは5.0V固定のため、6V公称値ではなく **5.0V時のトルクで評価する**。5.0V時の値が公式データシートに無い機種は「未確認」とし、線形補間する場合はその旨を明記する。数値の出典は `docs/reference/servo_datasheets.md` を参照。

### 2.1 目・瞼（eyeX／eyeY／eyelidLeft／eyelidRight）

| 軸 | 候補型番 | 5V時トルク | 要求トルク帯との比較 | トルク余裕率（5V時トルク÷要求トルク上限） |
|---|---|---|---|---|
| eyeX | MG90S（到着待ち4個を充当） | 約1.84kg·cm（4.8V/6.6V公式値間の線形補間、未実測。`docs/reference/servo_datasheets.md` §2） | 要求上限2.5kg·cmに対し約74% | ＿＿（L3で頭部・可動部の実荷重確定後に記入） |
| eyeY | MG90S（同上） | 同上 | 同上 | ＿＿（同上） |
| eyelidLeft | MG90S（同上） | 同上 | 同上 | ＿＿（同上） |
| eyelidRight | MG90S（同上） | 同上 | 同上 | ＿＿（同上） |

**要検討事項**：5.0V時のMG90S推定トルク（約1.84kg·cm、補間値）は要求上限（2.5kg·cm）の約74%であり、公称6.6V値（2.2kg·cm）を前提に選定した場合より余裕が小さい。目・瞼は皮膚アンカー荷重が無い低負荷軸のため実用上は許容範囲と考えられるが、**実測値ではなく補間値である点に注意し、L3で実測トルクを確認すること。**

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
- [ ] eyeX/eyeY/eyelid×2のMG90S、5V時トルクの実測（補間値からの乖離確認）

---

## 変更履歴

- **v1 2026-08-04** 旧H-Phase体系文書からL2軸割当のみを分離して新規作成。決定1に従い首サーボの発注保留を解除。5.0V時トルクの評価によりjaw候補の再検討事項を明記。

## 参照一覧

| 資料 | URL | 取得日 |
|---|---|---|
| InMoov Default Hardware Map（Neck/RotHead: HS-805BB×2 or PDI-HV2060MG、Jaw: HK15298B or PDI-6221MG(180°)の記載を確認） | https://inmoov.fr/default-hardware-map/ | 2026-08-04 |
| InMoov Neck and Jaw | https://inmoov.fr/neck-and-jaw/ | 2026-08-04 |
| InMoov Head i2 | https://inmoov.fr/headi2/ | 2026-08-04 |
| `docs/reference/servo_datasheets.md` | ローカルリポジトリ内 | 2026-08-04 |
| `docs/decisions/2026-08-03_lrsm_migration.md` | ローカルリポジトリ内 | 2026-08-04 |
| 旧文書 `docs/archive/2026-08-03_servo_comparison_hphase.md` | ローカルリポジトリ内 | 2026-08-03 |
