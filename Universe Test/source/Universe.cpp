#include "pch.h"
#include "Universe.h"
#include "StarSystem/StarSystem.h"

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

std::weak_ptr<uvy::StarSystem> uvy::Universe::DoesSectorHaveStarModified(const int64_t& l_x, const int64_t& l_y) {

	auto starModified = m_starSystems.find(GetSeed(l_x, l_y));
	if (starModified == m_starSystems.end())
		return std::weak_ptr<StarSystem>(); 
	
	return starModified->second;
	

}

std::weak_ptr<uvy::StarSystem> uvy::Universe::AddStarToModify(std::weak_ptr<class StarSystem> l_starSystem) {
	
	if (l_starSystem.expired())
		return std::weak_ptr<StarSystem>();

	auto star = m_starSystems.find(l_starSystem.lock()->GetID());

	//Check if this star system is already in the list and return if it has
	if (star != m_starSystems.end())
		return star->second;
	
	//Add it to the modified list
	l_starSystem.lock()->SetModified(true);
	return m_starSystems.emplace(l_starSystem.lock()->GetID(), l_starSystem.lock()).first->second;
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

void uvy::Universe::SetSelectedModifiedStar(std::weak_ptr<class StarSystem> l_starSelectedModified) {
	m_starSelectedModified = l_starSelectedModified; //Don't need to lock because star modified is in a container
}

void uvy::Universe::SetSelectedStar(std::weak_ptr<class StarSystem> l_starSelected) {
	m_starSelected = l_starSelected.lock(); //Needs to be locked because star selected is not in a container
}

std::shared_ptr<uvy::StarSystem> uvy::Universe::GetStarUnderMouse() {

	//Get the sector that the mouse is in
	sf::Vector2<int64_t> mouseSec = GetMouseSector();

	std::shared_ptr<StarSystem> starSystem;

	//Check if this sector has a star system that has been modified
	auto starModified = DoesSectorHaveStarModified(mouseSec.x, mouseSec.y);

	if (!starModified.expired())
		starSystem = starModified.lock(); //Set the star system to the one that has been modified
	else
		//Create a new star system
		starSystem = std::make_shared<StarSystem>(mouseSec.x, mouseSec.y);

	//Check if this sector has a star system and return if it doesn't
	if (!starSystem->HasStar())
		return nullptr;

	Window& window = SharedData::GetWindow();
	sf::Vector2f mousePos = window.GetWindow().mapPixelToCoords(sf::Mouse::getPosition(window.GetWindow()));//Mouse view position
	if (!starSystem->GetStarShape().getGlobalBounds().contains(mousePos))
		return nullptr; //Mouse isn't over star, so return null

	return starSystem;
}

std::weak_ptr<class uvy::StarSystem> uvy::Universe::GetSelectedModifiedStar() {
	return m_starSelected;
}

std::shared_ptr<class uvy::StarSystem> uvy::Universe::GetSelectedStar() {
	return m_starSelected;
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

uint64 uvy::Universe::GetStarsModified() const {
	return m_starSystems.size();
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

	//Check if this sector has a star system that has been modified
	std::weak_ptr<StarSystem> starModified = DoesSectorHaveStarModified(l_x, l_y);
	if (!starModified.expired()) {
		m_drawQueue.push_back(std::make_unique<sf::CircleShape>(starModified.lock()->GetStarShape()));
		l_starsDrawn++;
		return true; //Star system has been modified, so we don't need to generate it again
	}

	//This sector has no star system tht has been modified, so we need to generate it
	StarSystem starSystem(l_x, l_y);
	if (!starSystem.HasStar())
		return false;
	
	l_starsDrawn++;
	m_drawQueue.push_back(std::make_unique<sf::CircleShape>(starSystem.GetStarShape()));
	return true;
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

	std::shared_ptr<StarSystem> starSystem = GetStarUnderMouse();

	if(starSystem == nullptr)
		return;

	Window& window				  = SharedData::GetWindow();

	starSystem->GetStarSelector().setPosition(starSystem->GetStarShape().getPosition());
	m_drawQueue.push_back(std::make_unique<sf::RectangleShape>(starSystem->GetStarSelector()));

	//Modify. It already checks if it's in the list
	//AddStarToModify(mouseSec.x, mouseSec.y, starSystem);

	//Display info//
	sf::Vector2f starScreenPos			= (sf::Vector2f)window.GetWindow().mapCoordsToPixel(starSystem->GetStarShape().getPosition());
	const sf::Vector2<uint64_t> winSize = SharedData::GetWindow().GetSize();

	ImGui::Begin("Star Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowFontScale(SharedData::GetWindowFontScale());
	ImGui::SetWindowSize(ImVec2(winSize.x * 0.15,winSize.y * 0.12));
	ImVec2 thisWinSize = ImGui::GetWindowSize();
	ImGui::SetWindowPos(ImVec2(starScreenPos.x - (thisWinSize.x / 2), starScreenPos.y + starSystem->GetStarShape().getGlobalBounds().width * 1.5));
	if(SharedData::GetDebug()) 
	ImGui::Text("Star ID: %d", starSystem->GetID());
	ImGui::Text("Star Position: %d, %d", starSystem->GetSectorPosition().x, starSystem->GetSectorPosition().y);
	ImGui::Text("Star Size: %s", starSystem->GetStarSizeString().c_str());
	ImGui::Text("Star Color: %s", starSystem->GetStarColorString().c_str());
	ImGui::End();
}

int64 uvy::Universe::GetSeed(const int64_t& l_x, const int64_t& l_y) {
	return SharedData::Get().GenerateSeed(l_x, l_y);
}
