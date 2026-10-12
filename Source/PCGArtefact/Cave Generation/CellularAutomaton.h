// CellularAutomaton.h
// Week 2 Tutorial 2: Hybrid Noise + CA
//
// Initialisation strategies and the 4-5 (majority) cellular automaton.

#pragma once

#include "CaveGrid.h"
#include "PerlinNoise.h"

#include <cstdint>
#include <vector>

namespace HybridCave
{
    enum class SeedStrategy
    {
        Uniform,     // Every cell has the same wall probability (Tutorial 1 baseline).
        NoiseSeeded  // Wall probability varies across the map, driven by fBm noise.
    };

    [[nodiscard]] const char* ToString(SeedStrategy strategy) noexcept;

    struct CaSettings
    {
        int    MaxPasses          = 5;     // Hard cap on majority passes.
        bool   StopWhenStable     = false; // Stop early once few cells change.
        double StableFraction     = 0.001; // "Few" = this fraction of all cells.
        bool   OutOfBoundsIsWall  = true;  // Boundary policy: closed caves.
    };

    struct NoiseSeedSettings
    {
        FbmSettings Fbm;
        double Bias           = 0.45; // Wall probability where the noise is neutral.
        double Contrast       = 0.90; // How strongly the noise pushes probability up or down.
        double MinProbability = 0.05; // Clamp: never guarantee floor...
        double MaxProbability = 0.95; // ...or guarantee wall.
    };

    // STEP B1: convert one fBm sample in [-1, 1] into a wall probability.
    [[nodiscard]] double RemapNoiseToProbability(double fbmValue, const NoiseSeedSettings& settings) noexcept;

    // Baseline: each cell is wall with the same probability.
    [[nodiscard]] CaveGrid InitialiseUniform(int width, int height, std::uint64_t seed, double wallProbability);

    // STEP B2: each cell is wall with a probability taken from the noise field.
    [[nodiscard]] CaveGrid InitialiseFromNoise(int width, int height, std::uint64_t seed, const NoiseSeedSettings& settings);

    // Build the probability field on its own, for visualisation.
    [[nodiscard]] std::vector<double> BuildProbabilityField(int width, int height, std::uint64_t seed, const NoiseSeedSettings& settings);

    // One synchronous majority pass. Returns the number of cells that changed.
    // source and destination must be different grids of the same size.
    int MajorityPass(const CaveGrid& source, CaveGrid& destination, bool outOfBoundsIsWall);

    // Run up to settings.MaxPasses passes in place.
    // Returns the number of cells changed on each pass (useful for convergence plots).
    std::vector<int> RunMajorityPasses(CaveGrid& grid, const CaSettings& settings);
}
