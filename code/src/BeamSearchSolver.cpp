#include "BeamSearchSolver.h"

#include <vector>
#include <algorithm>
#include <numeric>

Arrangement BeamSearchSolver::solve() const {
    std::vector<int> students(problem.n);
    std::iota(students.begin(), students.end(), 0);
    std::sort(students.begin(), students.end(), [&](int a, int b) {
        return problem.talkativeness[a] > problem.talkativeness[b];
    });

    Arrangement initial(problem.n);
    initial.place(students[0], 0, problem);

    std::vector<Arrangement> beam = {initial};

    for (int k = 1; k < problem.n; ++k) {
        int student = students[k];
        double v_k = problem.talkativeness[student];

        struct Candidate {
            double score;
            int beam_idx;
            int seat;
        };

        std::vector<Candidate> scored;
        scored.reserve(beam.size() * (problem.n - k));

        for (int bi = 0; bi < (int)beam.size(); ++bi) {
            const Arrangement& arr = beam[bi];
            for (int s = 0; s < problem.n; ++s) {
                if (!arr.seat_taken[s]) {
                    double gain = 2.0 * v_k * arr.seat_potential[s];
                    scored.push_back({arr.current_score + gain, bi, s});
                }
            }
        }

        std::sort(scored.begin(), scored.end(), [](const Candidate& a, const Candidate& b) {
            return a.score > b.score;
        });

        int keep = std::min((int)scored.size(), beam_width);
        std::vector<Arrangement> new_beam;
        new_beam.reserve(keep);

        for (int i = 0; i < keep; ++i) {
            Arrangement next = beam[scored[i].beam_idx];
            next.place(student, scored[i].seat, problem);
            new_beam.push_back(std::move(next));
        }

        beam = std::move(new_beam);
    }

    return beam.front();
}
