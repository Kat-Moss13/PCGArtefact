// PerlinNoise.h
// Week 2 Tutorial 2: Hybrid Noise + CA
//
// A compact, seeded 2D gradient (Perlin) noise with fractal Brownian motion.
// You may replace this with your own Week 1 implementation, provided that
// Fbm() returns values in approximately [-1, 1].

#pragma once

#include <array>
#include <cstdint>

namespace HybridCave
{
    struct FbmSettings
    {
        int    Octaves     = 4;
        double Frequency   = 0.04; // Noise cycles per cell. Lower = larger features.
        double Lacunarity  = 2.0;  // Frequency multiplier per octave.
        double Persistence = 0.5;  // Amplitude multiplier per octave.
    };

    class PerlinNoise2D
    {
    public:
        explicit PerlinNoise2D(std::uint64_t seed);

        // Single octave of gradient noise, approximately in [-1, 1].
        [[nodiscard]] double Noise(double x, double y) const noexcept;

        // Sum of octaves, normalised by total amplitude, approximately in [-1, 1].
        [[nodiscard]] double Fbm(double x, double y, const FbmSettings& settings) const noexcept;

    private:
        // Permutation table duplicated to 512 entries to avoid index wrapping.
        std::array<std::uint8_t, 512> Permutation{};
    };
}
