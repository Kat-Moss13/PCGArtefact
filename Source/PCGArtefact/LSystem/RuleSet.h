// RuleSet.h
// Week 3 Tutorial 1: L-System Engine
//
// Stores the productions of an L-system. Each symbol (one char) can have
// zero productions (identity), one production (deterministic) or several
// weighted productions (stochastic).

#pragma once

#include <array>
#include <cstddef>
#include <span>
#include <string>
#include <vector>

namespace LSystem
{
    struct Production
    {
        std::string Successor;     // What the predecessor becomes. May be empty.
        double Probability = 1.0;  // Only meaningful when a symbol has several productions.
    };

    class RuleSet
    {
    public:
        // Add a production: predecessor -> successor, chosen with the given probability.
        void Add(char predecessor, std::string successor, double probability = 1.0);

        // All productions for a symbol. Empty span = no rule, so the symbol copies itself.
        [[nodiscard]] std::span<const Production> ProductionsFor(char symbol) const noexcept;

        // Rescale each symbol's probabilities so they sum to exactly 1.
        // Hand-typed rules often sum to 0.99 or 1.01 (see Lecture 1, Common Failure Modes).
        void NormaliseProbabilities();

        // Human-readable listing, e.g. "F -> F[+F]F (p = 0.33)".
        [[nodiscard]] std::string Describe() const;

    private:
        // 256 slots indexed by the symbol's byte value: O(1) lookup, no hashing.
        std::array<std::vector<Production>, 256> Table{};

        // Cast through unsigned char: plain char may be signed, and a negative index is undefined behaviour.
        [[nodiscard]] static std::size_t Slot(char symbol) noexcept
        {
            return static_cast<std::size_t>(static_cast<unsigned char>(symbol));
        }
    };
}
