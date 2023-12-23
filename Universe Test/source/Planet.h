#pragma once
#include "pch.h"

struct Planet {
	Planet(int64_t l_seed);

	sf::CircleShape PlanetShape;
	float Size;
	bool Exists;
};