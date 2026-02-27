#pragma once

#include <vector>
#include "Problem.h"

struct Arrangement {
    std::vector<int> order;
    std::vector<bool> seat_taken;
    std::vector<bool> student_placed;
    std::vector<std::vector<double>> partial_gain;
    double current_score = 0.0;

    Arrangement(int n);

    double gain(int student_idx, int seat) const;
    void place(int student_idx, int seat, const Problem& problem);

    bool operator<(const Arrangement& other) const {
        return current_score > other.current_score;
    }
};