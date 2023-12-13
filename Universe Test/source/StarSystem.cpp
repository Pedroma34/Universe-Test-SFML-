#include "pch.h"
#include "StarSystem.h"

StarSystem::StarSystem(int64_t l_x, int64_t l_y) : Color(sf::Color::Yellow), Size(5), HasStar(false) {
    
    std::mt19937_64& rng = *SharedData::GetRNG();
    const double& StarSystemProbability = SharedData::GetStarSystemProbability();

    rng.seed(SharedData::Get().GenerateSeed(l_x, l_y));
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // x% chance of a star
    if (dist(rng) > StarSystemProbability)
        return; // No star
    HasStar = true;

    // Determine the size of the star
    double sizeRoll = dist(rng);
    const double probabilitySmall = 0.40;
    const double probabilityMedium = 0.55; 
    const double probabilityLarge = 0.05;
    if(sizeRoll < probabilitySmall)
		Size = 3;
	else if (sizeRoll < (probabilitySmall + probabilityMedium))
		Size = 5;
	else
		Size = 9;
    StarShape.setRadius(Size);
    //Setting origin to middle
    StarShape.setOrigin(StarShape.getGlobalBounds().width / 2, StarShape.getGlobalBounds().height / 2);


    // Determine the color of the star
    double colorRoll = dist(rng);
    const double probabilityYellow = 0.40;
    const double probabilityBlue = 0.10; // Adjust this as needed
    const double probabilityRed = 0.50; // Adjust this as needed


    if (colorRoll < probabilityYellow)
        StarShape.setFillColor(sf::Color::Yellow);
    else if (colorRoll < (probabilityYellow + probabilityBlue))
        StarShape.setFillColor(sf::Color::Blue);
    else
        StarShape.setFillColor(sf::Color::Red);
}

