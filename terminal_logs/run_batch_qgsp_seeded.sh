#!/bin/bash
set -e
random_seed() { od -An -N4 -tu4 < /dev/urandom | tr -d ' '; }
jobs=(
  "alpha 5.489 1000000 alpha_5p489_qgsp_seeded"
  "alpha 6.00 1000000 alpha_6p00_qgsp_seeded"
  "alpha 7.69 1000000 alpha_7p69_qgsp_seeded"
  "pi+ 4.5 1000000 mip_pi_4p5_qgsp_seeded"
  "proton 4.5 1000000 mip_p_4p5_qgsp_seeded"
  "kaon+ 4.5 1000000 mip_K_4p5_qgsp_seeded"
  "e+ 4.5 1000000 mip_ep_4p5_qgsp_seeded"
  "pi+ 10.0 1000000 mip_pi_10_qgsp_seeded"
  "proton 10.0 1000000 mip_p_10_qgsp_seeded"
  "kaon+ 10.0 1000000 mip_K_10_qgsp_seeded"
)
: > seeds_used_qgsp.txt
for job in "${jobs[@]}"; do
  read -r particle momentum nevents stem <<< "$job"
  SEED=$(random_seed)
  echo "$stem seed=$SEED" >> seeds_used_qgsp.txt
  echo "=== $particle $momentum $stem QGSP_BERT seed=$SEED ==="
  ./RadonDetectorSim "$particle" "$momentum" "$nevents" "$stem" QGSP_BERT "$SEED" > "${stem}.log" 2>&1
done
echo "QGSP-SEEDED DONE"
