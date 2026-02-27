#include "Problem.h"
#include "Arrangement.h"
#include "GreedySolver.h"
#include "BeamSearchSolver.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

int main() {
    const int N_MIN = 5;
    const int N_MAX = 100;
    const int N_STEP = 5;
    const int B_MAX = 10;
    const int TRIALS = 20;

    std::ofstream out("results.csv");
    if (!out) {
        std::cerr << "Failed to open results.csv" << std::endl;
        return 1;
    }

    out << "n,B,trial,greedy_score,beam_score\n";

    for (int n = N_MIN; n <= N_MAX; n += N_STEP) {
        std::cerr << "n = " << n << std::endl;

        for (int trial = 0; trial < TRIALS; ++trial) {
            std::mt19937 rng(static_cast<unsigned>(n * 10000 + trial));
            std::uniform_real_distribution<double> dist(0.0, 1.0);

            std::vector<double> talkativeness(n);
            for (int i = 0; i < n; ++i)
                talkativeness[i] = dist(rng);

            Problem problem(n, talkativeness);

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
