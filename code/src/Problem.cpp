#include "Problem.h"

#include <vector>
#include <cmath>
#include <random>

Problem::Problem(int n, unsigned seed) : n(n) {
    cross_talk_cos.assign(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            int k = std::min(std::abs(i - j), n - std::abs(i - j));
            double central_angle = 2.0 * M_PI * k / n;
            cross_talk_cos[i][j] = std::abs(std::sin(central_angle));
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