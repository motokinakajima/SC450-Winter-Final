#include "Problem.h"

#include <vector>
#include <cmath>
#include <random>

Problem::Problem(int n, unsigned seed, double aspect_ratio)
    : n(n), aspect_ratio(aspect_ratio) {

    double a = aspect_ratio, b = 1.0;

    cross_talk_cos.assign(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        double px = a * std::cos(2 * M_PI * i / n);
        double py = b * std::sin(2 * M_PI * i / n);
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            double qx = a * std::cos(2 * M_PI * j / n);
            double qy = b * std::sin(2 * M_PI * j / n);
            double oix = px, oiy = py;
            double ijx = qx - px, ijy = qy - py;
            double len_oi = std::sqrt(oix*oix + oiy*oiy);
            double len_ij = std::sqrt(ijx*ijx + ijy*ijy);
            double cos_theta = (oix*ijx + oiy*ijy) / (len_oi * len_ij);
            cross_talk_cos[i][j] = std::abs(cos_theta);
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