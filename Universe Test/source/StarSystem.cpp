#include "pch.h"
#include "StarSystem.h"
#include "Universe.h"

uvy::StarSystem::StarSystem(const int64_t& l_x, const int64_t& l_y) :
m_starSize(StarSize::Small), m_starColor(StarColor::Orange),
m_starPositionInSector(StarPositionInSector::Center),
m_id(0), m_starRadius(1.f), m_hasStar(false)
{

	GenerateSeed(l_x, l_y);

	if(!GenerateStar())
		return;	//Star doesn't exist. m_hasStar is false by default.

	DetermineStarColor();
	DetermineStarSize();
	DetermineStarPosition();
}

uvy::StarSystem::~StarSystem() {}

void uvy::StarSystem::SetStarPositionInSector(const int64_t& l_x, const int64_t& l_y) {

	const sf::Vector2<int64_t>& sectorSize = SharedData::GetUniverse().GetSectorSize();
	sf::Vector2f position;

	switch (m_starPositionInSector) {
		case StarPositionInSector::Center:
			position = sf::Vector2f(l_x * sectorSize.x + sectorSize.x * 0.50f, l_y * sectorSize.y + sectorSize.y * 0.50f);
			break;
		case StarPositionInSector::CenterLeft:
			position = sf::Vector2f(l_x * sectorSize.x + sectorSize.x * 0.15f, l_y * sectorSize.y + sectorSize.y * 0.50f);
			break;
		case StarPositionInSector::CenterRight:
			position = sf::Vector2f(l_x * sectorSize.x + sectorSize.x * 0.85f, l_y * sectorSize.y + sectorSize.y * 0.50f);
			break;
		case StarPositionInSector::CenterTop:
			position = sf::Vector2f(l_x * sectorSize.x + sectorSize.x * 0.50f, l_y * sectorSize.y + sectorSize.y * 0.15f);
			break;
		case StarPositionInSector::TopLeft:
			position = sf::Vector2f(l_x * sectorSize.x + sectorSize.x * 0.15f, l_y * sectorSize.y + sectorSize.y * 0.15f);
			break;
		case StarPositionInSector::TopRight:
			position = sf::Vector2f(l_x * sectorSize.x + sectorSize.x * 0.90f, l_y * sectorSize.y + sectorSize.y * 0.15f);
			break;
		case StarPositionInSector::BottomLeft:
			position = sf::Vector2f(l_x * sectorSize.x + sectorSize.x * 0.15f, l_y * sectorSize.y + sectorSize.y * 0.85f);
			break;
		case StarPositionInSector::BottomRight:
			position = sf::Vector2f(l_x * sectorSize.x + sectorSize.x * 0.90f, l_y * sectorSize.y + sectorSize.y * 0.85f);
			break;
		default:
			throw std::runtime_error("StarSystem::SetStarPositionInSector() - Invalid StarPositionInSector");
	}

	m_starShape.setPosition(position);

}

sf::CircleShape& uvy::StarSystem::GetStarShape() {
	return m_starShape;
}

sf::RectangleShape& uvy::StarSystem::GetStarSelector() {
	return m_starSelector;
}

const StarSize& uvy::StarSystem::GetStarSize() const {
	return m_starSize;
}

