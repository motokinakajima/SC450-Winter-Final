#include "BeamSearchSolver.h"

#include <vector>
#include <algorithm>
#include <unordered_map>

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

        const int max_per_parent = 2;
        std::unordered_map<int, int> parent_count;
        std::vector<Candidate> selected;
        selected.reserve(beam_width);

        int greedy_cand_idx = -1;
        for (int i = 0; i < (int)scored.size(); ++i) {
            if (scored[i].beam_idx == 0) {
                greedy_cand_idx = i;
                break;
            }
        }

        if (greedy_cand_idx >= 0) {
            selected.push_back(scored[greedy_cand_idx]);
            parent_count[0] = 1;
        }

        for (int i = 0; i < (int)scored.size() && (int)selected.size() < beam_width; ++i) {
            if (i == greedy_cand_idx) continue;
            if (parent_count[scored[i].beam_idx] < max_per_parent) {
                selected.push_back(scored[i]);
                parent_count[scored[i].beam_idx]++;
            }
        }

        std::vector<Arrangement> new_beam;
        new_beam.reserve(selected.size());
        for (auto& c : selected) {
            Arrangement next = beam[c.beam_idx];
            next.place(c.student_idx, c.seat, problem);
            new_beam.push_back(std::move(next));
        }
        beam = std::move(new_beam);
    }

    auto best = std::max_element(beam.begin(), beam.end(),
        [](const Arrangement& a, const Arrangement& b) {
            return a.current_score < b.current_score;
        });
    return *best;
}
