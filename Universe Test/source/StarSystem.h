#pragma once
#include "pch.h"

struct StarSystem {

    StarSystem(int64_t l_x, int64_t l_y);

    void SetStarPositionInSector(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow);

    sf::CircleShape StarShape;
    sf::Color Color;
    float Size;
    bool HasStar;
    sf::CircleShape DebugStarSelectorShape;

private:
    void DetermineStarSize(std::mt19937_64& l_rng);
    void DetermineStarColor(std::mt19937_64& l_rng);

};