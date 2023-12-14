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
		Size = 4;
	else if (sizeRoll < (probabilitySmall + probabilityMedium))
		Size = 6;
	else
		Size = 9;
    StarShape.setRadius(Size);
    //Setting origin to middle
    StarShape.setOrigin(StarShape.getGlobalBounds().width / 2, StarShape.getGlobalBounds().height / 2);

    //Size of the debug relative to the size of the sector
    DebugStarSelectorShape.setSize(sf::Vector2f(SharedData::GetSectorSize(), SharedData::GetSectorSize()));
    DebugStarSelectorShape.setOrigin(DebugStarSelectorShape.getGlobalBounds().width / 2, DebugStarSelectorShape.getGlobalBounds().height / 2);
    DebugStarSelectorShape.setFillColor(sf::Color::Red);
    DebugStarSelectorShape.setOutlineColor(sf::Color::Red);
    DebugStarSelectorShape.setOutlineThickness(1);

    // Determine the color of the star
    double colorRoll = dist(rng);
    const double probabilityYellow = 0.10;
    const double probabilityBlue = 0.05; 
    const double probabilityOrange = 0.10;
    const double probabilityWhite = 0.10;
    const double probabilityRed = 0.65; 


    if (colorRoll < probabilityYellow)
        StarShape.setFillColor(sf::Color::Yellow);
    else if (colorRoll < (probabilityYellow + probabilityBlue))
        StarShape.setFillColor(sf::Color::Blue);
    else if (colorRoll < (probabilityYellow + probabilityBlue + probabilityOrange))
        StarShape.setFillColor(sf::Color(255, 165, 0));
    else if (colorRoll < (probabilityYellow + probabilityBlue + probabilityOrange + probabilityWhite))
        StarShape.setFillColor(sf::Color::White);
    else
        StarShape.setFillColor(sf::Color::Red);
}

void StarSystem::SetStarPositionInSector(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow) {
    //Variables
    auto& rng = *SharedData::GetRNG();
    const int64_t& sectorSize = SharedData::GetSectorSize();
    const auto& starGlobalBounds = StarShape.getGlobalBounds();
    const double& UserX = SharedData::GetUserX();
    const double& UserY = SharedData::GetUserY();
    double OffsetX = fmod(UserX, sectorSize);
    double OffsetY = fmod(UserY, sectorSize);

    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double positionRoll = dist(rng);
    const double probabilityTopLeft = 0.125;
    const double probabilityTopRight = 0.125;
    const double probabilityBottomLeft = 0.125;
    const double probabilityBottomRight = 0.125;
    const double probabilityCenterTop = 0.125;
    const double probabilityCenterLeft = 0.125;
    const double probabilityCenterRight = 0.125;
    const double probabilityCenter = 0.125;

    //Top left
    if (positionRoll < probabilityTopLeft) {
        StarShape.setPosition((l_row - l_startRow) * sectorSize + starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + starGlobalBounds.height - OffsetY);
    }

    //Top Right
    else if (positionRoll < (probabilityTopLeft + probabilityTopRight)) {
        StarShape.setPosition((l_row - l_startRow) * sectorSize + sectorSize - starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + starGlobalBounds.height - OffsetY);
    }

    //Bottom Left
    else if (positionRoll < (probabilityTopLeft + probabilityTopRight + probabilityBottomLeft)) {
        StarShape.setPosition((l_row - l_startRow) * sectorSize + starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize - starGlobalBounds.height - OffsetY);
    }

    //Bottom Right
    else if (positionRoll < (probabilityTopLeft + probabilityTopRight + probabilityBottomLeft + probabilityBottomRight)) {
        StarShape.setPosition((l_row - l_startRow) * sectorSize + sectorSize - starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize - starGlobalBounds.height - OffsetY);
    }

    //Center Top
    else if (positionRoll < (probabilityTopLeft + probabilityTopRight + probabilityBottomLeft + probabilityBottomRight + probabilityCenterTop)) {
        StarShape.setPosition((l_row - l_startRow) * sectorSize + sectorSize / 2 - OffsetX, (l_column - l_startColumn) * sectorSize + starGlobalBounds.height - OffsetY);
    }

    //Center Left
    else if (positionRoll < (probabilityTopLeft + probabilityTopRight + probabilityBottomLeft + probabilityBottomRight + probabilityCenterTop + probabilityCenterLeft)) {
        StarShape.setPosition((l_row - l_startRow) * sectorSize + starGlobalBounds.width + starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize / 2 - OffsetY);
    }

    //Center Right
    else if (positionRoll < (probabilityTopLeft + probabilityTopRight + probabilityBottomLeft + probabilityBottomRight + probabilityCenterTop + probabilityCenterLeft + probabilityCenterRight)) {
        StarShape.setPosition((l_row - l_startRow) * sectorSize + sectorSize - starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize / 2 - OffsetY);
    }

    //Center
    else {
        StarShape.setPosition((l_row - l_startRow) * sectorSize + sectorSize / 2 - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize / 2 - OffsetY);
    }
}

