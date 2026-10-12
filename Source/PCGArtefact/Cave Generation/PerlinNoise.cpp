// PerlinNoise.cpp
// Week 2 Tutorial 2: Hybrid Noise + CA

#include "PerlinNoise.h"
#include "DeterministicRandom.h"

#include <cmath>
#include <utility>

namespace HybridCave
{
    namespace
    {
        // Quintic fade curve 6t^5 - 15t^4 + 10t^3 (Perlin, 2002).
        // Its first and second derivatives are zero at t = 0 and t = 1,
        // which removes visible creases at lattice boundaries.
        [[nodiscard]] constexpr double PerlinFade(double t) noexcept
        {
            return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
        }

        [[nodiscard]] constexpr double PerlinLerp(double a, double b, double t) noexcept
        {
            return a + t * (b - a);
        }

        // Dot product of one of eight gradient directions with the offset (x, y).
        [[nodiscard]] constexpr double PerlinGradient(std::uint8_t hash, double x, double y) noexcept
        {
            switch (hash & 7u)
            {
                case 0:  return  x + y;
                case 1:  return -x + y;
                case 2:  return  x - y;
                case 3:  return -x - y;
                case 4:  return  x;
                case 5:  return -x;
                case 6:  return  y;
                default: return -y;
            }
        }
    }

    PerlinNoise2D::PerlinNoise2D(std::uint64_t seed)
    {
        for (int i = 0; i < 256; ++i)
        {
            Permutation[static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(i);
        }

        // Fisher-Yates shuffle driven by our own hash, so results are
        // identical on every compiler and platform.
        for (int i = 255; i > 0; --i)
        {
            const std::uint64_t r = Mix64(seed + static_cast<std::uint64_t>(i));
            const int j = static_cast<int>(r % static_cast<std::uint64_t>(i + 1));
            std::swap(Permutation[static_cast<std::size_t>(i)], Permutation[static_cast<std::size_t>(j)]);
        }

        for (int i = 0; i < 256; ++i)
        {
            Permutation[static_cast<std::size_t>(i + 256)] = Permutation[static_cast<std::size_t>(i)];
        }
    }

    double PerlinNoise2D::Noise(double x, double y) const noexcept
    {
        const double floorX = std::floor(x);
        const double floorY = std::floor(y);

        // Lattice cell coordinates wrapped into the permutation table.
        const int xi = static_cast<int>(floorX) & 255;
        const int yi = static_cast<int>(floorY) & 255;

        // Position within the lattice cell, in [0, 1).
        const double xf = x - floorX;
        const double yf = y - floorY;

        const double u = PerlinFade(xf);
        const double v = PerlinFade(yf);

        const auto P = [this](int index) { return Permutation[static_cast<std::size_t>(index)]; };

        const std::uint8_t aa = P(P(xi)     + yi);
        const std::uint8_t ab = P(P(xi)     + yi + 1);
        const std::uint8_t ba = P(P(xi + 1) + yi);
        const std::uint8_t bb = P(P(xi + 1) + yi + 1);

        const double bottom = PerlinLerp(PerlinGradient(aa, xf, yf),       PerlinGradient(ba, xf - 1.0, yf),       u);
        const double top    = PerlinLerp(PerlinGradient(ab, xf, yf - 1.0), PerlinGradient(bb, xf - 1.0, yf - 1.0), u);

        return PerlinLerp(bottom, top, v);
    }

    double PerlinNoise2D::Fbm(double x, double y, const FbmSettings& settings) const noexcept
    {
        double sum = 0.0;
        double amplitude = 1.0;
        double totalAmplitude = 0.0;
        double frequency = settings.Frequency;

        for (int octave = 0; octave < settings.Octaves; ++octave)
        {
            // Offset each octave so octaves do not share lattice points at the origin.
            const double offset = 17.13 * static_cast<double>(octave);
            sum += amplitude * Noise(x * frequency + offset, y * frequency + offset);
            totalAmplitude += amplitude;
            amplitude *= settings.Persistence;
            frequency *= settings.Lacunarity;
        }

        return (totalAmplitude > 0.0) ? sum / totalAmplitude : 0.0;
    }
}
