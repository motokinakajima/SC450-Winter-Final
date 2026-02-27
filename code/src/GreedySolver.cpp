#include "GreedySolver.h"

Arrangement GreedySolver::solve() const {
    Arrangement arrangement(problem.n);

    for (int step = 0; step < problem.n; ++step) {
        int best_student = -1, best_seat = -1;
        double best_gain = -1.0;
        for (int k = 0; k < problem.n; ++k) {
            if (arrangement.student_placed[k]) continue;
            for (int s = 0; s < problem.n; ++s) {
                if (arrangement.seat_taken[s]) continue;
                double g = arrangement.gain(k, s);
                if (g > best_gain) {
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
