// CaveAnalysis.cpp
// Week 2 Tutorial 2: Hybrid Noise + CA

#include "CaveAnalysis.h"

#include <array>
#include <queue>
#include <utility>

namespace HybridCave
{
    RegionLabels LabelFloorRegions(const CaveGrid& grid)
    {
        RegionLabels result;
        result.Labels.assign(grid.Cells.size(), -1);

        // 4-connectivity: the player cannot squeeze diagonally between two walls.
        static constexpr std::array<std::pair<int, int>, 4> Offsets{ { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } } };

        // An explicit queue avoids the stack overflow a recursive fill hits on large caves.
        std::queue<std::pair<int, int>> frontier;

        for (int y = 0; y < grid.Height; ++y)
        {
            for (int x = 0; x < grid.Width; ++x)
            {
                if (grid.Get(x, y) != Floor || result.Labels[grid.Index(x, y)] != -1)
                {
                    continue;
                }

                const int regionIndex = static_cast<int>(result.RegionSizes.size());
                int regionSize = 0;

                result.Labels[grid.Index(x, y)] = regionIndex; // Label on push, not on pop.
                frontier.emplace(x, y);

                while (!frontier.empty())
                {
                    const auto [cx, cy] = frontier.front();
                    frontier.pop();
                    ++regionSize;

                    for (const auto& [ox, oy] : Offsets)
                    {
                        const int nx = cx + ox;
                        const int ny = cy + oy;
                        if (grid.InBounds(nx, ny)
                            && grid.Get(nx, ny) == Floor
                            && result.Labels[grid.Index(nx, ny)] == -1)
                        {
                            result.Labels[grid.Index(nx, ny)] = regionIndex;
                            frontier.emplace(nx, ny);
                        }
                    }
                }

                result.RegionSizes.push_back(regionSize);
                if (result.LargestRegion == -1 || regionSize > result.RegionSizes[static_cast<std::size_t>(result.LargestRegion)])
                {
                    result.LargestRegion = regionIndex;
                }
            }
        }

        return result;
    }

    int CullToLargestRegion(CaveGrid& grid, const RegionLabels& regions)
    {
        int removed = 0;
        for (std::size_t i = 0; i < grid.Cells.size(); ++i)
        {
            if (grid.Cells[i] == Floor && regions.Labels[i] != regions.LargestRegion)
            {
                grid.Cells[i] = Wall;
                ++removed;
            }
        }
        return removed;
    }

    int CountBoundaryEdges(const CaveGrid& grid)
    {
        int edges = 0;
        static constexpr std::array<std::pair<int, int>, 4> Offsets{ { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } } };

        for (int y = 0; y < grid.Height; ++y)
        {
            for (int x = 0; x < grid.Width; ++x)
            {
                if (grid.Get(x, y) != Floor)
                {
                    continue;
                }
                for (const auto& [ox, oy] : Offsets)
                {
                    const int nx = x + ox;
                    const int ny = y + oy;
                    if (!grid.InBounds(nx, ny) || grid.Get(nx, ny) == Wall)
                    {
                        ++edges;
                    }
                }
            }
        }
        return edges;
    }

    double ComputeChamberRatio(const CaveGrid& grid, int radius)
    {
        int floorCells = 0;
        int chamberCells = 0;

        for (int y = 0; y < grid.Height; ++y)
        {
            for (int x = 0; x < grid.Width; ++x)
            {
                if (grid.Get(x, y) != Floor)
                {
                    continue;
                }
                ++floorCells;

                bool allFloor = true;
                for (int dy = -radius; dy <= radius && allFloor; ++dy)
                {
                    for (int dx = -radius; dx <= radius; ++dx) 
                    {
                        const int nx = x + dx;
                        const int ny = y + dy;
                        if (!grid.InBounds(nx, ny) || grid.Get(nx, ny) == Wall)
                        {
                            allFloor = false;
                            break;
                        }
                    }
                }

                if (allFloor)
                {
                    ++chamberCells;
                }
            }
        }
        return floorCells > 0 ? static_cast<double>(chamberCells) / static_cast<double>(floorCells) : 0.0;
    }

    RubricScore EvaluateAgainstRubric(const CaveMetrics& metrics, const RubricThresholds& thresholds) noexcept
    {
        RubricScore score;

        if (metrics.LargestShareBeforeCull >= thresholds.ConnectivityGood) { score.Connectivity = 2; }
        else if (metrics.LargestShareBeforeCull >= thresholds.ConnectivityAcceptable) { score.Connectivity = 1; }

        if (metrics.FloorLostToCulling <= thresholds.WasteGood) { score.Waste = 2; }
        else if (metrics.FloorLostToCulling <= thresholds.WasteAcceptable) { score.Waste = 1; }

        const double open = metrics.OpenRatio;

        if (open >= thresholds.OpenRatioMin && open <= thresholds.OpenRatioMax) { score.Openness = 2; }
        else if (open >= thresholds.OpenRatioMin - thresholds.OpenRatioTolerance && open <= thresholds.OpenRatioMax + thresholds.OpenRatioTolerance) { score.Openness = 1; }

        if (metrics.ChamberRatio >= thresholds.ChamberTarget) { score.Chambers = 2; }
        else if (metrics.ChamberRatio >= 0.5 * thresholds.ChamberTarget) { score.Chambers = 1; }

        score.Total = score.Connectivity + score.Waste + score.Openness + score.Chambers;

        const bool noZeros = score.Connectivity > 0 && score.Waste > 0 && score.Openness > 0 && score.Chambers > 0;
        score.Pass = noZeros && score.Total >= 6;

        return score;
    }
}
