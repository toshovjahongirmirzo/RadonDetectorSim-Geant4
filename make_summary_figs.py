"""Figures built ONLY from the published summary statistics in the repo
(summary_stats/stats_FINAL.txt, stats_seeded_comparison.txt, cr39_ratio_FINAL_v2.txt).
No simulated or invented numbers."""
import re, numpy as np, matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
R = "/Users/uzmacbook/Desktop/Geant4Results/summary_stats/"
N = 1_000_000
plt.rcParams.update({"font.size": 10, "font.family": "serif", "axes.grid": True, "grid.alpha": .25})

def parse(fn):
    d = {}
    for line in open(R + fn):
        t = line.split()
        if not t: continue
        d[t[0]] = {k: float(v) for k, v in re.findall(r"(\w+(?:\|\w+)?)=([-\d.eE+]+)", line)}
    return d
seeded = parse("stats_seeded_comparison.txt")
final = parse("stats_FINAL.txt")
cfg = [("pi_4p5", r"$\pi^+$ 4.5"), ("p_4p5", r"$p$ 4.5"), ("K_4p5", r"$K^+$ 4.5"),
       ("pi_10", r"$\pi^+$ 10"), ("p_10", r"$p$ 10"), ("K_10", r"$K^+$ 10")]
x = np.arange(len(cfg)); w = 0.27

# ---- Fig A: P_inel per configuration, 3 independent random-number states
fig, ax = plt.subplots(figsize=(6.6, 3.5))
series = [("Default seed (both lists)", [final[f"mip_{c}_big"]["P_inel"] for c, _ in cfg], "0.55"),
          ("FTFP_BERT, independent seed", [seeded[f"mip_{c}_ftfp"]["P_inel"] for c, _ in cfg], "C0"),
          ("QGSP_BERT, independent seed", [seeded[f"mip_{c}_qgsp"]["P_inel"] for c, _ in cfg], "C3")]
for i, (lab, v, col) in enumerate(series):
    v = np.array(v); err = np.sqrt(v * N) / N
    ax.bar(x + (i - 1) * w, v * 1e4, w, yerr=err * 1e4, capsize=2, label=lab, color=col)
ax.set_xticks(x); ax.set_xticklabels([l for _, l in cfg])
ax.set_xlabel(r"Species and momentum (GeV/$c$)")
ax.set_ylabel(r"$P_{\rm inel}$ in PIPS active layer ($10^{-4}$)")
ax.legend(fontsize=8, frameon=False, loc="upper left", ncol=1); ax.set_ylim(0, 10.5)
fig.tight_layout(); fig.savefig("fig_pinel_seeds.pdf"); plt.close(fig)

# ---- Fig B: f_mimic|inel per configuration
fig, ax = plt.subplots(figsize=(6.6, 3.5))
series = [("Default seed", [final[f"mip_{c}_big"]["f_mimic"] for c, _ in cfg], [final[f"mip_{c}_big"]["P_inel"] for c, _ in cfg], "0.55"),
          ("FTFP_BERT, indep. seed", [seeded[f"mip_{c}_ftfp"]["f_mimic"] for c, _ in cfg], [seeded[f"mip_{c}_ftfp"]["P_inel"] for c, _ in cfg], "C0"),
          ("QGSP_BERT, indep. seed", [seeded[f"mip_{c}_qgsp"]["f_mimic"] for c, _ in cfg], [seeded[f"mip_{c}_qgsp"]["P_inel"] for c, _ in cfg], "C3")]
for i, (lab, f, p, col) in enumerate(series):
    f = np.array(f); n = np.array(p) * N          # number of inelastic events
    err = np.sqrt(f * (1 - f) / n)                # binomial error on conditional fraction
    ax.bar(x + (i - 1) * w, f, w, yerr=err, capsize=2, label=lab, color=col)
ax.set_xticks(x); ax.set_xticklabels([l for _, l in cfg])
ax.set_xlabel(r"Species and momentum (GeV/$c$)")
ax.set_ylabel(r"$f_{\rm mimic|inel}$"); ax.set_ylim(0, 0.35)
ax.legend(fontsize=8, frameon=False, loc="upper left")
fig.tight_layout(); fig.savefig("fig_fmimic.pdf"); plt.close(fig)

# ---- Fig C: CR39-direct : PIPS-mimic ratio (default-seed runs; the only file with LET>=5 ratio)
ratio = parse("cr39_ratio_FINAL_v2.txt")
u = [ratio[f"mip_{c}_big"]["CR39direct:mimic_unconditional"] for c, _ in cfg] if False else None
txt = open(R + "cr39_ratio_FINAL_v2.txt").read()
un, le = [], []
for c, _ in cfg:
    m = re.search(rf"mip_{c}_big\s+CR39direct:mimic_unconditional=([\d.]+)\s+CR39direct:mimic_LETge5=([\d.]+)", txt)
    un.append(float(m.group(1))); le.append(float(m.group(2)))
fig, ax = plt.subplots(figsize=(6.6, 3.4))
ax.bar(x - w/2, un, w, label="All track-producing steps", color="C2")
ax.bar(x + w/2, le, w, label=r"LET $\geq 5$ keV/$\mu$m only", color="C1")
ax.axhline(1, color="k", lw=.8, ls="--")
ax.set_xticks(x); ax.set_xticklabels([l for _, l in cfg])
ax.set_xlabel(r"Species and momentum (GeV/$c$)")
ax.set_ylabel("CR-39-direct : PIPS-mimic ratio"); ax.set_ylim(0, 40); ax.legend(fontsize=8, frameon=False, loc="upper left")
fig.tight_layout(); fig.savefig("fig_cr39_ratio.pdf"); plt.close(fig)
print("un", un); print("le", le)
