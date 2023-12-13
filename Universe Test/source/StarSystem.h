#pragma once
#include "pch.h"

struct StarSystem {

    StarSystem(int64_t l_x, int64_t l_y);

    sf::CircleShape StarShape;
    sf::Color Color;
    float Size;
    bool HasStar;

};