# docs

版数: v1
改訂日: 2026-08-04
準拠文書: 器創造計画 2026-2033（改訂基準日 2026-08-03）
系統タグ: 全系統共通（メタ文書）
変更履歴:
- 2026-08-04 新規作成（旧H-Phase体系からL/R/S/M体系への移行に伴う）

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
  README.md                        本ファイル
  decisions/                       意思決定の記録
  archive/                         旧H-Phase体系の退避文書（参照専用、編集禁止）
  L/                                L系（InMoov学習機、非商用）
    servo_assignment_L2.md
    channel_map_L2.md
    failure_log/
      scale_94.md
  R/                                R系（独自商用頭部、2027年〜）
    reserved_axes.md
  S/                                S系（安全・制御・知覚）
    power_tree.md
    bench_axes_S3S4.md
  reference/                        系統非依存の仕様表
    servo_datasheets.md
  OPEN_ISSUES.md                    未確認事項一覧
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

## 参照一覧

- 器創造計画 2026-2033（改訂基準日 2026-08-03）
- 蓮冥頭部 H-Phase 0-24 改訂手順書（2026-08-03改訂、旧体系） 取得日: 2026-08-03
