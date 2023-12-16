#include "pch.h"
#include "StarSystem.h"

StarSystem::StarSystem(int64_t l_x, int64_t l_y) : Color(StarColor::White), Size(StarSize::Medium), HasStar(false) {
    
    std::mt19937_64& rng = *SharedData::GetRNG();
    const double& StarSystemProbability = SharedData::GetStarSystemProbability();

    rng.seed(SharedData::Get().GenerateSeed(l_x, l_y));
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // x% chance of a star
    if (dist(rng) > StarSystemProbability)
        return; // No star
    HasStar = true;

    DetermineStarSize(rng);
    DetermineStarColor(rng);
}

void StarSystem::SetStarPositionInSector(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow) {
    //Variables
    auto& rng = *SharedData::GetRNG();
    const int64_t& sectorSize = SharedData::GetSectorSize();
    const auto& starGlobalBounds = StarShape.getGlobalBounds();
    const auto& selectorGlobalBounds = DebugStarSelectorShape.getGlobalBounds();
    const double& UserX = SharedData::GetUserX();
    const double& UserY = SharedData::GetUserY();
    double OffsetX = fmod(UserX, sectorSize);
    double OffsetY = fmod(UserY, sectorSize);

    //Chance and position
    std::vector<std::pair<double, sf::Vector2f>> positions{
        /*Top Left     */ {0.125, sf::Vector2f((l_row - l_startRow) * sectorSize + starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + starGlobalBounds.height - OffsetY)                           }, 
        /*Top Right    */ {0.125, sf::Vector2f((l_row - l_startRow) * sectorSize + sectorSize - starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + starGlobalBounds.height - OffsetY)              }, 
        /*Bottom Left  */ {0.125, sf::Vector2f((l_row - l_startRow) * sectorSize + starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize - starGlobalBounds.height - OffsetY)              }, 
        /*Bottom Right */ {0.125, sf::Vector2f((l_row - l_startRow) * sectorSize + sectorSize - starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize - starGlobalBounds.height - OffsetY) }, 
        /*Center Top   */ {0.125, sf::Vector2f((l_row - l_startRow) * sectorSize + sectorSize / 2 - OffsetX, (l_column - l_startColumn) * sectorSize + starGlobalBounds.height - OffsetY)                                   }, 
        /*Center Left  */ {0.125, sf::Vector2f((l_row - l_startRow) * sectorSize + starGlobalBounds.width + starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize / 2 - OffsetY)           }, 
        /*Center Right */ {0.125, sf::Vector2f((l_row - l_startRow) * sectorSize + sectorSize - starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize / 2 - OffsetY)                       }, 
        /*Center       */ {0.125, sf::Vector2f((l_row - l_startRow) * sectorSize + sectorSize / 2 - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize / 2 - OffsetY)                                            }, 
	};

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double positionRoll = dist(rng);

    double cumulativeProbability = 0.0;

    for (auto& position : positions) {

        cumulativeProbability += position.first;

        if (positionRoll > cumulativeProbability)
			continue;

        StarShape.setPosition(position.second);
        Position = position.second;
        return;
    }

}

void StarSystem::DetermineStarSize(std::mt19937_64& l_rng) {

    float medium = 6.0f;
    float large = 9.0f;
    float small = 4.0f;
    float hyperLarge = 17.0f;

    //Chance and Size
    std::vector<std::pair<double, float>> sizeProbabilities = {
        {0.55, medium     }, //55.0%
        {0.25, large      }, //25.0%  
        {0.18, small      }, //18.0% 
        {0.02, hyperLarge }  //02.0%  
    };

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double sizeRoll = dist(l_rng);

    double cumulativeProbability = 0.0;

    for (auto& sizeProbability : sizeProbabilities) {

		cumulativeProbability += sizeProbability.first;

		if (sizeRoll > cumulativeProbability)
			continue;

        //Checking and setting StarSize (6 is medium, 9 is large, 4 is small, 17 is hyper large)
        if (sizeProbability.second == medium)
			Size = StarSize::Medium;
		else if (sizeProbability.second == large)
			Size = StarSize::Large;
		else if (sizeProbability.second == small)
			Size = StarSize::Small;
		else if (sizeProbability.second == hyperLarge)
			Size = StarSize::HyperLarge;
        else
            throw std::runtime_error("StarSystem::DetermineStarSize() - Invalid StarSize");

		StarShape.setRadius(sizeProbability.second);
		//Setting origin to middle
		StarShape.setOrigin(StarShape.getGlobalBounds().width / 2, StarShape.getGlobalBounds().height / 2);

		return;
	}
}

void StarSystem::DetermineStarColor(std::mt19937_64& l_rng) {
    
    //Chance and Color
    std::vector<std::pair<double, sf::Color>> colorProbabilities = {
		{ 0.525, sf::Color::Red                      }, //52.5%
		{ 0.325, sf::Color(255, 69, 0)    /*Orange*/ }, //32.5%
        { 0.075, sf::Color(255, 255, 159) /*White*/  }, //07.5%
		{ 0.050, sf::Color::Blue                     }, //05.0%
        { 0.025, sf::Color(50, 23, 77)   /*Purple*/  }, //02.5%
	};

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double colorRoll = dist(l_rng);

    double cumulativeProbability = 0.0;

    for (auto& colorProbability : colorProbabilities) {

		cumulativeProbability += colorProbability.first;

        if (colorRoll > cumulativeProbability)
            continue;
        //Checking and setting StarColor
        if (colorProbability.second == sf::Color::Red)
            Color = StarColor::Red;
        else if (colorProbability.second == sf::Color(255, 69, 0))
            Color = StarColor::Orange;
		else if (colorProbability.second == sf::Color(255, 255, 159))
			Color = StarColor::White;
		else if (colorProbability.second == sf::Color::Blue)
			Color = StarColor::Blue;
		else if (colorProbability.second == sf::Color(50, 23, 77))
			Color = StarColor::Purple;
        else 
           throw std::runtime_error("StarSystem::DetermineStarColor() - Invalid StarColor");


		StarShape.setFillColor(colorProbability.second);

		return;
	}

}