std::string uvy::StarSystem::GetStarSizeString() const {

	switch (m_starSize) {
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

const StarColor& uvy::StarSystem::GetStarColor() const {
	return m_starColor;
}

std::string uvy::StarSystem::GetStarColorString() const {
	switch (m_starColor) {
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

const StarPositionInSector& uvy::StarSystem::GetStarPositionInSector() const {
	return m_starPositionInSector;
}

std::string uvy::StarSystem::GetStarPositionInSectorString() const {
	
	switch (m_starPositionInSector) {
	case StarPositionInSector::TopLeft:
		return "Top Left";
		break;
	case StarPositionInSector::TopRight:
		return "Top Right";
		break;
	case StarPositionInSector::BottomLeft:
		return "Bottom Left";
		break;
	case StarPositionInSector::BottomRight:
		return "Bottom Right";
		break;
	case StarPositionInSector::CenterTop:
		return "Center Top";
		break;
	case StarPositionInSector::CenterLeft:
		return "Center Left";
		break;
	case StarPositionInSector::CenterRight:
		return "Center Right";
		break;
	case StarPositionInSector::Center:
		return "Center";
		break;
	default:
		throw std::runtime_error("StarSystem::GetStarPositionInSectorString() - Invalid StarPositionInSector");
	}

}

const int64_t& uvy::StarSystem::GetID() const {
	return m_id;
}

const float uvy::StarSystem::GetStarRadius() const {
	return m_starRadius;
}

const bool uvy::StarSystem::HasStar() const {
	return m_hasStar;
}

void uvy::StarSystem::GenerateSeed(const int64_t& l_x, const int64_t& l_y) {

	m_id = SharedData::Get().GenerateSeed(l_x, l_y);
	SharedData::GetRNG().seed(m_id); //setting unique seed to rng, so that every star stays the same

}

bool uvy::StarSystem::GenerateStar() {

	std::uniform_real_distribution<float> dst(0.f, 1.f);

	if (dst(SharedData::GetRNG()) > SharedData::GetUniverse().GetStarSystemChance())
		return false;

	m_hasStar = true;
	return true;
}

void uvy::StarSystem::DetermineStarSize() {

	std::mt19937_64& rng = SharedData::GetRNG();

	float medium	 = 8.0f;
	float large		 = 12.0f;
	float small		 = 4.0f;
	float hyperLarge = 24.0f;

	//Chance and Size
	std::vector<std::pair<double, float>> sizeProbabilities = {
		{0.565, medium     }, //56.5%
		{0.250, large      }, //25.0%  
		{0.180, small      }, //18.0% 
		{0.005, hyperLarge }  //00.5%  
	};

	std::uniform_real_distribution<double> dist(0.0, 1.0);
	double sizeRoll = dist(rng);

	double cumulativeProbability = 0.0;

	for (auto& sizeProbability : sizeProbabilities) {

		cumulativeProbability += sizeProbability.first;

		if (sizeRoll > cumulativeProbability)
			continue;

		//Checking and setting StarSize
		if (sizeProbability.second == medium)
			m_starSize = StarSize::Medium;
		else if (sizeProbability.second == large)
			m_starSize = StarSize::Large;
		else if (sizeProbability.second == small)
			m_starSize = StarSize::Small;
		else if (sizeProbability.second == hyperLarge)
			m_starSize = StarSize::HyperLarge;
		else
			throw std::runtime_error("StarSystem::DetermineStarSize() - Invalid StarSize");

		m_starShape.setRadius(sizeProbability.second);
		m_starRadius = sizeProbability.second;
		m_starShape.setOrigin(m_starShape.getGlobalBounds().width / 2, m_starShape.getGlobalBounds().height / 2);

		//Selector shape
		m_starSelector.setSize(sf::Vector2f(m_starShape.getGlobalBounds().width * 2, m_starShape.getGlobalBounds().height * 2));
		m_starSelector.setOrigin(m_starSelector.getGlobalBounds().width / 2, m_starSelector.getGlobalBounds().height / 2);
		m_starSelector.setFillColor(sf::Color::Transparent);
		m_starSelector.setOutlineColor(sf::Color::White);
		m_starSelector.setOutlineThickness(1.5);


		return;
	}
}

void uvy::StarSystem::DetermineStarColor() {

	std::mt19937_64& rng = SharedData::GetRNG();

	//Chance and Color
	std::vector<std::pair<double, sf::Color>> colorProbabilities = {
		{ 0.525, sf::Color::Red                      }, //52.5%
		{ 0.325, sf::Color(255, 69, 0)    /*Orange*/ }, //32.5%
		{ 0.075, sf::Color(255, 255, 159) /*White*/  }, //07.5%
		{ 0.050, sf::Color::Blue                     }, //05.0%
		{ 0.025, sf::Color(50, 23, 77)   /*Purple*/  }, //02.5%
	};

	std::uniform_real_distribution<double> dist(0.0, 1.0);
	double colorRoll = dist(rng);

	double cumulativeProbability = 0.0;

	for (auto& colorProbability : colorProbabilities) {

		cumulativeProbability += colorProbability.first;

		if (colorRoll > cumulativeProbability)
			continue;
		//Checking and setting StarColor
		if (colorProbability.second == sf::Color::Red)
			m_starColor = StarColor::Red;
		else if (colorProbability.second == sf::Color(255, 69, 0))
			m_starColor = StarColor::Orange;
		else if (colorProbability.second == sf::Color(255, 255, 159))
			m_starColor = StarColor::White;
		else if (colorProbability.second == sf::Color::Blue)
			m_starColor = StarColor::Blue;
		else if (colorProbability.second == sf::Color(50, 23, 77))
			m_starColor = StarColor::Purple;
		else
			throw std::runtime_error("StarSystem::DetermineStarColor() - Invalid StarColor");


		m_starShape.setFillColor(colorProbability.second);

		return;
	}
}

void uvy::StarSystem::DetermineStarPosition() {
	
	std::mt19937_64& rng = SharedData::GetRNG();

	//Chance and position
	std::vector<std::pair<double, StarPositionInSector>> positions{
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

		m_starPositionInSector = position.second;
		return;
	}
}
