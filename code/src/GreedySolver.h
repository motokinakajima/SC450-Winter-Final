#pragma once

#include "Solver.h"

struct GreedySolver : Solver {
    GreedySolver(const Problem& problem) : Solver(problem) {}

    Arrangement solve() const override;
};