// LSystem.h
// Week 3 Tutorial 1: L-System Engine
//
// Parallel rewriting: every symbol in the current string is rewritten in the
// same step, reading from generation n and writing a new generation n + 1.

#pragma once

#include "RuleSet.h"

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace LSystem
{
    // Everything needed to describe one L-system, including how to draw it.
    struct LSystemDefinition
    {
        std::string Name;
        std::string Axiom;
        RuleSet     Rules;
        int         Iterations            = 4;
        double      AngleDegrees          = 25.0;
        double      InitialHeadingDegrees = 90.0;  // 90 = pointing up the screen.
        double      LengthScalePerDepth   = 1.0;   // < 1 makes deeper branches shorter.
        std::string DrawSymbols           = "F";   // Symbols that draw a line.
        std::string MoveSymbols           = "f";   // Symbols that move without drawing.
    };

    struct GenerationSettings
    {
        std::uint64_t Seed       = 1;
        std::size_t   MaxSymbols = 5'000'000; // Safety cap (~5 MB of text).
    };

    struct GenerationResult
    {
        std::string Symbols;
        std::vector<std::size_t> LengthPerIteration; // Index 0 is the axiom.
        int  IterationsCompleted = 0;
        bool Success = true;
        std::string Error;
    };

    // STEP B2: choose one of several weighted productions, reproducibly.
    // The choice must be a pure function of (seed, iteration, position).
    [[nodiscard]] const std::string& ChooseSuccessor(std::span<const Production> options,
                                                     std::uint64_t seed, int iteration,
                                                     std::size_t position);

    // STEP B1: one parallel rewriting step.
    [[nodiscard]] std::string Rewrite(std::string_view current, const RuleSet& rules,
                                      std::uint64_t seed, int iteration);

    // Expected string length after 'iterations' steps, without rewriting anything.
    // Uses symbol counts and probabilities: the growth-matrix idea from Lecture 1.
    [[nodiscard]] double PredictLength(std::string_view axiom, const RuleSet& rules, int iterations);

    // Run the full derivation with a size check before every step.
    [[nodiscard]] GenerationResult Generate(const LSystemDefinition& definition,
                                            const GenerationSettings& settings);
}
