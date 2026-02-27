#!/usr/bin/env python3
"""
Harkness Table — Beam Search vs Greedy  Visualization
======================================================
Reads ../results/results.csv and produces four publication-quality plots:

  1. improvement_vs_B.pdf       — Mean % improvement over greedy  vs  B  (one line per n)
  2. improvement_vs_n.pdf       — Mean % improvement over greedy  vs  n  (one line per B)
  3. score_vs_n.pdf             — Mean absolute score  vs  n  (greedy & beam B=10)
  4. heatmap_improvement.pdf    — Heatmap of mean % improvement  (n × B)
"""

import pathlib, sys
import numpy as np
import pandas as pd
import matplotlib
matplotlib.use("Agg")          # non-interactive backend
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

# ── paths ───────────────────────────────────────────
ROOT   = pathlib.Path(__file__).resolve().parent.parent
CSV    = ROOT / "results" / "results.csv"
OUTDIR = pathlib.Path(__file__).resolve().parent   # save PDFs next to script

if not CSV.exists():
    sys.exit(f"CSV not found: {CSV}")

df = pd.read_csv(CSV)
df["improve_pct"] = (df["beam_score"] - df["greedy_score"]) / df["greedy_score"].abs() * 100

# ── colour / style setup ────────────────────────────
plt.rcParams.update({
    "font.family": "serif",
    "font.size": 11,
    "axes.grid": True,
    "grid.alpha": 0.3,
    "savefig.bbox": "tight",
    "savefig.dpi": 300,
})

n_vals = sorted(df["n"].unique())
B_vals = sorted(df["B"].unique())
cmap   = plt.cm.viridis

# ════════════════════════════════════════════════════
# Plot 1 — Improvement vs B  (one line per n)
# ════════════════════════════════════════════════════
fig, ax = plt.subplots(figsize=(7, 4.5))
for i, n in enumerate(n_vals):
    sub = df[df["n"] == n].groupby("B")["improve_pct"].mean()
    colour = cmap(i / max(len(n_vals) - 1, 1))
    ax.plot(sub.index, sub.values, "o-", color=colour, ms=4, lw=1.4, label=f"n={n}")

ax.set_xlabel("Beam Width  $B$")
ax.set_ylabel("Mean Improvement over Greedy (%)")
ax.set_title("Beam Search Improvement vs Beam Width")
ax.yaxis.set_major_formatter(mtick.PercentFormatter(decimals=0))
ax.legend(fontsize=8, ncol=3, loc="upper left", framealpha=0.8)
ax.set_xticks(B_vals)
fig.savefig(OUTDIR / "improvement_vs_B.pdf")
plt.close(fig)
print("✓ improvement_vs_B.pdf")

# ════════════════════════════════════════════════════
# Plot 2 — Improvement vs n  (one line per B)
# ════════════════════════════════════════════════════
fig, ax = plt.subplots(figsize=(7, 4.5))
for B in B_vals:
    sub = df[df["B"] == B].groupby("n")["improve_pct"].mean()
    ax.plot(sub.index, sub.values, "o-", ms=4, lw=1.4, label=f"B={B}")

ax.set_xlabel("Number of Students  $n$")
ax.set_ylabel("Mean Improvement over Greedy (%)")
ax.set_title("Beam Search Improvement vs Problem Size")
ax.yaxis.set_major_formatter(mtick.PercentFormatter(decimals=0))
ax.legend(fontsize=8, ncol=2, loc="upper left", framealpha=0.8)
ax.set_xticks(n_vals)
fig.savefig(OUTDIR / "improvement_vs_n.pdf")
plt.close(fig)
print("✓ improvement_vs_n.pdf")

# ════════════════════════════════════════════════════
# Plot 3 — Absolute score vs n  (greedy & beam B=10)
# ════════════════════════════════════════════════════
fig, ax = plt.subplots(figsize=(7, 4.5))
grp = df[df["B"] == max(B_vals)].groupby("n")
greedy_mean = grp["greedy_score"].mean()
beam_mean   = grp["beam_score"].mean()
greedy_std  = grp["greedy_score"].std()
beam_std    = grp["beam_score"].std()

ax.errorbar(greedy_mean.index, greedy_mean.values, yerr=greedy_std.values,
            fmt="s-", capsize=3, lw=1.4, label="Greedy")
ax.errorbar(beam_mean.index, beam_mean.values, yerr=beam_std.values,
            fmt="o-", capsize=3, lw=1.4, label=f"Beam  $B={max(B_vals)}$")
ax.set_xlabel("Number of Students  $n$")
ax.set_ylabel("Total Cross-Talk Score")
ax.set_title("Greedy vs Beam Search — Absolute Score")
ax.legend(fontsize=10, framealpha=0.8)
ax.set_xticks(n_vals)
fig.savefig(OUTDIR / "score_vs_n.pdf")
plt.close(fig)
print("✓ score_vs_n.pdf")

# ════════════════════════════════════════════════════
# Plot 4 — Heatmap  (n × B)
# ════════════════════════════════════════════════════
pivot = df.groupby(["n", "B"])["improve_pct"].mean().unstack("B")

fig, ax = plt.subplots(figsize=(8, 5))
im = ax.imshow(pivot.values, aspect="auto", origin="lower", cmap="YlOrRd")
ax.set_xticks(range(len(B_vals)))
ax.set_xticklabels(B_vals)
ax.set_yticks(range(len(n_vals)))
ax.set_yticklabels(n_vals)
ax.set_xlabel("Beam Width  $B$")
ax.set_ylabel("Number of Students  $n$")
ax.set_title("Mean Improvement over Greedy (%)")

# annotate cells
for yi in range(len(n_vals)):
    for xi in range(len(B_vals)):
        val = pivot.values[yi, xi]
        colour = "white" if val > pivot.values.max() * 0.65 else "black"
        ax.text(xi, yi, f"{val:.1f}", ha="center", va="center",
                fontsize=7, color=colour)

cbar = fig.colorbar(im, ax=ax, pad=0.02)
cbar.ax.yaxis.set_major_formatter(mtick.PercentFormatter(decimals=0))
fig.savefig(OUTDIR / "heatmap_improvement.pdf")
plt.close(fig)
print("✓ heatmap_improvement.pdf")

print("\nAll plots saved to", OUTDIR)
