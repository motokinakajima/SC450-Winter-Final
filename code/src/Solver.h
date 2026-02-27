#pragma once

#include "Arrangement.h"
#include "Problem.h"

struct Solver {
    const Problem& problem;

    Solver(const Problem& problem) : problem(problem) {}

    virtual Arrangement solve() const = 0;

    virtual ~Solver() = default;
};