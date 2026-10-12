// DeterministicRandom.h
// Week 2 Tutorial 2: Hybrid Noise + CA
//
// Counter-based (hash) randomness, as introduced in Week 1.
// Instead of advancing a stateful generator, every random value is a pure
// function of (seed, x, y). The same cell always receives the same number,
// regardless of the order cells are visited or which strategy is running.
//
// We deliberately avoid std::mt19937 + std::uniform_real_distribution here:
// the distribution algorithms are implementation-defined, so the same seed
// can give different results on MSVC, GCC and Clang.

#pragma once

#include <cstdint>

namespace HybridCave
{
    // SplitMix64 finaliser: a fast, well-mixed 64-bit hash.
    [[nodiscard]] constexpr std::uint64_t Mix64(std::uint64_t z) noexcept
    {
        z += 0x9E3779B97F4A7C15ull;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
        return z ^ (z >> 31);
    }

    // Hash a seed and a 2D integer coordinate into 64 random-looking bits.
    [[nodiscard]] constexpr std::uint64_t HashCell(std::uint64_t seed, std::int32_t x, std::int32_t y) noexcept
    {
        std::uint64_t h = Mix64(seed);
        h = Mix64(h ^ static_cast<std::uint64_t>(static_cast<std::uint32_t>(x)));
        h = Mix64(h ^ (static_cast<std::uint64_t>(static_cast<std::uint32_t>(y)) << 32));
        return h;
    }

    // Convert 64 random bits to a double in [0, 1) using the top 53 bits.
    [[nodiscard]] constexpr double ToUnitInterval(std::uint64_t bits) noexcept
    {
        return static_cast<double>(bits >> 11) * (1.0 / 9007199254740992.0);
    }

    // Salts separate independent random "streams" derived from one seed.
    inline constexpr std::uint64_t CellDrawSalt  = 0x0000000000000000ull;
    inline constexpr std::uint64_t NoiseSeedSalt = 0xA5A5F00DCAFE1234ull;
}
