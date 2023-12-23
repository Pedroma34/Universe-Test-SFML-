#include "pch.h"
#include "Planet.h"

Planet::Planet(int64_t l_seed) : Exists(false) {
	std::mt19937_64& rng = *SharedData::GetRNG();
	std::uniform_real_distribution<double> dist(0.0, 1.0);

	auto result = dist(rng);
	if(result > SharedData::GetStarPlanetProbability())
		return; // No planet

	Exists = true;
	Create();
}

Planet::Planet() {
	Exists = true;
	Create();
}

void Planet::Create() {
	PlanetShape.setRadius(1.f);
	PlanetShape.setFillColor(sf::Color::Green);
	PlanetShape.setOrigin(PlanetShape.getGlobalBounds().width / 2, PlanetShape.getGlobalBounds().height / 2);
}
