"""Raw-data figures for the PIPS/CR-39/ZnS(Ag) manuscript (Sec. 4.6).

Run this on the machine that holds the raw per-event Geant4 CSVs
(the ones listed in checksums.txt under raw_data/).

    python3 make_geant4_figs.py --data /path/to/raw_data --out figs/

Produces:
  fig_pips_edep.pdf   PIPS active-layer energy deposition, alpha vs MIP, log-E axis (+ linear alpha zoom)
  fig_cr39_let.pdf    CR-39 maximum LET by physical source, with the 5 keV/um threshold

It prints every number it plots (event counts, zero-deposit fractions, peak
positions, overlap) so captions can be checked against the data.
"""
import argparse, os, numpy as np, pandas as pd
import matplotlib; matplotlib.use("Agg")
import matplotlib.pyplot as plt

ap = argparse.ArgumentParser()
ap.add_argument("--data", required=True); ap.add_argument("--out", default="figs")
a = ap.parse_args(); os.makedirs(a.out, exist_ok=True)
plt.rcParams.update({"font.size": 10, "font.family": "serif", "axes.grid": True, "grid.alpha": .25})

def load(stem, cols):
    fn = os.path.join(a.data, f"{stem}_nt_edep.csv")
    return pd.read_csv(fn, comment="#", header=None, names=[
        "EventID","EntranceWindow_MeV","PIPS_DeadLayer_MeV","PIPS_Active_MeV","ZnSAg_MeV","LightGuide_MeV",
        "CR39_MeV","InelasticInPIPS","ElasticInPIPS","ZnSAnyHit","CR39AnyHit","CR39LETPass",
        "CR39_MaxLET_keVperUm","CR39_MaxLET_source"], usecols=cols)

alphas = [("alpha_5p489_big", r"$\alpha$ 5.489 MeV", "k"), ("alpha_6p00_big", r"$\alpha$ 6.00 MeV", "0.35"),
          ("alpha_7p69_big", r"$\alpha$ 7.69 MeV", "0.6")]
mips = [("mip_pi_4p5_big", r"$\pi^+$ 4.5 GeV/$c$", "C0"), ("mip_p_4p5_big", r"$p$ 4.5 GeV/$c$", "C1"),
        ("mip_K_4p5_big", r"$K^+$ 4.5 GeV/$c$", "C2"), ("mip_pi_10_big", r"$\pi^+$ 10 GeV/$c$", "C3"),
        ("mip_p_10_big", r"$p$ 10 GeV/$c$", "C4"), ("mip_K_10_big", r"$K^+$ 10 GeV/$c$", "C5"),
        ("mip_ep_4p5_big", r"$e^+$ 4.5 GeV/$c$", "C6")]

# ---------------- Fig 1: PIPS energy deposition ----------------
bins = np.logspace(np.log10(1e-3), np.log10(12), 220)       # uniform in ln E
fig, (ax, ax2) = plt.subplots(1, 2, figsize=(10, 3.9), gridspec_kw={"width_ratios": [1.7, 1]})
amax_lo, mip_hi = None, 0.0
for stem, lab, col in alphas + mips:
    e = load(stem, ["PIPS_Active_MeV"])["PIPS_Active_MeV"].to_numpy()
    nz = e[e > 0]; n0 = e.size - nz.size
    h, _ = np.histogram(nz, bins)
    ax.step(bins[:-1], h, where="post", color=col, lw=1.2, label=lab, ls="-" if stem.startswith("alpha") else "-")
    print(f"{stem:18s} N={e.size} zero-deposit={n0/e.size:.4f}  median(nonzero)={np.median(nz):.4f} MeV  max={nz.max():.3f} MeV")
    if stem.startswith("alpha"):
        amax_lo = nz.min() if amax_lo is None else min(amax_lo, nz.min())
        pk = nz[(nz > 0.8*nz.max()*0 + np.percentile(nz, 50)*0.5)]
    else:
        mip_hi = max(mip_hi, nz.max())
print(f"lowest non-zero alpha deposit = {amax_lo:.3f} MeV ; highest MIP deposit = {mip_hi:.3f} MeV")
ax.set_xscale("log"); ax.set_yscale("log")
ax.set_xlabel(r"Energy deposited in PIPS active layer, $E$ (MeV)  [log axis $\equiv \ln E$]")
ax.set_ylabel(r"Events per bin ($\Delta\ln E$ = const.)")
ax.set_ylim(0.7, None); ax.legend(fontsize=7, ncol=2, frameon=False, loc="upper left")
ax.set_title("(a)", loc="left", fontsize=10)
lin = np.linspace(4.0, 8.2, 420)
for stem, lab, col in alphas:
    e = load(stem, ["PIPS_Active_MeV"])["PIPS_Active_MeV"].to_numpy()
    ax2.hist(e[(e > 4) & (e < 8.2)], lin, histtype="step", color=col, label=lab)
    sel = e[(e > 0.85*np.median(e[e > 1])) & (e < 1.1*np.median(e[e > 1]))]
    print(f"{stem}: peak mean={sel.mean():.3f} MeV  sigma={sel.std():.4f} MeV")
ax2.set_yscale("log"); ax2.set_xlabel("Energy deposited in PIPS active layer (MeV)"); ax2.set_ylabel("Events per bin")
ax2.legend(fontsize=7, frameon=False); ax2.set_title("(b)", loc="left", fontsize=10)
fig.tight_layout(); fig.savefig(os.path.join(a.out, "fig_pips_edep.pdf")); plt.close(fig)

# ---------------- Fig 2: CR-39 LET by source ----------------
fig, axs = plt.subplots(1, 2, figsize=(10, 3.7), sharey=True)
lb = np.logspace(-2, 3, 120)
groups = [("Pions/kaons/protons, 4.5 GeV/$c$", ["mip_pi_4p5_big", "mip_p_4p5_big", "mip_K_4p5_big"]),
          ("Pions/kaons/protons, 10 GeV/$c$",  ["mip_pi_10_big", "mip_p_10_big", "mip_K_10_big"])]
for ax, (title, stems) in zip(axs, groups):
    d = pd.concat([load(s, ["CR39_MaxLET_keVperUm", "CR39_MaxLET_source"]) for s in stems])
    d = d[d.CR39_MaxLET_keVperUm > 0]
    for src, lab, col in [(0, "Primary / other", "0.55"), (1, "PIPS-produced secondary", "C3"), (2, "CR-39-direct interaction", "C0")]:
        v = d[d.CR39_MaxLET_source == src].CR39_MaxLET_keVperUm.to_numpy()
        ax.hist(v, lb, histtype="step", color=col, lw=1.3, label=f"{lab} (n={v.size})")
        print(f"{title} | {lab}: n={v.size}  frac>=5keV/um={np.mean(v>=5) if v.size else float('nan'):.3f}")
    ax.axvline(5.0, color="k", ls="--", lw=1); ax.text(5.4, 0.55, r"$\mathrm{LET_{th}}=5$ keV/$\mu$m", rotation=90, va="bottom", fontsize=8, transform=ax.get_xaxis_transform())
    ax.set_xscale("log"); ax.set_yscale("log"); ax.set_xlabel(r"Maximum step LET in CR-39 (keV/$\mu$m)")
    ax.set_title(title, fontsize=9); ax.legend(fontsize=7, frameon=False, loc="upper right")
axs[0].set_ylabel("Events per bin")
fig.tight_layout(); fig.savefig(os.path.join(a.out, "fig_cr39_let.pdf")); plt.close(fig)
print("done")
