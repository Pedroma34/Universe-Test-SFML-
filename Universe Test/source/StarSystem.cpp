#include "pch.h"
#include "StarSystem.h"
#include "Planet.h"

StarSystem::StarSystem(int64_t l_x, int64_t l_y) : Color(StarColor::White), 
    Size(StarSize::Medium), HasStar(false), HasPlanet(false), ChanceForMultiplePlanets(0.9f), PositionInSector(StarPositionInSector::Center)
{
    
    std::mt19937_64& rng = *SharedData::GetRNG();
    const double& StarSystemProbability = SharedData::GetStarSystemProbability();

    uint64_t seed = SharedData::Get().GenerateSeed(l_x, l_y);
    rng.seed(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    if (dist(rng) > StarSystemProbability)
        return; // No star
    HasStar = true;

    DetermineStarSize(rng);
    DetermineStarColor(rng);
    DetermineStarPosition();
    
    GeneratePlanet(seed);

}

void StarSystem::SetStarPositionInSector(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow) {
    //Variables
    const int64_t& sectorSize = SharedData::GetSectorSize();
    const auto& starGlobalBounds = StarShape.getGlobalBounds();
    const double& UserX = SharedData::GetUserX();
    const double& UserY = SharedData::GetUserY();
    double OffsetX = fmod(UserX, sectorSize);
    double OffsetY = fmod(UserY, sectorSize);


    switch (PositionInSector) {
        case StarPositionInSector::TopLeft:
			Position = sf::Vector2f((l_row - l_startRow) * sectorSize + starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + starGlobalBounds.height - OffsetY);
            break;
        case StarPositionInSector::TopRight:
            Position = sf::Vector2f((l_row - l_startRow) * sectorSize + sectorSize - starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + starGlobalBounds.height - OffsetY);
			break;
            case StarPositionInSector::BottomLeft:
			Position = sf::Vector2f((l_row - l_startRow) * sectorSize + starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize - starGlobalBounds.height - OffsetY);
            break;
		case StarPositionInSector::BottomRight:
            Position = sf::Vector2f((l_row - l_startRow) * sectorSize + sectorSize - starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize - starGlobalBounds.height - OffsetY);
            break;
        case StarPositionInSector::CenterTop:
            Position = sf::Vector2f((l_row - l_startRow) * sectorSize + sectorSize / 2 - OffsetX, (l_column - l_startColumn) * sectorSize + starGlobalBounds.height - OffsetY);
            break;
        case StarPositionInSector::CenterLeft:
            Position = sf::Vector2f((l_row - l_startRow) * sectorSize + starGlobalBounds.width + starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize / 2 - OffsetY);
            break;
        case StarPositionInSector::CenterRight:
	        Position = sf::Vector2f((l_row - l_startRow) * sectorSize + sectorSize - starGlobalBounds.width - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize / 2 - OffsetY);
			break;
		case StarPositionInSector::Center:
			Position = sf::Vector2f((l_row - l_startRow) * sectorSize + sectorSize / 2 - OffsetX, (l_column - l_startColumn) * sectorSize + sectorSize / 2 - OffsetY);
			break;
        default:
		    throw std::runtime_error("StarSystem::SetStarPositionInSector() - Invalid StarPositionInSector");
    }

    StarShape.setPosition(Position);

}

std::string StarSystem::GetStarSizeString() const {
    switch (Size) {
	case StarSize::Small:
        return "Small";
        break;
    case StarSize::Medium:
        return "Medium";
        break;
    case StarSize::Large:
        return "Large";
        break;
    case StarSize::HyperLarge:
        return "Hyper Large";
        break;
	default:
		throw std::runtime_error("StarSystem::GetStarSizeString() - Invalid StarSize");
	}
}

std::string StarSystem::GetStarColorString() const {
    switch (Color) {
	case StarColor::Red:
		return "Red";
		break;
	case StarColor::Orange:
		return "Orange";
		break;
	case StarColor::White:
		return "White";
		break;
	case StarColor::Blue:
		return "Blue";
		break;
	case StarColor::Purple:
		return "Purple";
		break;
	default:
		throw std::runtime_error("StarSystem::GetStarColorString() - Invalid StarColor");
	}
}

void StarSystem::DetermineStarSize(std::mt19937_64& l_rng) {

    float medium     = 8.0f;
    float large      = 12.0f;
    float small      = 4.0f;
    float hyperLarge = 24.0f;

    //Chance and Size
    std::vector<std::pair<double, float>> sizeProbabilities = {
        {0.565, medium     }, //56.5%
        {0.250, large      }, //25.0%  
        {0.180, small      }, //18.0% 
        {0.005, hyperLarge }  //00.5%  
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

        //Selector shape
        StarSelectorShape.setSize(sf::Vector2f(StarShape.getGlobalBounds().width * 2, StarShape.getGlobalBounds().height* 2));
        StarSelectorShape.setOrigin(StarSelectorShape.getGlobalBounds().width / 2, StarSelectorShape.getGlobalBounds().height / 2);
        StarSelectorShape.setFillColor(sf::Color::Transparent);
        StarSelectorShape.setOutlineColor(sf::Color::White);
        StarSelectorShape.setOutlineThickness(1.5);


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

void StarSystem::DetermineStarPosition() {
    //Variables
    auto& rng = *SharedData::GetRNG();
    const int64_t& sectorSize = SharedData::GetSectorSize();
    const auto& starGlobalBounds = StarShape.getGlobalBounds();
    const double& UserX = SharedData::GetUserX();
    const double& UserY = SharedData::GetUserY();
    double OffsetX = fmod(UserX, sectorSize);
    double OffsetY = fmod(UserY, sectorSize);

    //Chance and position
    std::vector<std::pair<double, StarPositionInSector>> positions { 
        /*Top Left     */ {0.125, StarPositionInSector::TopLeft     },
        /*Top Right    */ {0.125, StarPositionInSector::TopRight    },
        /*Bottom Left  */ {0.125, StarPositionInSector::BottomLeft  },
        /*Bottom Right */ {0.125, StarPositionInSector::BottomRight },
        /*Center Top   */ {0.125, StarPositionInSector::CenterTop   },
        /*Center Left  */ {0.125, StarPositionInSector::CenterLeft  },
        /*Center Right */ {0.125, StarPositionInSector::CenterRight },
        /*Center       */ {0.125, StarPositionInSector::Center      },
    };

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double positionRoll = dist(rng);

    double cumulativeProbability = 0.0;

    for (auto& position : positions) {

        cumulativeProbability += position.first;

        if (positionRoll > cumulativeProbability)
            continue;

        PositionInSector = position.second;
        return;
    }
}

void StarSystem::GeneratePlanet(int64_t l_seed) {

    std::mt19937_64& rng = *SharedData::GetRNG();

    //Generating single planet
    auto planet = std::make_shared<Planet>(l_seed);
    if (!planet->Exists) {
        planet.reset();
        return; // No planet
    }

    HasPlanet = true;
    Planets.push_back(std::move(planet));

    //Generating multiple planets
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    auto r = dist(rng);
    if (r > ChanceForMultiplePlanets)
		return; // No more planets

    uint64_t maxNumberOfPlanets = 8;
    uint64_t numbofPlanets = [&]() { //Number of planets to generate
		std::uniform_int_distribution<uint64_t> dist(1, maxNumberOfPlanets - 1); //It's -1 because we already generated one planet
		return dist(rng);
	}();
    
    for (uint64_t i = 0; i < numbofPlanets; ++i) {

        //Just to be safe, clamping the number of planets to maxNumberOfPlanets
        if(Planets.size() >= maxNumberOfPlanets)
            return; // No more planets

		auto planet = std::make_shared<Planet>();
		Planets.push_back(std::move(planet));

	}

}
