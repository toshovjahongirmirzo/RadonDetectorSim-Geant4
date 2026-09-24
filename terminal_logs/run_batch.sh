#!/bin/bash
set -e

jobs=(
  "alpha 5.489 1000000 alpha_5p489_big"
  "alpha 6.00 1000000 alpha_6p00_big"
  "alpha 7.69 1000000 alpha_7p69_big"
  "pi+ 4.5 1000000 mip_pi_4p5_big"
  "proton 4.5 1000000 mip_p_4p5_big"
  "kaon+ 4.5 1000000 mip_K_4p5_big"
  "e+ 4.5 1000000 mip_ep_4p5_big"
  "pi+ 10.0 1000000 mip_pi_10_big"
  "proton 10.0 1000000 mip_p_10_big"
  "kaon+ 10.0 1000000 mip_K_10_big"
)

for job in "${jobs[@]}"; do
  read -r particle momentum nevents stem <<< "$job"
  echo "=== Running: $particle $momentum $nevents $stem ==="
  START=$(date +%s)
  ./RadonDetectorSim "$particle" "$momentum" "$nevents" "$stem" > "${stem}.log" 2>&1
  END=$(date +%s)
  echo "=== Done: $stem (took $((END-START))s) ==="
done

echo "ALL JOBS COMPLETE"
