#pragma once
#include "pch.h"

struct Planet {
	Planet(int64_t l_seed); //Create based on seed
	Planet(); //Create when called

	sf::CircleShape PlanetShape;
	float Size;
	bool Exists;
	
private:
	void Create();
};