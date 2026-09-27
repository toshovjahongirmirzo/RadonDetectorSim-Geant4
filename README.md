# Hybrid PIPS/CR-39/ZnS(Ag) Radon Detector — Geant4 Validation

Geant4 simulation supporting the manuscript *"A Hybrid PIPS/CR-39/ZnS(Ag)
Detector for Alpha Discrimination in Relativistic Radiation Fields"*
(Ismailov, Toshov, Toshov — CERN Beamline for Schools 2025, team ALIENS).

## What this validates
- Full radon-chain alpha containment in the PIPS active layer
- MIP energy-deposition distributions for pi+, p, K+ at 4.5 and 10 GeV/c, plus e+
- Alpha-mimic interaction probability P_inel and conditional fraction f_mimic|inel
- CR-39 local-LET distribution and physical source attribution (PIPS-mimic vs. CR-39-direct)
- FTFP_BERT vs QGSP_BERT cross-check, including an independent-random-seed replication

## Key results (1,000,000 events per configuration)

| Quantity | Result |
|---|---|
| Alpha events reaching ZnS(Ag) (3 lines, 3,000,000 primaries) | 4 (1 hadronic-flagged, 3 unexplained) |
| P_inelastic, MIP in PIPS (pi/p/K, 4.5-10 GeV/c) | 4.94e-4 - 7.61e-4 |
| P_elastic, MIP in PIPS | 1.36e-4 - 4.88e-4 |
| f_mimic given inel (LET >= 5 keV/um criterion) | 0.117 - 0.250 |
| CR39-direct : PIPS-mimic ratio (unconditional) | 7.2 - 8.2 : 1 |
| CR39-direct : PIPS-mimic ratio (LET >= 5 keV/um only) | 17.2 - 29.1 : 1 |
| FTFP_BERT vs QGSP_BERT, shared default seed | bit-identical for all 10 configs |
| FTFP_BERT vs QGSP_BERT, independent seeds | statistically consistent, |z| < 1.7 for all configs |

Full derivation: summary_stats/stats_FINAL.txt and summary_stats/cr39_ratio_FINAL_v2.txt.

## Physics-list cross-check, independent-seed replication

The original FTFP_BERT vs QGSP_BERT comparison used Geant4's shared default
seed and found bit-identical per-event output for all ten configurations.
That is expected, not surprising: both physics lists route hadronic
interactions through the same underlying models (BertiniCascade transitioning
to FTF) below their mutual divergence from QGS at 12-25 GeV, and no
configuration in this study exceeds 10 GeV/c, so an identical seed guarantees
an identical code path.

To get a genuine independent check, all ten configurations were rerun under
both physics lists with freshly-drawn, logged 32-bit seeds:
- terminal_logs/seeds_used_ftfp.txt
- terminal_logs/seeds_used_qgsp.txt
- summary_stats/stats_seeded_comparison.txt

P_inel and P_elastic agreed within Poisson counting uncertainty for every
configuration under independent seeds (|z| < 1.7 throughout, no consistent
directional bias between the two physics lists). The run-to-run spread across
independent seeds (e.g. proton, 4.5 GeV/c: P_inel = 7.61, 7.30, 7.05 x1e-4
across three seeds) gives an empirical Monte Carlo statistical uncertainty of
roughly 4-8% on these rates at 1e6 events per configuration.

## Repository layout
- source_code/ — Geant4 application (.hh/.cc, main.cc, CMakeLists.txt) and compiled binary
- terminal_logs/ — run logs, per-run random seeds, and batch scripts
- summary_stats/ — final verified per-event statistics, derived directly from raw output
- figures/ — publication figures (alpha lines, alpha/MIP separation, CR-39 LET distribution)
- checksums.txt — SHA-256 checksums of the raw per-event CSVs

Raw per-event CSV output (~2 GB total including the seeded reruns) is
available from the corresponding authors on request; checksums.txt lets you
verify integrity against what is reported here.

## Build and run
mkdir build && cd build
cmake ..
make -j4
./RadonDetectorSim particle momentum_GeV nEvents outputStem FTFP_BERT_or_QGSP_BERT seed

If seed is omitted, one is drawn from the system clock automatically.

## Scope and limitations
All results use normal-incidence, single-particle, fixed-point-source
transport. No angular distribution, beam divergence, or absolute detection
efficiency is evaluated. These are simulation-based predictions for a planned
beam test, not a completed performance measurement. See the manuscript's
Limitations section for the complete list, including: three of the four alpha
leakage events are not yet mechanistically explained, and the independent-seed
cross-check above used one additional replicate per configuration rather than
a full multi-replicate Monte Carlo uncertainty budget.
