#pragma once
#include "pch.h"

enum class StarSize  { Small, Medium, Large, HyperLarge };
enum class StarColor { Red, Orange, White, Blue, Purple };
enum class StarPositionInSector { TopLeft, TopRight, BottomLeft, BottomRight, CenterTop, CenterLeft, CenterRight, Center };

struct Planet;

struct StarSystem {

    StarSystem(int64_t l_x, int64_t l_y);

    void SetStarPositionInSector(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow);

    sf::CircleShape StarShape;
    sf::RectangleShape StarSelectorShape;
    StarSize Size;
    StarColor Color;
    StarPositionInSector PositionInSector;
    sf::Vector2f Position;
    std::vector<std::shared_ptr<Planet>> Planets;
    bool HasStar;
    bool HasPlanet;
    float ChanceForMultiplePlanets;

    std::string GetStarSizeString()  const;
    std::string GetStarColorString() const;

private:
    void DetermineStarSize(std::mt19937_64& l_rng);
    void DetermineStarColor(std::mt19937_64& l_rng);
    void DetermineStarPosition();
    void GeneratePlanet(int64_t l_seed);
};