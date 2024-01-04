#include "pch.h"
#include "Universe.h"
#include "StarSystem.h"

uvy::Universe::Universe() : m_sectorSize({ 250, 250}), m_starSystemChance(0.30f),
m_sectorsDrawn(0), m_starsDrawn(0), m_drawSectors(true)
{
	SharedData::SetUniverse(this);
}

uvy::Universe::~Universe() {}

void uvy::Universe::Update(sf::View& l_view) {

	//Variables
	sf::Vector2f viewCenter			   = l_view.getCenter();
	sf::Vector2f viewSize			   = l_view.getSize();
	sf::Vector2f viewTopLeft           = viewCenter - (viewSize / 2.f);	 //Get the top left corner of the view
	sf::Vector2f viewBottomRight	   = viewCenter + (viewSize / 2.f);//Get the bottom right corner of the view
	sf::Vector2i viewTopLeftSector	   = sf::Vector2i(floor(viewTopLeft.x / m_sectorSize.x), floor(viewTopLeft.y / m_sectorSize.y));//Get the top left sector
	sf::Vector2i viewBottomRightSector = sf::Vector2i(floor(viewBottomRight.x / m_sectorSize.x), floor(viewBottomRight.y / m_sectorSize.y));//Get the bottom right sector

	//Go through sectors inside view
	ProcessSectors(viewTopLeftSector, viewBottomRightSector);
	MouseHoverStar();
}

void uvy::Universe::Render(sf::View& l_view) {
	for (auto& drawable : m_drawQueue)
		SharedData::GetWindow().Draw(*drawable);

	m_drawQueue.clear();
}

void uvy::Universe::SetSectorSize(const sf::Vector2<int64_t>& l_sectorSize) {
	m_sectorSize = l_sectorSize;
}

void uvy::Universe::SetStarSystemChance(const float l_starSystemChance) {
	m_starSystemChance = l_starSystemChance;
}

void uvy::Universe::SetDrawSectors(const bool l_drawSectors) {
	m_drawSectors = l_drawSectors;
}

const sf::Vector2<int64_t>& uvy::Universe::GetSectorSize() const {
	return m_sectorSize;
}

const float uvy::Universe::GetStarSystemChance() const {
	return m_starSystemChance;
}

const uint64_t& uvy::Universe::GetSectorsDrawn() const {
	return m_sectorsDrawn;
}

const uint64_t& uvy::Universe::GetStarsDrawn() const {
	return m_starsDrawn;
}

const sf::Vector2<int64_t>& uvy::Universe::GetMouseSector() const {
	return m_mouseSector;
}

const bool uvy::Universe::GetDrawSectors() const {
	return m_drawSectors;
}

void uvy::Universe::ProcessSectors(const sf::Vector2i& l_sectorOnTopLeftView, const sf::Vector2i& l_sectorOnBottomRightView) {
	
	//Variables
	const bool   debug	  = SharedData::GetDebug();

	int64_t sectorsDrawn = 0;
	int64_t starsDrawn   = 0;

	for (int64_t x = l_sectorOnTopLeftView.x; x <= l_sectorOnBottomRightView.x; ++x)
		for (int64_t y = l_sectorOnTopLeftView.y; y <= l_sectorOnBottomRightView.y; ++y) {

			UpdateMousePosition(x, y);

			//Draw sector shape in debug//
			if (debug && m_drawSectors) {
				DrawSector(x, y);
				++sectorsDrawn;
			}

			//Star systems//
			if (!GenerateStarSystem(x, y, starsDrawn))
				continue;
			

		}

	//Debugging
	m_sectorsDrawn = sectorsDrawn;
	m_starsDrawn   = starsDrawn;
}

bool uvy::Universe::GenerateStarSystem(const int64_t& l_x, const int64_t& l_y, int64_t& l_starsDrawn) {

	StarSystem starSystem(l_x, l_y);
	if (!starSystem.HasStar())
		return false;
	
	l_starsDrawn++;
	m_drawQueue.push_back(std::make_unique<sf::CircleShape>(starSystem.GetStarShape()));
}

