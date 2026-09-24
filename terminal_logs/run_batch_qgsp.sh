#!/bin/bash
set -e

jobs=(
  "alpha 5.489 1000000 alpha_5p489_qgsp"
  "alpha 6.00 1000000 alpha_6p00_qgsp"
  "alpha 7.69 1000000 alpha_7p69_qgsp"
  "pi+ 4.5 1000000 mip_pi_4p5_qgsp"
  "proton 4.5 1000000 mip_p_4p5_qgsp"
  "kaon+ 4.5 1000000 mip_K_4p5_qgsp"
  "e+ 4.5 1000000 mip_ep_4p5_qgsp"
  "pi+ 10.0 1000000 mip_pi_10_qgsp"
  "proton 10.0 1000000 mip_p_10_qgsp"
  "kaon+ 10.0 1000000 mip_K_10_qgsp"
)

for job in "${jobs[@]}"; do
  read -r particle momentum nevents stem <<< "$job"
  echo "=== Running: $particle $momentum $nevents $stem (QGSP_BERT) ==="
  START=$(date +%s)
  ./RadonDetectorSim "$particle" "$momentum" "$nevents" "$stem" QGSP_BERT > "${stem}.log" 2>&1
  END=$(date +%s)
  echo "=== Done: $stem (took $((END-START))s) ==="
done

echo "ALL QGSP JOBS COMPLETE"
