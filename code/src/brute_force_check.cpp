#include "Problem.h"
#include "Arrangement.h"
#include "GreedySolver.h"
#include "BeamSearchSolver.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <numeric>
#include <cmath>

// Brute-force: try all n! permutations, return optimal score
double brute_force_optimal(const Problem& problem) {
    int n = problem.n;
    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0);  // 0,1,...,n-1

    double best = -1e18;
    do {
        // compute total score for this permutation
        // perm[seat] = student placed at that seat
        double score = 0.0;
        for (int s1 = 0; s1 < n; ++s1) {
            for (int s2 = s1 + 1; s2 < n; ++s2) {
                int i = perm[s1], j = perm[s2];
                double ct = problem.cross_talk_cos[s1][s2];
                double a  = problem.affinity[i][j];
                score += 2.0 * a * ct;  // symmetric pair
            }
        }
        if (score > best) best = score;
    } while (std::next_permutation(perm.begin(), perm.end()));

    return best;
}

int main() {
    const double AR = 11.0 / 7.0;
    const int TRIALS = 50;

    std::cout << std::setprecision(15);
    std::cout << "n,trial,greedy,beam20,optimal,greedy/opt%,beam20/opt%\n";

    for (int n : {5, 10}) {  // 5!=120, 10!=3628800 (still feasible)
        std::cerr << "n=" << n << " ..." << std::endl;
        for (int trial = 0; trial < TRIALS; ++trial) {
            unsigned seed = static_cast<unsigned>(n * 10000 + trial);
            Problem prob(n, seed, AR);

            double greedy = GreedySolver(prob).solve().current_score;
            double beam20 = BeamSearchSolver(prob, 20).solve().current_score;
            double opt    = brute_force_optimal(prob);

            std::cout << n << "," << trial << ","
                      << greedy << "," << beam20 << "," << opt << ","
                      << (greedy / opt * 100) << ","
                      << (beam20 / opt * 100) << "\n";
        }
    }

    std::cerr << "Done." << std::endl;
    return 0;
}
