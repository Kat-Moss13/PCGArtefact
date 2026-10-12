// CellularAutomaton.cpp
// Week 2 Tutorial 2: Hybrid Noise + CA

#include "CellularAutomaton.h"
#include "DeterministicRandom.h"

#include <algorithm>
#include <utility>

namespace HybridCave
{
    const char* ToString(SeedStrategy strategy) noexcept
    {
        switch (strategy)
        {
            case SeedStrategy::Uniform:     return "Uniform";
            case SeedStrategy::NoiseSeeded: return "NoiseSeeded";
        }
        return "Unknown";
    }

    double RemapNoiseToProbability(double fbmValue, const NoiseSeedSettings& settings) noexcept
    {
        // TODO (Step B1): Remap the fBm value into a wall probability.
        //   1. Map fbmValue from [-1, 1] into [0, 1].
        const double normalised = 0.5 * (fbmValue + 1.0);
        //   2. Centre on 0.5, multiply by settings.Contrast, add settings.Bias.
        const double probability = settings.Bias + settings.Contrast * (normalised - 0.5);
        //   3. Clamp between settings.MinProbability and settings.MaxProbability.
        
        return std::clamp(probability, settings.MinProbability, settings.MaxProbability);
    }

    CaveGrid InitialiseUniform(int width, int height, std::uint64_t seed, double wallProbability)
    {
        CaveGrid grid(width, height, Floor);

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                // One independent draw per cell, from the shared cell-draw stream.
                const double draw = ToUnitInterval(HashCell(seed ^ CellDrawSalt, x, y));
                grid.Set(x, y, draw < wallProbability ? Wall : Floor);
            }
        }

        return grid;
    }

    CaveGrid InitialiseFromNoise(int width, int height, std::uint64_t seed,
                                 const NoiseSeedSettings& settings)
    {
        CaveGrid grid(width, height, Floor);
        const PerlinNoise2D noise(Mix64(seed ^ NoiseSeedSalt));
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const double fbm = noise.Fbm(static_cast<double>(x), static_cast<double>(y), settings.Fbm);
                const double wallProbability = RemapNoiseToProbability(fbm, settings);

                const double draw = ToUnitInterval(HashCell(seed ^ CellDrawSalt, x, y));
                grid.Set(x, y, draw < wallProbability ? Wall : Floor);
            }
        }

        return grid;
    }

    std::vector<double> BuildProbabilityField(int width, int height, std::uint64_t seed, const NoiseSeedSettings& settings)
    {
        std::vector<double> field(static_cast<std::size_t>(width) * static_cast<std::size_t>(height));
        const PerlinNoise2D noise(Mix64(seed ^ NoiseSeedSalt));

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const double fbm = noise.Fbm(static_cast<double>(x), static_cast<double>(y), settings.Fbm);
                field[static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x)]
                    = RemapNoiseToProbability(fbm, settings);
            }
        }

        return field;
    }

    int MajorityPass(const CaveGrid& source, CaveGrid& destination, bool outOfBoundsIsWall)
    {
        const std::uint8_t outsideValue = outOfBoundsIsWall ? Wall : Floor;
        int changed = 0;

        for (int y = 0; y < source.Height; ++y)
        {
            for (int x = 0; x < source.Width; ++x)
            {
                int walls = 0;

                // The window includes the cell itself, so ">= 5 of 9" is a strict majority.
                for (int dy = -1; dy <= 1; ++dy)
                {
                    for (int dx = -1; dx <= 1; ++dx)
                    {
                        const int nx = x + dx;
                        const int ny = y + dy;
                        walls += source.InBounds(nx, ny) ? source.Get(nx, ny) : outsideValue;
                    }
                }

                const std::uint8_t next = (walls >= 5) ? Wall : Floor;
                if (next != source.Get(x, y))
                {
                    ++changed;
                }
                destination.Set(x, y, next);
            }
        }

        return changed;
    }

    std::vector<int> RunMajorityPasses(CaveGrid& grid, const CaSettings& settings)
    {
        std::vector<int> changesPerPass;
        changesPerPass.reserve(static_cast<std::size_t>(std::max(settings.MaxPasses, 0)));

        // Allocate the second buffer once and swap, rather than copying every pass.
        CaveGrid scratch(grid.Width, grid.Height, Floor);
        const int stableThreshold = static_cast<int>(settings.StableFraction * grid.CellCount());

        for (int pass = 0; pass < settings.MaxPasses; ++pass)
        {
            const int changed = MajorityPass(grid, scratch, settings.OutOfBoundsIsWall);
            std::swap(grid.Cells, scratch.Cells);
            changesPerPass.push_back(changed);

            if (settings.StopWhenStable && changed <= stableThreshold)
            {
                break;
            }
        }

        return changesPerPass;
    }
}
