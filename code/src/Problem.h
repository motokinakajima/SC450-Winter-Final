#pragma once

#include <vector>

struct Problem {
    int n;
    std::vector<std::vector<double>> affinity;
    std::vector<std::vector<double>> cross_talk_cos;

    Problem(int n, unsigned seed);
};

double get_cross_talk(const Problem& problem, int i, int j);