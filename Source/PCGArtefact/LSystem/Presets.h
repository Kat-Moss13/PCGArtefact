// Presets.h
// Week 3 Tutorial 1: L-System Engine
//
// Ready-made L-systems from Lecture 1 (after Prusinkiewicz & Lindenmayer, 1990),
// plus MyPlant(): the one you design in Part D.

#pragma once

#include "LSystem.h"

#include <vector>

namespace LSystem::Presets
{
    [[nodiscard]] LSystemDefinition Algae();          // A -> AB, B -> A (no drawing)
    [[nodiscard]] LSystemDefinition KochCurve();      // Deterministic, self-similar
    [[nodiscard]] LSystemDefinition DragonCurve();    // Growth symbols X and Y
    [[nodiscard]] LSystemDefinition BracketTree();    // Deterministic branching
    [[nodiscard]] LSystemDefinition FractalPlant();   // Branching with growth symbol X
    [[nodiscard]] LSystemDefinition StochasticBush(); // Three weighted productions for F
    [[nodiscard]] LSystemDefinition MyPlant();        // STEP D1: your design

    // Every drawable preset, in the order main() renders them.
    [[nodiscard]] std::vector<LSystemDefinition> AllDrawable();
}
