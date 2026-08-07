# cad

## 用途

3MF・gcode・STL の保存先。

## 命名規則

`L2_部品名_100pct_YYYY-MM-DD.3mf` の形式とする。

## gcodeを保存する理由

ファイル末尾にスライサー設定（倍率・層高・Horizontal Expansion・材料）がテキストで残るため、印刷済み部品のスケール証拠として使える。Cura系は `;SETTING_3` ブロック、Orca／Prusa系は末尾の設定ブロックに記録される。

## LFS対象

`.stl` / `.3mf` / `.gcode`
