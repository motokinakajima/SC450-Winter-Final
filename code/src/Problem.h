#pragma once

#include <vector>

struct Problem {
    int n;

    std::vector<double> talkativeness;

    std::vector<std::vector<double>> cross_talk_cos;

    Problem(int n, const std::vector<double>& talkativeness);
};

void update_cross_talk(Problem& problem);

double calculate_total_cross_talk(const Problem& problem);

double get_cross_talk(const Problem& problem, int i, int j);