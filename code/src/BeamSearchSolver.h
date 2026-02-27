#pragma once

#include "Solver.h"

struct BeamSearchSolver : Solver {
    int beam_width;

    BeamSearchSolver(const Problem& problem, int beam_width) : Solver(problem), beam_width(beam_width) {}

    Arrangement solve() const override;
};