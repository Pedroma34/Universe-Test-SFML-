#pragma once
#include "pch.h"

enum class StarSize  { Small, Medium, Large, HyperLarge };
enum class StarColor { Red, Orange, White, Blue, Purple };

struct StarSystem {

    StarSystem(int64_t l_x, int64_t l_y);

    void SetStarPositionInSector(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow);

    sf::CircleShape StarShape;
    sf::RectangleShape StarSelectorShape;
    StarSize Size;
    StarColor Color;
    sf::Vector2f Position;
    bool HasStar;

    std::string GetStarSizeString()  const;
    std::string GetStarColorString() const;

private:
    void DetermineStarSize(std::mt19937_64& l_rng);
    void DetermineStarColor(std::mt19937_64& l_rng);

};