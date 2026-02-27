#include "Arrangement.h"

Arrangement::Arrangement(int n)
    : order(n, -1), seat_taken(n, false), student_placed(n, false),
      partial_gain(n, std::vector<double>(n, 0.0)) {}

double Arrangement::gain(int student_idx, int seat) const {
    return 2.0 * partial_gain[student_idx][seat];
}

void Arrangement::place(int student_idx, int seat, const Problem& problem) {
    current_score += gain(student_idx, seat);
    order[seat] = student_idx;
    seat_taken[seat] = true;
    student_placed[student_idx] = true;
    for (int m = 0; m < (int)order.size(); ++m) {
        if (student_placed[m]) continue;
        for (int s = 0; s < (int)order.size(); ++s) {
            if (seat_taken[s]) continue;
            partial_gain[m][s] += problem.affinity[m][student_idx] * get_cross_talk(problem, s, seat);
        }
    }
}