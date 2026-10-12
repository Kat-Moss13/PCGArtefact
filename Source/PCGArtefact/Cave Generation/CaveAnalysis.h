// CaveAnalysis.h
// Week 2 Tutorial 2: Hybrid Noise + CA
//
// Region labelling (flood fill), repair by culling, and the metrics and rubric
// used to compare seeding strategies.

#pragma once

#include "CaveGrid.h"

#include <vector>

namespace HybridCave
{
    // ---------------------------------------------------------------- Regions

    struct RegionLabels
    {
        std::vector<int> Labels;      // Per cell: region index, or -1 for walls.
        std::vector<int> RegionSizes; // Floor cells in each region.
        int LargestRegion = -1;       // Index of the biggest region, or -1 if no floor.
    };

    // Label 4-connected floor regions using an iterative breadth-first flood fill.
    [[nodiscard]] RegionLabels LabelFloorRegions(const CaveGrid& grid);

    // Turn every floor cell outside the largest region into wall.
    // Returns the number of floor cells removed.
    int CullToLargestRegion(CaveGrid& grid, const RegionLabels& regions);

    // ---------------------------------------------------------------- Metrics

    struct CaveMetrics
    {
        // Measured BEFORE repair (raw generator quality).
        int    RegionCount             = 0;
        double LargestShareBeforeCull  = 0.0; // Largest region / all floor.

        // Measured AFTER repair (what the player gets).
        double OpenRatio               = 0.0; // Floor cells / all cells.
        double FloorLostToCulling      = 0.0; // Removed floor / floor before culling.
        double ChamberRatio            = 0.0; // Floor cells deep inside open space / floor.
        double BoundaryRoughness       = 0.0; // Perimeter^2 / (16 * area); 1.0 = a square.

        // Process measures.
        int    FinalPassChanges        = 0;
        double GenerationMilliseconds  = 0.0;
    };

    // Count floor-to-wall edges between 4-neighbours (the cave's perimeter, in cell edges).
    // Out-of-bounds counts as wall.
    [[nodiscard]] int CountBoundaryEdges(const CaveGrid& grid);

    // STEP C1: fraction of floor cells whose whole (2r+1) x (2r+1) window is floor and in bounds.
    [[nodiscard]] double ComputeChamberRatio(const CaveGrid& grid, int radius);

    // ---------------------------------------------------------------- Rubric

    struct RubricThresholds
    {
        // Connectivity (before repair): largest region share.
        double ConnectivityGood       = 0.95;
        double ConnectivityAcceptable = 0.85;

        // Waste: fraction of floor destroyed by culling.
        double WasteGood              = 0.05;
        double WasteAcceptable        = 0.15;

        // Openness (after repair): target band for the open ratio.
        double OpenRatioMin           = 0.40;
        double OpenRatioMax           = 0.60;
        double OpenRatioTolerance     = 0.05;

        // Chambers: target chamber ratio.
        double ChamberTarget          = 0.35;
    };

    struct RubricScore
    {
        int  Connectivity = 0; // 0, 1 or 2
        int  Waste        = 0;
        int  Openness     = 0;
        int  Chambers     = 0;
        int  Total        = 0; // 0 to 8
        bool Pass         = false;
    };

    // STEP D1: score one generated cave against the rubric.
    [[nodiscard]] RubricScore EvaluateAgainstRubric(const CaveMetrics& metrics, const RubricThresholds& thresholds) noexcept;
}
