// CaveGrid.h
// Week 2 Tutorial 2: Hybrid Noise + CA
//
// A minimal 2D grid of cells stored as one flat, row-major array.
// Row-major means index = y * Width + x, which keeps neighbouring cells
// close together in memory and is friendly to the CPU cache.

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace HybridCave
{
    // Cell values. A wall is 1 so that summing a window counts walls directly.
    inline constexpr std::uint8_t Wall  = 1;
    inline constexpr std::uint8_t Floor = 0;

    struct CaveGrid
    {
        int Width  = 0;
        int Height = 0;
        std::vector<std::uint8_t> Cells;

        CaveGrid() = default;

        CaveGrid(int width, int height, std::uint8_t fill = Wall)
            : Width(width)
            , Height(height)
            , Cells(static_cast<std::size_t>(width) * static_cast<std::size_t>(height), fill)
        {
        }

        [[nodiscard]] bool InBounds(int x, int y) const noexcept
        {
            return x >= 0 && y >= 0 && x < Width && y < Height;
        }

        [[nodiscard]] std::size_t Index(int x, int y) const noexcept
        {
            return static_cast<std::size_t>(y) * static_cast<std::size_t>(Width)
                 + static_cast<std::size_t>(x);
        }

        [[nodiscard]] std::uint8_t Get(int x, int y) const noexcept { return Cells[Index(x, y)]; }
        void Set(int x, int y, std::uint8_t value) noexcept { Cells[Index(x, y)] = value; }

        [[nodiscard]] int CellCount() const noexcept { return Width * Height; }
    };
}
