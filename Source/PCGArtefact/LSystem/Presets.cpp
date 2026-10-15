// Presets.cpp
// Week 3 Tutorial 1: L-System Engine

#include "Presets.h"

namespace LSystem::Presets
{
    LSystemDefinition Algae()
    {
        LSystemDefinition d;
        d.Name = "Algae";
        d.Axiom = "A";
        d.Rules.Add('A', "AB");
        d.Rules.Add('B', "A");
        d.Iterations = 7;
        return d;
    }

    LSystemDefinition KochCurve()
    {
        LSystemDefinition d;
        d.Name = "KochCurve";
        d.Axiom = "F";
        d.Rules.Add('F', "F+F--F+F");
        d.Iterations = 4;
        d.AngleDegrees = 60.0;
        d.InitialHeadingDegrees = 0.0;
        return d;
    }

    LSystemDefinition DragonCurve()
    {
        LSystemDefinition d;
        d.Name = "DragonCurve";
        d.Axiom = "FX";
        d.Rules.Add('X', "X+YF+");
        d.Rules.Add('Y', "-FX-Y");
        d.Iterations = 10;
        d.AngleDegrees = 90.0;
        d.InitialHeadingDegrees = 0.0;
        return d;
    }

    LSystemDefinition BracketTree()
    {
        LSystemDefinition d;
        d.Name = "BracketTree";
        d.Axiom = "F";
        d.Rules.Add('F', "F[+F]F[-F]F");
        d.Iterations = 4;
        d.AngleDegrees = 25.7;
        return d;
    }

    LSystemDefinition FractalPlant()
    {
        LSystemDefinition d;
        d.Name = "FractalPlant";
        d.Axiom = "X";
        d.Rules.Add('X', "F-[[X]+X]+F[+FX]-X");
        d.Rules.Add('F', "FF");
        d.Iterations = 6;
        d.AngleDegrees = 22.5;
        d.InitialHeadingDegrees = 75.0;
        return d;
    }

    LSystemDefinition StochasticBush()
    {
        LSystemDefinition d;
        d.Name = "StochasticBush";
        d.Axiom = "F";
        d.Rules.Add('F', "F[+F]F[-F]F", 1.0 / 3.0);
        d.Rules.Add('F', "F[+F]F",      1.0 / 3.0);
        d.Rules.Add('F', "F[-F]F",      1.0 / 3.0);
        d.Iterations = 5;
        d.AngleDegrees = 25.7;
        return d;
    }

    LSystemDefinition MyPlant()
    {
        LSystemDefinition d;
        d.Name = "MyPlant";

        // TODO (Step D1): Design your own plant. Replace these placeholder rules.
        //   Ideas: a growth symbol (like X or A), several weighted productions,
        //   LengthScalePerDepth < 1 for tapering branches, a different angle.
        d.Axiom = "F";
        d.Rules.Add('F', "F[+F]F");
        d.Iterations = 4;
        d.AngleDegrees = 30.0;

        d.Rules.NormaliseProbabilities();
        return d;
    }

    std::vector<LSystemDefinition> AllDrawable()
    {
        return { KochCurve(), DragonCurve(), BracketTree(), FractalPlant(), StochasticBush(), MyPlant() };
    }
}
