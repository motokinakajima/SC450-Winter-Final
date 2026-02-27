# SC450 Copilot Session Continuation Log

**Last updated**: February 27, 2026  
**AI model used**: GitHub Copilot (Claude Sonnet 4.6)  
**Purpose**: Resume context for next AI session. Read this entire file before continuing.

---

## 1. Project Overview

- **Course**: SC450 (advanced CS research paper course at St. Paul's School)
- **Student**: Motoki Nakajima
- **Research question**: In a circular Harkness table discussion, how effective is the **beam search algorithm**, compared to a simple **greedy algorithm**, in determining the table arrangement that maximizes cross-talk between students?
- **Topic category**: 7 — "A well-defined algorithmic modification"
- **Language**: C++17 compiled with `g++ -std=c++17 -O2 -Wall`
- **Visualization**: Python + matplotlib (not yet written)
- **Paper**: LaTeX in `paper/main.tex`

**Important note on project direction**: A humanities teacher's perspective guides this project. The goal is not to manufacture a large beam-search "win" — honest experimental reporting is the priority, even if beam search only modestly outperforms greedy.

---

## 2. Directory Structure

```
SC450-Winter-Final/
├── AI_LOG.md                      ← original AI usage log (do not modify)
├── COPILOT_SESSION_LOG.md         ← this file
├── README.md
├── paper/
│   └── main.tex                   ← LaTeX paper (needs major updates — see Section 7)
└── code/
    └── src/
        ├── Problem.h/.cpp
        ├── Arrangement.h/.cpp
        ├── Solver.h
        ├── GreedySolver.h/.cpp
        ├── BeamSearchSolver.h/.cpp
        ├── main.cpp
        └── results.csv            ← output of latest run
```

Compile & run (from `code/src/`):
```bash
g++ -std=c++17 -O2 -Wall -o main Problem.cpp Arrangement.cpp GreedySolver.cpp BeamSearchSolver.cpp main.cpp && ./main
```

---

## 3. Problem Formulation (QAP)

**Quadratic Assignment Problem**: assign $n$ students to $n$ seats on an **elliptical** table (aspect ratio 2:1) to maximize:

$$\text{Score} = \sum_{i \neq j} a_{ij} \cdot \text{cross\_talk}[\sigma(i)][\sigma(j)]$$

- Seat positions: $P_s = (2\cos\frac{2\pi s}{n},\ \sin\frac{2\pi s}{n})$  
- `affinity[i][j]` = random pairwise student compatibility, symmetric, uniform in $[0,1]$  
- `cross_talk_cos[s][t]` = geometric factor between seats $s$ and $t$ (formula disputed — see Section 4)  
- RNG seed: `n * 10000 + trial` (deterministic, fully reproducible)

**Harkness context**: Students sit around an oval table and discuss freely. The teacher's insight: a student "reaching" toward someone at roughly 90° to their own radial direction has maximal cross-talk (not just talking to a neighbor, nor to someone directly across). This motivates the `|cos|` formula below.

---

## 4. Cross-Talk Formula — History & Recommendation

### ⚠️ Current code (UNDERPERFORMING — revert this):
```cpp
// |sin(angle between vector O→Pi and vector O→Pj)| on ellipse
double dot = px[i]*px[j] + py[i]*py[j];
double len_i = std::sqrt(px[i]*px[i] + py[i]*py[i]);
double len_j = std::sqrt(px[j]*px[j] + py[j]*py[j]);
double cos_angle = dot / (len_i * len_j);
cos_angle = std::max(-1.0, std::min(1.0, cos_angle));
cross_talk_cos[i][j] = std::abs(std::sin(std::acos(cos_angle)));
// Results: B=10 avg +0.71%, beam < greedy in 776/2000 cases (38.8%)
```

### ✅ Best formula found (REVERT TO THIS):
```cpp
// |cos(angle between direction O→Pi and direction Pi→Pj)| on ellipse
double oix = px[i], oiy = py[i];               // O → Pi vector
double ijx = px[j]-px[i], ijy = py[j]-py[i];  // Pi → Pj vector
double len_oi = std::sqrt(oix*oix + oiy*oiy);
double len_ij = std::sqrt(ijx*ijx + ijy*ijy);
double cos_theta = (oix*ijx + oiy*ijy) / (len_oi * len_ij);
cross_talk_cos[i][j] = std::abs(cos_theta);
// Results: B=10 avg +4-5%, beam < greedy ~13/2000 (<1%)
```

Interpretation: "how much does the direction from the center through student $i$ align with the direction from $i$ toward student $j$?" — maximized when $j$ is roughly perpendicular to $i$'s radial direction, which matches the teacher's 90° intuition.

---

## 5. Full Source Code (current state as of this session)

### `Problem.h`
```cpp
#pragma once
#include <vector>

struct Problem {
    int n;
    double aspect_ratio;
    std::vector<std::vector<double>> affinity;
    std::vector<std::vector<double>> cross_talk_cos;

    Problem(int n, unsigned seed, double aspect_ratio = 2.0);
};

double get_cross_talk(const Problem& problem, int i, int j);
```

### `Problem.cpp` (current — underperforming, revert cross-talk block)
```cpp
#include "Problem.h"
#include <vector>
#include <cmath>
#include <random>

Problem::Problem(int n, unsigned seed, double aspect_ratio)
    : n(n), aspect_ratio(aspect_ratio) {

    double a = aspect_ratio, b = 1.0;
    std::vector<double> px(n), py(n);
    for (int i = 0; i < n; ++i) {
        px[i] = a * std::cos(2.0 * M_PI * i / n);
        py[i] = b * std::sin(2.0 * M_PI * i / n);
    }

    cross_talk_cos.assign(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            // ← REPLACE THIS BLOCK with the |cos| version from Section 4
            double dot = px[i]*px[j] + py[i]*py[j];
            double len_i = std::sqrt(px[i]*px[i] + py[i]*py[i]);
            double len_j = std::sqrt(px[j]*px[j] + py[j]*py[j]);
            double cos_angle = dot / (len_i * len_j);
            cos_angle = std::max(-1.0, std::min(1.0, cos_angle));
            double angle = std::acos(cos_angle);
            cross_talk_cos[i][j] = std::abs(std::sin(angle));
        }
    }

    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    affinity.assign(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            affinity[i][j] = affinity[j][i] = dist(rng);
}

double get_cross_talk(const Problem& problem, int i, int j) {
    return problem.cross_talk_cos[i][j];
}
```

### `Arrangement.h`
```cpp
#pragma once
#include "Problem.h"
#include <vector>

struct Arrangement {
    std::vector<int> order;                          // order[seat] = student_idx, -1 if empty
    std::vector<bool> seat_taken;
    std::vector<bool> student_placed;
    std::vector<std::vector<double>> partial_gain;   // [student][seat], O(1) gain lookup
    double current_score = 0.0;

    Arrangement(int n);
    double gain(int student_idx, int seat) const;
    void place(int student_idx, int seat, const Problem& problem);
};
```

### `Arrangement.cpp`
```cpp
#include "Arrangement.h"

Arrangement::Arrangement(int n)
    : order(n, -1), seat_taken(n, false), student_placed(n, false),
      partial_gain(n, std::vector<double>(n, 0.0)) {}

double Arrangement::gain(int student_idx, int seat) const {
    return 2.0 * partial_gain[student_idx][seat];
}

void Arrangement::place(int student_idx, int seat, const Problem& problem) {
    current_score += gain(student_idx, seat);
    order[seat] = student_idx;
    seat_taken[seat] = true;
    student_placed[student_idx] = true;
    // Update partial_gain for all (unplaced student, empty seat) pairs
    for (int m = 0; m < (int)order.size(); ++m) {
        if (student_placed[m]) continue;
        for (int s = 0; s < (int)order.size(); ++s) {
            if (seat_taken[s]) continue;
            partial_gain[m][s] += problem.affinity[m][student_idx]
                                 * get_cross_talk(problem, s, seat);
        }
    }
}
```

### `Solver.h`
```cpp
#pragma once
#include "Arrangement.h"

struct Solver {
    const Problem& problem;
    Solver(const Problem& p) : problem(p) {}
    virtual Arrangement solve() const = 0;
    virtual ~Solver() = default;
};
```

### `GreedySolver.h`
```cpp
#pragma once
#include "Solver.h"

struct GreedySolver : Solver {
    GreedySolver(const Problem& problem) : Solver(problem) {}
    Arrangement solve() const override;
};
```

### `GreedySolver.cpp`
```cpp
#include "GreedySolver.h"

Arrangement GreedySolver::solve() const {
    Arrangement arrangement(problem.n);
    for (int step = 0; step < problem.n; ++step) {
        int best_student = -1, best_seat = -1;
        double best_gain = -1.0;
        for (int k = 0; k < problem.n; ++k) {
            if (arrangement.student_placed[k]) continue;
            for (int s = 0; s < problem.n; ++s) {
                if (arrangement.seat_taken[s]) continue;
                double g = arrangement.gain(k, s);
                if (g > best_gain) {
                    best_gain = g;
                    best_student = k;
                    best_seat = s;
                }
            }
        }
        arrangement.place(best_student, best_seat, problem);
    }
    return arrangement;
}
```

### `BeamSearchSolver.h`
```cpp
#pragma once
#include "Solver.h"

struct BeamSearchSolver : Solver {
    int beam_width;
    BeamSearchSolver(const Problem& problem, int B) : Solver(problem), beam_width(B) {}
    Arrangement solve() const override;
};
```

### `BeamSearchSolver.cpp`
```cpp
#include "BeamSearchSolver.h"
#include <vector>
#include <algorithm>

Arrangement BeamSearchSolver::solve() const {
    std::vector<Arrangement> beam = {Arrangement(problem.n)};

    for (int step = 0; step < problem.n; ++step) {
        struct Candidate {
            double score;
            int beam_idx, student_idx, seat;
        };
        std::vector<Candidate> scored;
        scored.reserve(beam.size() * problem.n * problem.n);

        for (int bi = 0; bi < (int)beam.size(); ++bi) {
            const Arrangement& arr = beam[bi];
            for (int k = 0; k < problem.n; ++k) {
                if (arr.student_placed[k]) continue;
                for (int s = 0; s < problem.n; ++s) {
                    if (arr.seat_taken[s]) continue;
                    scored.push_back({arr.current_score + arr.gain(k, s), bi, k, s});
                }
            }
        }

        // Deterministic tiebreaking: score desc, then beam_idx asc, student asc, seat asc
        std::stable_sort(scored.begin(), scored.end(), [](const Candidate& a, const Candidate& b) {
            if (a.score != b.score) return a.score > b.score;
            if (a.beam_idx != b.beam_idx) return a.beam_idx < b.beam_idx;
            if (a.student_idx != b.student_idx) return a.student_idx < b.student_idx;
            return a.seat < b.seat;
        });

        int keep = std::min((int)scored.size(), beam_width);
        std::vector<Arrangement> new_beam;
        new_beam.reserve(keep);
        for (int i = 0; i < keep; ++i) {
            Arrangement next = beam[scored[i].beam_idx];
            next.place(scored[i].student_idx, scored[i].seat, problem);
            new_beam.push_back(std::move(next));
        }
        beam = std::move(new_beam);
    }
    return beam.front();
}
```

### `main.cpp`
```cpp
#include "Problem.h"
#include "Arrangement.h"
#include "GreedySolver.h"
#include "BeamSearchSolver.h"
#include <iostream>
#include <fstream>

int main() {
    const int N_MIN = 5;
    const int N_MAX = 50;   // paper says 100 — sync after formula settled
    const int N_STEP = 5;
    const int B_MAX = 10;
    const int TRIALS = 20;

    std::ofstream out("results.csv");
    if (!out) { std::cerr << "Failed to open results.csv\n"; return 1; }
    out << "n,B,trial,greedy_score,beam_score\n";

    for (int n = N_MIN; n <= N_MAX; n += N_STEP) {
        std::cerr << "n = " << n << std::endl;
        for (int trial = 0; trial < TRIALS; ++trial) {
            unsigned seed = static_cast<unsigned>(n * 10000 + trial);
            Problem problem(n, seed, 2.0);
            double greedy_score = GreedySolver(problem).solve().current_score;
            for (int B = 1; B <= B_MAX; ++B) {
                double beam_score = BeamSearchSolver(problem, B).solve().current_score;
                out << n << "," << B << "," << trial << ","
                    << greedy_score << "," << beam_score << "\n";
            }
        }
    }
    out.close();
    std::cerr << "Done." << std::endl;
    return 0;
}
```

---

## 6. Experimental Results (as of this session)

### Current run (underperforming `|sin|` formula):
| Metric | Value |
|---|---|
| B=1 same as greedy | 200/200 ✓ |
| beam < greedy (prune bug) | **776/2000 (38.8%)** |
| B=10 avg improvement | **+0.71%** |
| n=50, B=10, trial=2 | beam=-1.58% (beam losing!) |

### Best run (earlier `|cos|` formula):
| Metric | Value |
|---|---|
| B=1 same as greedy | 200/200 ✓ |
| beam < greedy | ~13/2000 (<1%) |
| B=10 avg improvement | **+4–5%** |
| n=50 typical diff | ~25–39 score points |

---

## 7. Paper ↔ Code Mismatches (need to fix in `paper/main.tex`)

| Item | Paper says | Code actually does |
|---|---|---|
| Table shape | circular | **ellipse, aspect_ratio=2.0** |
| Score formula | `\|v_i v_j cos(θ_ij)\|` with per-student talkativeness | `a_ij * cross_talk[seat_i][seat_j]` with **pairwise affinity** |
| n range | 5 to **100** | 5 to **50** (N_MAX) |
| RNG | `std::random_device` | `std::mt19937` seeded with `n*10000+trial` |
| Greedy complexity | O(n²) | **O(n³)** (n steps × n² pairs) |
| Loop invariant claim | "beam always ≥ greedy" | **false** with current beam implementation |

---

## 8. Known Issues

### Issue 1: `beam < greedy` (prune bug — unresolved)
**Root cause**: At step $k$, the greedy algorithm's partial path always picks one single best move. That path is not explicitly kept in the beam. If it doesn't rank in the top-$B$ candidates at step $k$, it gets pruned. Later, the pruned greedy path may produce a higher final score than all retained beam paths.

**Two options**:
1. Force greedy path into `beam[0]` at every step → guarantees beam ≥ greedy always → enables loop invariant proof. (Needs implementation.)
2. Accept beam < greedy as a valid experimental finding → revise paper's theoretical section to note this limitation. (User currently OK with this approach.)

### Issue 2: Score formula not settled
The `|sin|` version (current) is worse. The `|cos|` version (earlier) gave best results. Recommend reverting — see Section 4.

### Issue 3: Paper entirely outdated
`main.tex` still describes the original circular-table per-student-talkativeness formulation. Needs full rewrite of Sections: Formal Problem Definition, Theoretical Plan, Experimental Plan.

---

## 9. TODO (next session, priority order)

1. **Revert `Problem.cpp`** cross-talk formula to `|cos|` version (Section 4). Recompile and run sanity checks.
2. **Write `code/src/plot.py`** — matplotlib:
   - Plot 1: 3D surface — avg beam score vs $n$ vs $B$
   - Plot 2: 2D line — greedy vs beam at $B$ with **largest** avg difference
   - Plot 3: 2D line — greedy vs beam at $B$ with **smallest** avg difference
3. **Update `paper/main.tex`**: rewrite formal problem definition, correct score formula, table shape, n range, RNG description, complexity, loop invariant.
4. Optionally increase N_MAX to 100 for final run (may take ~40s). Sync with paper.
5. Decide on loop invariant approach (force greedy in beam vs. report as finding).

---

## 10. Sanity Check Commands

Run these from `code/src/` after any code change:
```bash
# B=1 must equal greedy (expected: 200/200)
awk -F',' 'NR>1 && $2==1 {same+=($4==$5); total++} END {print "B=1 same/total:", same"/"total}' results.csv

# beam < greedy count (want near 0)
awk -F',' 'NR>1 && $5<$4 {bad++} NR>1 {total++} END {print "beam < greedy:", bad"/"total}' results.csv

# B=10 average % improvement (want ~4-5% with |cos| formula)
awk -F',' 'NR>1 && $2==10 {diff+=($5-$4)/$4*100; total++} END {printf "B=10 avg improvement: %.2f%%\n", diff/total}' results.csv

# n=50, B=10 per-trial detail
awk -F',' 'NR>1 && $2==10 && $1==50 {printf "trial=%d greedy=%.3f beam=%.3f diff%%=%.2f\n",$3,$4,$5,($5-$4)/$4*100}' results.csv | head -10
```

---

## 11. Design Decisions History

| Step | Change | Result |
|---|---|---|
| 1 | Circular table, per-student `v_i`, score=`\|v_i v_j cos(θ_ij)\|` | beam ≈ greedy always (circular symmetry) |
| 2 | Added pairwise affinity `a_ij` | slight improvement |
| 3 | Added elliptical table (aspect=2.0) | +4–5% beam improvement, <1% beam<greedy |
| 4 | Changed formula to `\|sin(angle OPi, OPj)\|` ← **current** | regressed to +0.71%, 38.8% beam<greedy |

**Verdict**: Step 3 configuration was best. Revert to it.
