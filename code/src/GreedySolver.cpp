#include "GreedySolver.h"

Arrangement GreedySolver::solve() const {
    Arrangement arrangement(problem.n);

    for (int step = 0; step < problem.n; ++step) {
        int best_student = -1, best_seat = -1;
        double best_gain = -1e18;
        for (int k = 0; k < problem.n; ++k) {
            if (arrangement.student_placed[k]) continue;
            for (int s = 0; s < problem.n; ++s) {
                if (arrangement.seat_taken[s]) continue;
                double g = arrangement.gain(k, s);
                double cur_score = arrangement.current_score + g;
                double best_score = arrangement.current_score + best_gain;
                if (cur_score > best_score ||
                    (cur_score == best_score && (k < best_student || (k == best_student && s < best_seat)))) {
                    best_gain = g;
                    best_student = k;
                    best_seat = s;
                }
            }
        }
        arrangement.place(best_student, best_seat, problem);
    }

    return arrangement;
}
