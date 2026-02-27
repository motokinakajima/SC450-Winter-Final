#include "Problem.h"

#include <vector>
#include <cmath>
#include <random>

Problem::Problem(int n, unsigned seed, double aspect_ratio)
    : n(n), aspect_ratio(aspect_ratio) {

    double a = aspect_ratio, b = 1.0;

    // 楕円上の座標
    std::vector<double> px(n), py(n);
    for (int i = 0; i < n; ++i) {
        px[i] = a * std::cos(2.0 * M_PI * i / n);
        py[i] = b * std::sin(2.0 * M_PI * i / n);
    }

    cross_talk_cos.assign(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            // OP_i と OP_j のなす角
            double dot = px[i]*px[j] + py[i]*py[j];
            double len_i = std::sqrt(px[i]*px[i] + py[i]*py[i]);
            double len_j = std::sqrt(px[j]*px[j] + py[j]*py[j]);
            double cos_angle = dot / (len_i * len_j);
            cos_angle = std::max(-1.0, std::min(1.0, cos_angle));
            double angle = std::acos(cos_angle);
            double s = std::abs(std::sin(angle));
            double c = std::abs(std::cos(angle));
            double half_cos = std::cos(angle / 2.0);
            // sin^16 + 0.3*cos^8 - λ*cos^64(θ/2)
            cross_talk_cos[i][j] = std::pow(s, 16)
                                 + 0.3 * std::pow(c, 8)
                                 - 4.5 * std::pow(half_cos, 64);
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