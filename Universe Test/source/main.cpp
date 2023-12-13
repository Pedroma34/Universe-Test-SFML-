#include "pch.h"
#include "StarSystem.h"


const int64_t WindowWidth           = 2560;               //2k is 2560x1440, 4k is 3840x2160, 1440p is 1920x1440
const int64_t WindowHeight          = 1440;
const int64_t ViewWidth		        = 1000;
const int64_t ViewHeight            = 800;
const int64_t UniverseSize          = INT_MAX;            // Define the virtual universe size (in sectors)  // 100x100 sectors
int64_t UserX                       = UniverseSize / 2;   // Variables to represent the user's position in the universe
int64_t UserY                       = UniverseSize / 2;
double StarSystemProbability        = 0.95;               // Probability of a star system appearing in a sector
const int64_t SectorSize            = 75;	              // Sector size in pixels
bool Debug                          = true;               // Debug flag to draw sector shapes
uint64_t SectorsDrawn               = 0;                  // Counter to track the number of sectors drawn
sf::Vector2i MouseWorldPosition     = sf::Vector2i(0,0);
std::vector<std::unique_ptr<sf::Shape>> DrawQueue;        // Queue of shapes to draw. Shapes are allocated when shape is pushed and vector is cleared when drawn.
        

// Draw star systems inside visible sectors
void DrawStarSystems(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow);


// Draw the shape of the sectors for debugging
void DrawSectorsShape(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow);


void DrawStarSelected(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow);


// Get the sector coordinates of the sector under the mouse cursor
sf::Vector2<int64_t> GetSectorCoordsOnMouse(sf::RenderWindow& l_window, int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow);


// Proccess visible universe, looping through visible sectors. Other functions, such as DrawStarSystems, are called inside this function
//in order to access visible sectors, stars, etc.
void ProccessVisibleUniverse();


// Draw shapes in the draw queue and clear the queue
void ProccessDrawQueue();


bool DoesSectorHaveStarSystem(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow, std::mt19937_64& l_rng);


bool IsMouseOnSector(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow);


int main() {
    SharedData::SetSectorSize(&SectorSize);

    //logging universie size
    spdlog::info("Universe size: {}", UniverseSize);

    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Infinite Universe");
    SharedData::SetWindow(&window);
    window.setVerticalSyncEnabled(true);
    sf::View view;
    view.reset(sf::FloatRect(0, 0, ViewWidth, ViewHeight));
    window.setView(view);

    //Imgui
    ImGui::SFML::Init(window);

    //Time
    sf::Time time;
    sf::Clock clock;


    // Random number generator and distribution
    std::mt19937_64 rng;
    SharedData::SetRNG(&rng);
    SharedData::SetStarSystemProbability(&StarSystemProbability);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
            
            //Keybaord
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Tab)
					Debug = !Debug;

                int speed = 1;
                if(event.key.code == sf::Keyboard::Left)
                    UserX = (UserX - speed + UniverseSize) % UniverseSize;
                if(event.key.code == sf::Keyboard::Right)
					UserX = (UserX + speed) % UniverseSize;
                if(event.key.code == sf::Keyboard::Up)
                    UserY = (UserY - speed + UniverseSize) % UniverseSize;
                if(event.key.code == sf::Keyboard::Down)
                    UserY = (UserY + speed) % UniverseSize;
                
            }

            //Mouse
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    //Calculating next zoom size, and if it's less than 50% of the view size, do not zoom in and set view to 50% of the view size
                    sf::Vector2f nextZoomSize = view.getSize();
                    nextZoomSize.x *= 0.95f;
                    nextZoomSize.y *= 0.95f;
                    if (nextZoomSize.x < ViewWidth / 2 && nextZoomSize.y < ViewHeight / 2)
                        view.setSize(ViewWidth / 2, ViewHeight / 2);
					else
						view.zoom(0.950f);
                }
                else if (event.mouseWheelScroll.delta < 0) {

                    //Calculating next zoom size, if next zoom size is greater than ViewWidth and ViewHeight, do not zoom out
                    sf::Vector2f nextZoomSize = view.getSize();
                    nextZoomSize.x *= 1.05f;
                    nextZoomSize.y *= 1.05f;
                    if (nextZoomSize.x > ViewWidth && nextZoomSize.y > ViewHeight)
						view.setSize(ViewWidth, ViewHeight);
                    else
                        view.zoom(1.050f);
                }
			}
        }

        //Update//
        window.setView(view);

        MouseWorldPosition = (sf::Vector2i)window.mapPixelToCoords(sf::Mouse::getPosition(window));

        ImGui::SFML::Update(window, time);

        //test window
        if (Debug) {
            ImGui::Begin("Test");
            ImGui::SetWindowPos(ImVec2(0, 0));
            ImGui::SetWindowFontScale(2.0f);
            ImGui::Text("Universe size: %d", UniverseSize);
            ImGui::Text("Star system probability: %f", StarSystemProbability);
            ImGui::Text("UserX: %d", UserX);
            ImGui::Text("UserY: %d", UserY);
            ImGui::Text("Sectors drawn: %d", SectorsDrawn);
            ImGui::Text(std::string("View size x: " + std::to_string(view.getSize().x)).c_str());
            ImGui::Text(std::string("View size y: " + std::to_string(view.getSize().y)).c_str());
            ImGui::Text(std::string("View center x: " + std::to_string(view.getCenter().x)).c_str());
            ImGui::Text(std::string("View center y: " + std::to_string(view.getCenter().y)).c_str());
            ImGui::Text(std::string("Mouse world position x: " + std::to_string(MouseWorldPosition.x)).c_str());
            ImGui::Text(std::string("Mouse world position y: " + std::to_string(MouseWorldPosition.y)).c_str());
            ImGui::Text("Debug: %d", Debug);
            ImGui::End();
        }
        //end test window

        ProccessVisibleUniverse();
        //End Update//
        

        //Late Update
        time = clock.restart();
        
        //End Late Update//
        
        
        //Drawing//
        window.clear();
        
        ProccessDrawQueue();
        ImGui::SFML::Render(window);

        window.display();
        //End Drawing//
    }

    return 0;
}


