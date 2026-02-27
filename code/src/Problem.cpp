#include "Problem.h"

#include <vector>
#include <cmath>

Problem::Problem(int n, const std::vector<double>& talkativeness) : n(n), talkativeness(talkativeness) {
    update_cross_talk(*this);
}

void update_cross_talk(Problem& problem) {
    problem.cross_talk_cos = std::vector<std::vector<double>>(problem.n, std::vector<double>(problem.n, 0.0));

    for (int i = 0; i < problem.n; ++i) {
        for (int j = 0; j < problem.n; ++j) {
            if (i != j) {
                double theta_inscribed_ij = (static_cast<double>(std::abs(i - j)) / problem.n) * (2 * M_PI);
                double theta_central_ij = std::min(theta_inscribed_ij, 2 * M_PI - theta_inscribed_ij);
                double theta_ij = (M_PI - theta_central_ij) / 2;
                problem.cross_talk_cos[i][j] = std::abs(std::cos(theta_ij));
            }
        }
    }
}

double calculate_total_cross_talk(const Problem& problem) {
    double total_cross_talk = 0.0;

    for (int i = 0; i < problem.n; ++i) {
        for (int j = 0; j < problem.n; ++j) {
            if (i != j) {
                total_cross_talk += problem.talkativeness[i] * problem.talkativeness[j] * problem.cross_talk_cos[i][j];
            }
        }
    }

    return total_cross_talk;
}

double get_cross_talk(const Problem& problem, int i, int j) {
    return problem.cross_talk_cos[i][j];
}