#include "GreedySolver.h"

#include <vector>
#include <algorithm>
#include <numeric>

Arrangement GreedySolver::solve() const {
    std::vector<int> students(problem.n);
    std::iota(students.begin(), students.end(), 0);
    std::sort(students.begin(), students.end(), [&](int a, int b) {
        return problem.talkativeness[a] > problem.talkativeness[b];
    });

    Arrangement arrangement(problem.n);
    arrangement.place(students[0], 0, problem);

    for (int k = 1; k < problem.n; ++k) {
        int student = students[k];
        double v_k = problem.talkativeness[student];

        int best_seat = -1;
        double best_gain = -1.0;
        for (int s = 0; s < problem.n; ++s) {
            if (!arrangement.seat_taken[s]) {
                double gain = v_k * arrangement.seat_potential[s];
                if (gain > best_gain) {
                    best_gain = gain;
                    best_seat = s;
                }
            }
        }

        arrangement.place(student, best_seat, problem);
    }

    return arrangement;
}
