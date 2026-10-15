// LSystem.cpp
// Week 3 Tutorial 1: L-System Engine

#include "LSystem.h"
#include "DeterministicRandom.h"

#include <array>
#include <sstream>

namespace LSystem
{
    const std::string& ChooseSuccessor(std::span<const Production> options,
                                       std::uint64_t seed, int iteration,
                                       std::size_t position)
    {
        
        const double u = PcgRandom::ToUnitInterval(
            PcgRandom::HashCell(seed, iteration, static_cast<std::int32_t>(position)));
        double cumulative = 0.0;

        for (const Production& option : options)
        {
            cumulative += option.Probability;
            if (u < cumulative)
            {
                return option.Successor;
            }
        }
        return options.back().Successor;
    }

    std::string Rewrite(std::string_view current, const RuleSet& rules,
                        std::uint64_t seed, int iteration)
    {
        std::string next;

        // TODO (Step B1): Rewrite every symbol of 'current' into 'next'.
        //   For each symbol (keep track of its position):
        //     - no productions      -> append the symbol itself
        //     - one production      -> append its Successor
        //     - several productions -> append ChooseSuccessor(productions, seed, iteration, position)
        //   Read only from 'current' and write only to 'next'.
        
        next.reserve(current.size() * 2);

        for (std::size_t position = 0; position < current.size(); ++position)
        {
            const char symbol = current[position];
            const auto productions = rules.ProductionsFor(symbol);

            if (productions.empty())
            {
                next += symbol;
            }
            else if (productions.size() == 1)
            {
                next += productions[0].Successor;
            }
            else {
                next += ChooseSuccessor(productions, seed, iteration, position);
            }
        }

        return next;
    }

    double PredictLength(std::string_view axiom, const RuleSet& rules, int iterations)
    {
        // counts[c] = expected number of symbol c in the current generation.
        std::array<double, 256> counts{};
        for (const char symbol : axiom)
        {
            counts[static_cast<unsigned char>(symbol)] += 1.0;
        }

        for (int i = 0; i < iterations; ++i)
        {
            std::array<double, 256> next{};
            for (std::size_t slot = 0; slot < counts.size(); ++slot)
            {
                if (counts[slot] == 0.0)
                {
                    continue;
                }
                const auto productions = rules.ProductionsFor(static_cast<char>(slot));
                if (productions.empty())
                {
                    next[slot] += counts[slot];
                    continue;
                }
                for (const Production& p : productions)
                {
                    const double weight = counts[slot] * (productions.size() == 1 ? 1.0 : p.Probability);
                    for (const char produced : p.Successor)
                    {
                        next[static_cast<unsigned char>(produced)] += weight;
                    }
                }
            }
            counts = next;
        }

        double total = 0.0;
        for (const double c : counts)
        {
            total += c;
        }
        return total;
    }

    GenerationResult Generate(const LSystemDefinition& definition, const GenerationSettings& settings)
    {
        GenerationResult result;
        result.Symbols = definition.Axiom;
        result.LengthPerIteration.push_back(result.Symbols.size());

        for (int iteration = 0; iteration < definition.Iterations; ++iteration)
        {
            // Predict the next length BEFORE allocating it (x1.5 margin for stochastic rules).
            const double predicted = PredictLength(result.Symbols, definition.Rules, 1);
            if (predicted * 1.5 > static_cast<double>(settings.MaxSymbols))
            {
                std::ostringstream message;
                message << "Stopped before iteration " << (iteration + 1) << ": about "
                        << static_cast<std::size_t>(predicted) << " symbols predicted, cap is "
                        << settings.MaxSymbols << ". Reduce Iterations.";
                result.Success = false;
                result.Error = message.str();
                return result;
            }

            result.Symbols = Rewrite(result.Symbols, definition.Rules, settings.Seed, iteration);
            result.LengthPerIteration.push_back(result.Symbols.size());
            result.IterationsCompleted = iteration + 1;
        }

        return result;
    }
}
