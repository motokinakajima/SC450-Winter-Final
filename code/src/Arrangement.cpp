#include "Arrangement.h"

Arrangement::Arrangement(int n)
    : order(n, -1), seat_taken(n, false), seat_potential(n, 0.0) {}

double Arrangement::score_gain(int student_idx, int seat) const {
    return seat_potential[seat] * (seat_taken[seat] ? 0.0 : 1.0);
}

void Arrangement::place(int student_idx, int seat, const Problem& problem) {
    double v = problem.talkativeness[student_idx];
    current_score += 2.0 * v * seat_potential[seat];
    order[seat] = student_idx;
    seat_taken[seat] = true;
    for (int s = 0; s < (int)order.size(); ++s)
        if (!seat_taken[s])
            seat_potential[s] += v * get_cross_talk(problem, s, seat);
}