void uvy::Universe::DrawSector(const int64_t& l_x, const int64_t& l_y) {

	//Get the position of the sector
	sf::Vector2f sectorPosition = sf::Vector2f(l_x * m_sectorSize.x, l_y * m_sectorSize.y);

	//Create a rectangle
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(m_sectorSize.x, m_sectorSize.y));
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::White);
	rect.setOutlineThickness(1.f);
	rect.setPosition(sectorPosition);

	m_drawQueue.push_back(std::make_unique<sf::RectangleShape>(rect));
}

void uvy::Universe::UpdateMousePosition(const int64_t& l_x, const int64_t& l_y) {
	//Variables
	Window& window				= SharedData::GetWindow();
	sf::Vector2f mousePos		= window.GetWindow().mapPixelToCoords(sf::Mouse::getPosition(window.GetWindow()));//Mouse view position
	sf::Vector2f sectorPosition = sf::Vector2f(l_x * m_sectorSize.x, l_y * m_sectorSize.y);

	if (!IsMouseInSector(l_x, l_y))
		return;

	m_mouseSector = sf::Vector2<int64_t>(l_x, l_y);
}

bool uvy::Universe::IsMouseInSector(const int64_t& l_x, const int64_t& l_y) {

	//Variables
	Window& window = SharedData::GetWindow();
	sf::Vector2f mousePos = window.GetWindow().mapPixelToCoords(sf::Mouse::getPosition(window.GetWindow()));//Mouse view position
	sf::Vector2f sectorPosition = sf::Vector2f(l_x * m_sectorSize.x, l_y * m_sectorSize.y);

	if (mousePos.x >= sectorPosition.x && mousePos.x <= sectorPosition.x + m_sectorSize.x &&
		mousePos.y >= sectorPosition.y && mousePos.y <= sectorPosition.y + m_sectorSize.y)
		return true;
	
	return false;
}

void uvy::Universe::MouseHoverStar() {

	//Draw selector around it//
	sf::Vector2<int64_t> mouseSec = GetMouseSector();
	StarSystem starSystem(mouseSec.x, mouseSec.y);
	if (!starSystem.HasStar())
		return;
	Window& window = SharedData::GetWindow();
	sf::Vector2f mousePos = window.GetWindow().mapPixelToCoords(sf::Mouse::getPosition(window.GetWindow()));//Mouse view position
	if (!starSystem.GetStarShape().getGlobalBounds().contains(mousePos))
		return;
	starSystem.GetStarSelector().setPosition(starSystem.GetStarShape().getPosition());
	m_drawQueue.push_back(std::make_unique<sf::RectangleShape>(starSystem.GetStarSelector()));

	//Display info//
	sf::Vector2f starScreenPos			= (sf::Vector2f)window.GetWindow().mapCoordsToPixel(starSystem.GetStarShape().getPosition());
	const sf::Vector2<uint64_t> winSize = SharedData::GetWindow().GetSize();

	ImGui::Begin("Star Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowFontScale(SharedData::GetWindowFontScale());
	ImGui::SetWindowSize(ImVec2(winSize.x * 0.15,winSize.y * 0.12));
	ImVec2 thisWinSize = ImGui::GetWindowSize();
	ImGui::SetWindowPos(ImVec2(starScreenPos.x - (thisWinSize.x / 2), starScreenPos.y + starSystem.GetStarShape().getGlobalBounds().width * 1.5));
	if(SharedData::GetDebug()) 
		ImGui::Text("Star ID: %d", starSystem.GetID());
	ImGui::Text("Star Position: %d, %d", mouseSec.x, mouseSec.y);
	ImGui::Text("Star Size: %s", starSystem.GetStarSizeString().c_str());
	ImGui::Text("Star Color: %s", starSystem.GetStarColorString().c_str());
	ImGui::End();
}
