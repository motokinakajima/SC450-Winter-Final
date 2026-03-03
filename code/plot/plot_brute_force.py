#!/usr/bin/env python3
"""
Brute-Force Verification Plots
================================
Reads ../results/brute_force.csv and produces three figures comparing
greedy / beam B=20 against the true optimal (brute-force) solution.
"""

import pathlib, sys
import numpy as np
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

ROOT   = pathlib.Path(__file__).resolve().parent.parent
CSV    = ROOT / "results" / "brute_force.csv"
OUTDIR = pathlib.Path(__file__).resolve().parent

if not CSV.exists():
    sys.exit(f"CSV not found: {CSV}")

df = pd.read_csv(CSV)

matplotlib.rcParams.update({
    "pgf.texsystem": "pdflatex",
    "pgf.preamble": "\\usepackage{amsmath}",
})
plt.rcParams.update({
    "font.family": "serif",
    "font.size": 11,
    "axes.grid": True,
    "grid.alpha": 0.3,
    "savefig.bbox": "tight",
    "savefig.dpi": 300,
})

def save(fig, name):
    """Save as both PDF and PGF."""
    fig.savefig(OUTDIR / f"{name}.pdf")
    fig.savefig(OUTDIR / f"{name}.pgf")
    plt.close(fig)
    print(f"✓ {name}.pdf  +  {name}.pgf")

n_vals = sorted(df["n"].unique())

# ════════════════════════════════════════════════════════
# Plot 1 — Optimality Ratio Histogram  (side-by-side n)
# ════════════════════════════════════════════════════════
fig, axes = plt.subplots(1, len(n_vals), figsize=(10, 4), sharey=True)
bins = np.arange(60, 101, 2)

for ax, n in zip(axes, n_vals):
    sub = df[df["n"] == n]
    ax.hist(sub["greedy/opt%"], bins=bins, alpha=0.55, color="#d62728",
            edgecolor="white", label="Greedy")
    ax.hist(sub["beam20/opt%"], bins=bins, alpha=0.55, color="#1f77b4",
            edgecolor="white", label="Beam $B\\!=\\!20$")
    ax.axvline(100, ls="--", color="black", lw=0.8, alpha=0.5)
    ax.set_xlabel("Score / Optimal (%)")
    ax.set_title(f"$n = {n}$")
    ax.legend(fontsize=9, framealpha=0.8)

axes[0].set_ylabel("Count (out of 50 trials)")
fig.suptitle("Optimality Ratio Distribution", fontsize=13, y=1.02)
save(fig, "bf_histogram")

# ════════════════════════════════════════════════════════
# Plot 2 — Scatter: Greedy / Beam vs Optimal
# ════════════════════════════════════════════════════════
fig, axes = plt.subplots(1, len(n_vals), figsize=(10, 4.5))

for ax, n in zip(axes, n_vals):
    sub = df[df["n"] == n]
    lo = min(sub["greedy"].min(), sub["optimal"].min()) * 0.9
    hi = sub["optimal"].max() * 1.05

    ax.scatter(sub["optimal"], sub["greedy"], s=30, alpha=0.6,
               color="#d62728", marker="s", label="Greedy", zorder=3)
    ax.scatter(sub["optimal"], sub["beam20"], s=30, alpha=0.6,
               color="#1f77b4", marker="o", label="Beam $B\\!=\\!20$", zorder=3)
    ax.plot([lo, hi], [lo, hi], "k--", lw=0.8, alpha=0.5, label="$y = x$ (optimal)")
    ax.set_xlabel("Optimal Score")
    ax.set_ylabel("Heuristic Score")
    ax.set_title(f"$n = {n}$")
    ax.set_xlim(lo, hi)
    ax.set_ylim(lo, hi)
    ax.set_aspect("equal")
    ax.legend(fontsize=8, loc="upper left", framealpha=0.8)

fig.suptitle("Heuristic vs True Optimal Score", fontsize=13, y=1.02)
fig.tight_layout()
save(fig, "bf_scatter")

# ════════════════════════════════════════════════════════
# Plot 3 — Sorted Optimality Profile
# ════════════════════════════════════════════════════════
fig, axes = plt.subplots(1, len(n_vals), figsize=(10, 4), sharey=True)

for ax, n in zip(axes, n_vals):
    sub = df[df["n"] == n].copy()

    # sort by beam optimality (descending) for a clean curve
    greedy_sorted = np.sort(sub["greedy/opt%"].values)[::-1]
    beam_sorted   = np.sort(sub["beam20/opt%"].values)[::-1]
    x = np.arange(1, len(sub) + 1)

    ax.fill_between(x, beam_sorted, greedy_sorted, alpha=0.15, color="#1f77b4")
    ax.plot(x, beam_sorted, "o-", ms=3, lw=1.2, color="#1f77b4",
            label="Beam $B\\!=\\!20$")
    ax.plot(x, greedy_sorted, "s-", ms=3, lw=1.2, color="#d62728",
            label="Greedy")
    ax.axhline(100, ls="--", color="black", lw=0.8, alpha=0.5)
    ax.set_xlabel("Trial rank (sorted)")
    ax.set_title(f"$n = {n}$")
    ax.legend(fontsize=9, framealpha=0.8)
    ax.yaxis.set_major_formatter(mtick.PercentFormatter(decimals=0))

axes[0].set_ylabel("Score / Optimal (%)")
fig.suptitle("Sorted Optimality Profile", fontsize=13, y=1.02)
save(fig, "bf_profile")

print(f"\nAll plots saved to {OUTDIR}")
