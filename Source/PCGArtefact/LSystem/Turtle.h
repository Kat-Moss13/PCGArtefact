// Turtle.h
// Week 3 Tutorial 1: L-System Engine
//
// Turtle interpretation (Prusinkiewicz, 1986): reads a finished L-system string
// and turns it into 2D line segments. The turtle knows nothing about rules,
// and the rewriting engine knows nothing about drawing.

#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace LSystem
{
    struct Vec2
    {
        double X = 0.0;
        double Y = 0.0;
    };

    struct Segment
    {
        Vec2 Start;
        Vec2 End;
        int  Depth = 0; // Branch depth: 0 = trunk, 1 = first branches, ...
    };

    struct TurtleSettings
    {
        double      StepLength            = 1.0;
        double      AngleDegrees          = 25.0;
        double      InitialHeadingDegrees = 90.0;
        double      LengthScalePerDepth   = 1.0;
        std::string DrawSymbols           = "F";
        std::string MoveSymbols           = "f";
    };

    struct TurtleResult
    {
        std::vector<Segment> Segments;
        int  MaxDepth = 0;
        bool Success  = true;
        std::string Error;
    };

    // STEP C1: check that every ']' has a matching '[' and none are left open.
    // Returns true if balanced; otherwise fills 'error' with a helpful message.
    [[nodiscard]] bool ValidateBrackets(std::string_view symbols, std::string& error);

    // STEP C2: interpret the symbols with the turtle commands
    //   F (draw symbols)  move forward and draw      f (move symbols)  move without drawing
    //   +  turn left by angle     -  turn right by angle     |  turn around (180 degrees)
    //   [  push state             ]  pop state
    // Any other symbol is ignored by the turtle.
    [[nodiscard]] TurtleResult InterpretTurtle(std::string_view symbols, const TurtleSettings& settings);
}
