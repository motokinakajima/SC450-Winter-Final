#include "BeamSearchSolver.h"

#include <vector>
#include <algorithm>

Arrangement BeamSearchSolver::solve() const {
    std::vector<Arrangement> beam = {Arrangement(problem.n)};

    for (int step = 0; step < problem.n; ++step) {
        struct Candidate {
            double score;
            int beam_idx, student_idx, seat;
        };

        std::vector<Candidate> scored;
        scored.reserve(beam.size() * problem.n * problem.n);

        for (int bi = 0; bi < (int)beam.size(); ++bi) {
            const Arrangement& arr = beam[bi];
            for (int k = 0; k < problem.n; ++k) {
                if (arr.student_placed[k]) continue;
                for (int s = 0; s < problem.n; ++s) {
                    if (arr.seat_taken[s]) continue;
                    scored.push_back({arr.current_score + arr.gain(k, s), bi, k, s});
                }
            }
        }

        std::stable_sort(scored.begin(), scored.end(), [](const Candidate& a, const Candidate& b) {
            if (a.score != b.score) return a.score > b.score;
            if (a.beam_idx != b.beam_idx) return a.beam_idx < b.beam_idx;
            if (a.student_idx != b.student_idx) return a.student_idx < b.student_idx;
            return a.seat < b.seat;
        });

        int keep = std::min((int)scored.size(), beam_width);
        std::vector<Arrangement> new_beam;
        new_beam.reserve(keep);
        for (int i = 0; i < keep; ++i) {
            Arrangement next = beam[scored[i].beam_idx];
            next.place(scored[i].student_idx, scored[i].seat, problem);
            new_beam.push_back(std::move(next));
        }
        beam = std::move(new_beam);
    }

    return beam.front();
}
