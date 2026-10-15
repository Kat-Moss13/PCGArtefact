// RuleSet.cpp
// Week 3 Tutorial 1: L-System Engine

#include "RuleSet.h"

#include <iomanip>
#include <sstream>
#include <utility>

namespace LSystem
{
    void RuleSet::Add(char predecessor, std::string successor, double probability)
    {
        Table[Slot(predecessor)].push_back(Production{ std::move(successor), probability });
    }

    std::span<const Production> RuleSet::ProductionsFor(char symbol) const noexcept
    {
        return Table[Slot(symbol)];
    }

    void RuleSet::NormaliseProbabilities()
    {
        for (std::vector<Production>& productions : Table)
        {
            double total = 0.0;
            for (const Production& p : productions)
            {
                total += p.Probability;
            }
            if (total <= 0.0)
            {
                continue;
            }
            for (Production& p : productions)
            {
                p.Probability /= total;
            }
        }
    }

    std::string RuleSet::Describe() const
    {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2);

        for (std::size_t slot = 0; slot < Table.size(); ++slot)
        {
            for (const Production& p : Table[slot])
            {
                out << "    " << static_cast<char>(slot) << " -> " << p.Successor;
                if (Table[slot].size() > 1)
                {
                    out << "   (p = " << p.Probability << ")";
                }
                out << '\n';
            }
        }
        return out.str();
    }
}