void DrawStarSystems(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow) {
    //Variables
    sf::RenderWindow& window = *SharedData::GetWindow();
    std::mt19937_64& rng = *SharedData::GetRNG();

    //Creating Star System//
    StarSystem starSystem(l_row, l_column);
    if (!starSystem.HasStar)
		return;
    sf::CircleShape&     starShape        = starSystem.StarShape;
    const sf::FloatRect& starGlobalBounds = starShape.getGlobalBounds();


    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double positionRoll = dist(rng);
    const double probabilityTopLeft     = 0.125;
    const double probabilityTopRight    = 0.125;
    const double probabilityBottomLeft  = 0.125;
    const double probabilityBottomRight = 0.125;
    const double probabilityCenterTop   = 0.125;
    const double probabilityCenterLeft  = 0.125;
    const double probabilityCenterRight = 0.125;
    const double probabilityCenter      = 0.125;

    //Top left
    if (positionRoll < probabilityTopLeft) 
		starShape.setPosition(l_row * SectorSize - l_startColumn * SectorSize + starGlobalBounds.width, l_column * SectorSize - l_startRow * SectorSize + starGlobalBounds.height);
	//Top Right
    else if (positionRoll < (probabilityTopLeft + probabilityTopRight)) 
		starShape.setPosition(l_row * SectorSize - l_startColumn * SectorSize + SectorSize - starGlobalBounds.width, l_column * SectorSize - l_startRow * SectorSize + starGlobalBounds.height);
	//Bottom Left
    else if (positionRoll < (probabilityTopLeft + probabilityTopRight + probabilityBottomLeft)) 
		starShape.setPosition(l_row * SectorSize - l_startColumn * SectorSize + starGlobalBounds.width, l_column * SectorSize - l_startRow * SectorSize + SectorSize - starGlobalBounds.height);
	//Bottom Right
    else if (positionRoll < (probabilityTopLeft + probabilityTopRight + probabilityBottomLeft + probabilityBottomRight)) 
		starShape.setPosition(l_row * SectorSize - l_startColumn * SectorSize + SectorSize - starGlobalBounds.width, l_column * SectorSize - l_startRow * SectorSize + SectorSize - starGlobalBounds.height);
	//Center Top
	else if (positionRoll < (probabilityTopLeft + probabilityTopRight + probabilityBottomLeft + probabilityBottomRight + probabilityCenterTop))
        starShape.setPosition(l_row * SectorSize - l_startColumn * SectorSize + SectorSize / 2, l_column * SectorSize - l_startRow * SectorSize + starGlobalBounds.height);
    //Center Left
    else if (positionRoll < (probabilityTopLeft + probabilityTopRight + probabilityBottomLeft + probabilityBottomRight + probabilityCenterTop + probabilityCenterLeft))
	   	starShape.setPosition(l_row * SectorSize - l_startColumn * SectorSize + starGlobalBounds.width, l_column * SectorSize - l_startRow * SectorSize + SectorSize / 2);
    	//Center Right
    else if (positionRoll < (probabilityTopLeft + probabilityTopRight + probabilityBottomLeft + probabilityBottomRight + probabilityCenterTop + probabilityCenterLeft + probabilityCenterRight))
        starShape.setPosition(l_row * SectorSize - l_startColumn * SectorSize + SectorSize - starGlobalBounds.width, l_column * SectorSize - l_startRow * SectorSize + SectorSize / 2);
    //Center
    else 
		starShape.setPosition(l_row * SectorSize - l_startColumn * SectorSize + SectorSize / 2, l_column * SectorSize - l_startRow * SectorSize+ SectorSize / 2);
	
    DrawQueue.push_back(std::make_unique<sf::CircleShape>(starSystem.StarShape));
}

