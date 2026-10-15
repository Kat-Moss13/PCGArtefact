// Turtle.cpp
// Week 3 Tutorial 1: L-System Engine

#include "Turtle.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <numbers>
#include <sstream>

namespace LSystem
{

    bool ValidateBrackets(std::string_view symbols, std::string& error)
    {
        // A single counter is enough: this is the pushdown-automaton idea from
        // Lecture 1, with the stack reduced to its height.
        long long depth = 0;
        for (std::size_t i = 0; i < symbols.size(); ++i)
        {
            if (symbols[i] == '[')
            {
                ++depth;
            }
            else if (symbols[i] == ']')
            {
                --depth;
                if (depth < 0)
                {
                    std::ostringstream message;
                    message << "Unmatched ']' at position " << i << '.';
                    error = message.str();
                    return false;
                }
            }
        }

        if (depth != 0)
        {
            std::ostringstream message;
            message << depth << " '[' left unclosed at the end of the string.";
            error = message.str();
            return false;
        }

        error.clear();
        return true;
    }

    TurtleResult InterpretTurtle(std::string_view symbols, const TurtleSettings& settings)
    {
        TurtleResult result;

        std::array<bool, 256> draws{};
        std::array<bool, 256> moves{};
        for (const char c : settings.DrawSymbols) { draws[static_cast<unsigned char>(c)] = true; }
        for (const char c : settings.MoveSymbols) { moves[static_cast<unsigned char>(c)] = true; }
        struct State
        {
            Vec2 Position;
            double HeadingDegrees = 0.0;
            int Depth = 0;
        };

        State turtle;
        turtle.HeadingDegrees = settings.InitialHeadingDegrees;
        std::vector<State> stack;

        constexpr double DegreesToRadians = std::numbers::pi / 180.0;

        for (std::size_t i = 0; i < symbols.size(); ++i)
        {
            const unsigned char c = static_cast<unsigned char>(symbols[i]);
            if (draws[c] || moves[c])
            {
                // Deeper branches can be shorter: step scale^depth.
                const double step = settings.StepLength * std::pow(settings.LengthScalePerDepth, turtle.Depth);
                const double radians = turtle.HeadingDegrees * DegreesToRadians;
                const Vec2 end{ turtle.Position.X + step * std::cos(radians),
                turtle.Position.Y + step * std::sin(radians) };

                if (draws[c])
                {
                    result.Segments.push_back(Segment{ turtle.Position, end, turtle.Depth });
                }
                turtle.Position = end;
            }

            else if (c == '+')
            {
                turtle.HeadingDegrees += settings.AngleDegrees; // Left (anticlockwise).
            }

            else if (c == '-')
            {
                turtle.HeadingDegrees -= settings.AngleDegrees; // Right (clockwise).
            }

            else if (c == '|')
            {
                turtle.HeadingDegrees += 180.0;
            }
            else if (c == '[')
            {
                stack.push_back(turtle);
                ++turtle.Depth;
                result.MaxDepth = std::max(result.MaxDepth, turtle.Depth);
            }

            else if (c == ']')
            {
                if (stack.empty())
                {
                    std::ostringstream message;
                    message << "Pop with an empty stack at position " << i << '.'; result.Success = false;
                    result.Error = message.str();
                    return result;
                }
                turtle = stack.back(); // Restores position, heading AND depth. 
                stack.pop_back();
            }
        }
        return result;
    }
}
