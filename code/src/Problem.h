#pragma once

#include <vector>

struct Problem {
    int n;
    double aspect_ratio;
    std::vector<std::vector<double>> affinity;
    std::vector<std::vector<double>> cross_talk_coeff;

    Problem(int n, unsigned seed, double aspect_ratio = 2.0);
};

double get_cross_talk(const Problem& problem, int i, int j);