void DrawSectorsShape(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow) {
    if (!Debug) 
        return;

    //Variables
    sf::RenderWindow& window = *SharedData::GetWindow();

    sf::RectangleShape sectorShape(sf::Vector2f(SectorSize, SectorSize));
	sectorShape.setFillColor(sf::Color::Transparent);
	sectorShape.setOutlineColor(sf::Color::White);
	sectorShape.setOutlineThickness(1);
	sectorShape.setPosition(l_row * SectorSize - l_startColumn * SectorSize, l_column * SectorSize - l_startRow * SectorSize);
    DrawQueue.push_back(std::make_unique<sf::RectangleShape>(sectorShape));

   //Debugging. Draw a circle around the sector if mouse is hovering over it
    if (IsMouseOnSector(l_row, l_column, l_startColumn, l_startRow)) {
		
        sf::CircleShape circleShape(SectorSize / 2);
		circleShape.setFillColor(sf::Color::Transparent);
		circleShape.setOutlineColor(sf::Color::White);
		circleShape.setOutlineThickness(1);
		circleShape.setPosition(l_row * SectorSize - l_startColumn * SectorSize, l_column * SectorSize - l_startRow * SectorSize);

        DrawQueue.push_back(std::make_unique<sf::CircleShape>(circleShape));
	}
}

void DrawStarSelected(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow) {


   // if (IsMouseOnSector(l_row, l_column, l_startColumn, l_startRow)) {

   //     //Checking if sector has star
   //     if (!DoesSectorHaveStarSystem(l_row, l_column, l_startColumn, l_startRow, l_rng))
			//return;
   //     
   //     //Getting star system data
   //     StarSystem starSystem;
   //     sf::CircleShape&     starShape        = starSystem.StarShape;
   //     const sf::FloatRect& starGlobalBounds = starShape.getGlobalBounds();

   //     //Drawing a square around the star system
   //     sf::RectangleShape starSelected(sf::Vector2f(starGlobalBounds.width * 2, starGlobalBounds.height * 2));
   //     starSelected.setFillColor(sf::Color::Transparent);
   //     starSelected.setOutlineColor(sf::Color::White);
   //     starSelected.setOutlineThickness(1);
   //     starSelected.setPosition(starShape.getPosition().x - starGlobalBounds.width, starShape.getPosition().y - starGlobalBounds.height);
   //     l_window.draw(starSelected);
   // }
}

sf::Vector2<int64_t> GetSectorCoordsOnMouse(sf::RenderWindow& l_window, int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow) {
    int64_t row    = (MouseWorldPosition.x + l_startColumn * SectorSize) / SectorSize;
    int64_t column = (MouseWorldPosition.y + l_startRow * SectorSize) / SectorSize;
    return sf::Vector2<int64_t>(row, column);
}

void ProccessVisibleUniverse() {
    //Variables
    sf::RenderWindow& window = *SharedData::GetWindow();
    std::mt19937_64& rng = *SharedData::GetRNG();

    // Calculate the range of sectors to render based on the user's position
    int64_t startX = std::max(0LL, UserX - ViewWidth / (2 * SectorSize));
    int64_t startY = std::max(0LL, UserY - ViewHeight / (2 * SectorSize));
    int64_t endX = std::min(startX + ViewWidth / SectorSize, UniverseSize);
    int64_t endY = std::min(startY + ViewHeight / SectorSize, UniverseSize);

    uint64_t count = 0;
    for (int64_t x = startX; x < endX; ++x) {
        for (int64_t y = startY; y < endY; ++y) {
            DrawStarSystems (x, y, startX, startY);
            DrawSectorsShape(x, y, startX, startY);
            DrawStarSelected(x, y, startX, startY);
            count++;
        }
        count++;
    }
    SectorsDrawn = count;
}

void ProccessDrawQueue() {
	sf::RenderWindow& window = *SharedData::GetWindow();
	for (auto& shape : DrawQueue) 
		window.draw(*shape);
	DrawQueue.clear();
}

bool DoesSectorHaveStarSystem(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow, std::mt19937_64& l_rng) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    l_rng.seed(SharedData::Get().GenerateSeed(l_row, l_column));
    return dist(l_rng) < StarSystemProbability;
}

bool IsMouseOnSector(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow) {
    if (MouseWorldPosition.x >= l_row * SectorSize - l_startColumn * SectorSize && MouseWorldPosition.x <= l_row * SectorSize - l_startColumn * SectorSize + SectorSize &&
        MouseWorldPosition.y >= l_column * SectorSize - l_startRow * SectorSize && MouseWorldPosition.y <= l_column * SectorSize - l_startRow * SectorSize + SectorSize)
        return true;
    return false;
}
