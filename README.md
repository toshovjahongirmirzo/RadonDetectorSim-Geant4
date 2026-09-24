# Hybrid PIPS/CR-39/ZnS(Ag) Radon Detector — Geant4 Validation

Geant4 simulation supporting the manuscript *"A Hybrid PIPS/CR-39/ZnS(Ag)
Detector for Alpha Discrimination in Relativistic Radiation Fields"*
(Ismailov, Toshov, Toshov — CERN Beamline for Schools 2025, team ALIENS).

## What this validates
- Full radon-chain alpha containment in the PIPS active layer
- MIP energy-deposition distributions for pi+, p, K+ at 4.5 and 10 GeV/c, plus e+
- Alpha-mimic interaction probability P_inel and conditional fraction f_mimic|inel
- CR-39 local-LET distribution and physical source attribution (PIPS-mimic vs. CR-39-direct)
- FTFP_BERT vs QGSP_BERT cross-check

## Key results (1,000,000 events per configuration, both physics lists)

| Quantity | Result |
|---|---|
| Alpha events reaching ZnS(Ag) (3 lines, 3,000,000 primaries) | 4 (1 hadronic-flagged, 3 unexplained) |
| P_inelastic, MIP in PIPS (pi/p/K, 4.5-10 GeV/c) | 4.94e-4 - 7.61e-4 |
| P_elastic, MIP in PIPS | 1.36e-4 - 4.88e-4 |
| f_mimic\|inel (LET >= 5 keV/um criterion) | 0.117 - 0.250 |
| CR39-direct : PIPS-mimic ratio (unconditional) | 7.2 - 8.2 : 1 |
| CR39-direct : PIPS-mimic ratio (LET >= 5 keV/um only) | 17.2 - 29.1 : 1 |
| FTFP_BERT vs QGSP_BERT | identical (checksum-verified) at these energies |

Full derivation: `summary_stats/stats_FINAL.txt` and `summary_stats/cr39_ratio_FINAL_v2.txt`.

## Repository layout
- `source_code/` — Geant4 application (`.hh`/`.cc`, `main.cc`, `CMakeLists.txt`) and compiled binary
- `terminal_logs/` — run logs and batch scripts (`run_batch.sh`, `run_batch_qgsp.sh`)
- `summary_stats/` — final verified per-event statistics, derived directly from raw output
- `figures/` — publication figures (alpha lines, alpha/MIP separation, CR-39 LET distribution)
- `checksums.txt` — SHA-256 checksums of the raw per-event CSVs (available on request; not included here due to size, ~1.3 GB total)

## Build and run
```bash
mkdir build && cd build
cmake ..
make -j4
./RadonDetectorSim <particle> <momentum_GeV> <nEvents> <outputStem> [FTFP_BERT|QGSP_BERT]
```

## Scope and limitations
All results use normal-incidence, single-particle, fixed-point-source transport.
No angular distribution, beam divergence, or absolute detection efficiency is
evaluated. These are simulation-based predictions for a planned beam test, not
a completed performance measurement. See the manuscript's Limitations section
for the complete list, including two open items: three of the four alpha
leakage events are not yet mechanistically explained, and results use
Geant4's default random seed (not yet cross-checked against independent seeds).
