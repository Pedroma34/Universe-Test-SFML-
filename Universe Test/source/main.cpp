#include "pch.h"
#include "StarSystem.h"


const int64_t WindowWidth           = 2560;               //2k is 2560x1440, 4k is 3840x2160, 1440p is 1920x1440
const int64_t WindowHeight          = 1440;
const int64_t ViewWidth		        = 1920;
const int64_t ViewHeight            = 1080;
double UserX                        = 0;                  // Position of user in the universe (in pixels, devide by SectorSize to get sector coords)
double UserY                        = 0;
double VelocityX                    = 0.0, 
VelocityY                           = 0.0;                  
double MaxVelocity                  = 1000;               // Maximum velocity
double Acceleration                 = 2000;               // Acceleration
double Friction                     = 0.97;               // Damping factor
double OffsetX;                                           // Calculate the pixel offset within the current sector
double OffsetY;                                           // Calculate the pixel offset within the current sector
double StarSystemProbability        = 0.15;               // Probability of a star system appearing in a sector
const int64_t SectorSize            = 100;	              // Sector size in pixels
bool Debug                          = false;              // Debug flag to draw sector shapes
bool IsDragging                     = false;
sf::Vector2i LastMousePos;
uint64_t SectorsDrawn               = 0;                  // Counter to track the number of sectors drawn
sf::Vector2i SectorCoordsOnMouse    = sf::Vector2i(0,0);  // Sector coordinates of under the mouse cursor
sf::Vector2i MouseWorldPosition     = sf::Vector2i(0,0);
uint64_t StarsVisible               = 0;
std::vector<std::unique_ptr<StarSystem>> StarSystems;     // Stores the star systems in the visible universe
std::vector<std::unique_ptr<sf::Shape>> DrawQueue;        // Queue of shapes to draw. Shapes are allocated when shape is pushed and vector is cleared when drawn.
        

// Draw star systems inside visible sectors
void DrawStarSystems(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow);


// Draw the shape of the sectors for debugging
void DrawSectorsShape(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow);


void DrawStarSelector(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow);


// Proccess visible universe, looping through visible sectors. Other functions, such as DrawStarSystems, are called inside this function
//in order to access visible sectors, stars, etc.
void ProccessVisibleUniverse();


void ProccessInput();


// Draw shapes in the draw queue and clear the queue
void ProccessDrawQueue();


void ProccessVelocityAndPosition();


void DisplayStarSystemInfo(const StarSystem& l_starSystem);


bool DoesSectorHaveStarSystem(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow, std::mt19937_64& l_rng);


bool IsMouseOnSector(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow);


int main() {
    SharedData::SetSectorSize(&SectorSize);
    SharedData::SetUserX(&UserX);
    SharedData::SetUserY(&UserY);

    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Infinite Universe", sf::Style::Default);
    SharedData::SetWindow(&window);
    window.setVerticalSyncEnabled(true);

    //chaning mouse cursor to cross from system
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Cross);
    window.setMouseCursor(cursor);


    //View
    sf::View view;
    view.reset(sf::FloatRect(0, 0, ViewWidth, ViewHeight));
    int zoomLimit = 4; //How much it will be devided by when zooming in
    view.setSize(ViewWidth / (zoomLimit / 2), ViewHeight / (zoomLimit / 2));
    window.setView(view);
    SharedData::SetView(&view);

    //Imgui
    ImGui::SFML::Init(window);

    //Time
    sf::Time time;
    sf::Clock clock;
    SharedData::SetTime(&time);




    // Random number generator and distribution
    std::mt19937_64 rng;
    SharedData::SetRNG(&rng);
    SharedData::SetStarSystemProbability(&StarSystemProbability);

    //Setting user position
    UserX = 54000 * SectorSize;
    UserY = 54000 * SectorSize;

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
            }

            //Mouse
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    //Calculating next zoom size, and if it's less than 50% of the view size, do not zoom in and set view to 50% of the view size
                    sf::Vector2f nextZoomSize = view.getSize();
                    nextZoomSize.x *= 0.95f;
                    nextZoomSize.y *= 0.95f;
                    if (nextZoomSize.x < ViewWidth / zoomLimit && nextZoomSize.y < ViewHeight / zoomLimit)
                        view.setSize(ViewWidth / zoomLimit, ViewHeight / zoomLimit);
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
            if (event.type == sf::Event::MouseButtonPressed)
                if (event.mouseButton.button == sf::Mouse::Right) {
                    IsDragging = true;
                    LastMousePos = sf::Mouse::getPosition(window);
                }

            if (event.type == sf::Event::MouseButtonReleased)
                if (event.mouseButton.button == sf::Mouse::Right)
                    IsDragging = false;

            if (event.type == sf::Event::MouseMoved)
                if (IsDragging) {
                    sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                    sf::Vector2i deltaPos = currentMousePos - LastMousePos;

                    // Update the user's position based on the mouse movement
                    // Adjust the factor to control the speed of the camera movement
                    UserX -= deltaPos.x * 0.4;
                    UserY -= deltaPos.y * 0.4;

                    LastMousePos = currentMousePos;
                }
        }

        //Update//
        window.setView(view);

        MouseWorldPosition = (sf::Vector2i)window.mapPixelToCoords(sf::Mouse::getPosition(window));

        ImGui::SFML::Update(window, time);

        ProccessInput();

        ProccessVelocityAndPosition();

        //Limiting the user to go below 100 user x (relative to sector size) and 100 user y (relative to sector size)
        if (UserX < 100 * SectorSize)
			UserX = 100 * SectorSize;
        if (UserY < 100 * SectorSize)
            UserY = 100 * SectorSize;

        // Calculate the pixel offset within the current sector
        OffsetX = fmod(UserX, SectorSize);
        OffsetY = fmod(UserY, SectorSize);

        //test window
        if (Debug) {
            ImGui::Begin("Test");
            ImGui::SetWindowPos(ImVec2(0, 0));
            ImGui::SetWindowFontScale(2.0f);
            ImGui::Text(std::string("FPS: " + std::to_string(1.0f / time.asSeconds())).c_str());
            ImGui::Text(std::string("System Probability: " + std::to_string(StarSystemProbability * 100)).c_str());
            ImGui::Text(std::string("Stars visible: " + std::to_string(StarsVisible)).c_str());
            ImGui::Text(std::string("UserX: " + std::to_string((int64_t)UserX / SectorSize)).c_str());
            ImGui::Text(std::string("UserY: " + std::to_string((int64_t)UserY / SectorSize)).c_str());
            ImGui::Text("Sectors drawn: %d", SectorsDrawn);
            ImGui::Text(std::string("View size x: " + std::to_string(view.getSize().x)).c_str());
            ImGui::Text(std::string("View size y: " + std::to_string(view.getSize().y)).c_str());
            ImGui::Text(std::string("View center x: " + std::to_string(view.getCenter().x)).c_str());
            ImGui::Text(std::string("View center y: " + std::to_string(view.getCenter().y)).c_str());
            ImGui::Text(std::string("Sector coords on mouse x: " + std::to_string(SectorCoordsOnMouse.x)).c_str());
            ImGui::Text(std::string("Sector coords on mouse y: " + std::to_string(SectorCoordsOnMouse.y)).c_str());
            ImGui::Text("Debug: %d", Debug);
            //I want to be able to input the coords I want to go to, and then press a button to go there
            static int x = 0;
            static int y = 0;
            ImGui::InputInt("X", &x);
            ImGui::InputInt("Y", &y);
            if (ImGui::Button("Go to")) {
				//setting user x and user y to x and y relative to sector size
                UserX = x * SectorSize;
                UserY = y * SectorSize;
			}

            ImGui::End();
        }
        //end test window

        ProccessVisibleUniverse();
        //End Update//
        

        //Late Update
        time = clock.restart();
        StarSystems.clear();
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


void DrawStarSystems(int64_t l_row, int64_t l_column, int64_t l_startRow, int64_t l_startColumn) {
    //Variables
    sf::RenderWindow& window = *SharedData::GetWindow();
    std::mt19937_64& rng = *SharedData::GetRNG();

    //Creating Star System//
    StarSystem starSystem(l_row, l_column);
    if (!starSystem.HasStar)
		return;
    sf::CircleShape&     starShape        = starSystem.StarShape;
    const sf::FloatRect& starGlobalBounds = starShape.getGlobalBounds();


    starSystem.SetStarPositionInSector(l_row, l_column, l_startColumn, l_startRow);
    StarSystems.push_back(std::make_unique<StarSystem>(starSystem));

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
    sectorShape.setPosition((l_row - l_startColumn) * SectorSize - OffsetX, (l_column - l_startRow) * SectorSize - OffsetY);
    DrawQueue.push_back(std::make_unique<sf::RectangleShape>(sectorShape));

   //Debugging. Draw a circle around the sector if mouse is hovering over it
    if (IsMouseOnSector(l_row, l_column, l_startColumn, l_startRow)) {
		
        sf::CircleShape circleShape(SectorSize / 2);
		circleShape.setFillColor(sf::Color::Transparent);
		circleShape.setOutlineColor(sf::Color::White);
		circleShape.setOutlineThickness(1);
        circleShape.setPosition((l_row - l_startColumn) * SectorSize - OffsetX, (l_column - l_startRow) * SectorSize - OffsetY);

        //If sector has a star system, change circle color to pink, otherwise, keep it white. Take offsetX and offsetY into account
        if (DoesSectorHaveStarSystem(l_row, l_column, l_startColumn, l_startRow, *SharedData::GetRNG()))
			circleShape.setOutlineColor(sf::Color::Magenta);
		else
			circleShape.setOutlineColor(sf::Color::White);

        DrawQueue.push_back(std::make_unique<sf::CircleShape>(circleShape));
	}
}


void DrawStarSelector(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow) {

    if (!IsMouseOnSector(l_row, l_column, l_startColumn, l_startRow))
        return;
    if (!DoesSectorHaveStarSystem(l_row, l_column, l_startColumn, l_startRow, *SharedData::GetRNG()))
        return;

    std::find_if(StarSystems.begin(), StarSystems.end(), [&](const std::unique_ptr<StarSystem>& starSystem) {

		const auto& starPosition = starSystem->StarShape.getPosition();
		const auto& starGlobalBounds = starSystem->StarShape.getGlobalBounds();

		//Checking if mouse is not on star, then continue
		if (starPosition.x < (l_row - l_startColumn) * SectorSize - OffsetX || starPosition.x > (l_row - l_startColumn) * SectorSize + SectorSize - OffsetX ||
            starPosition.y < (l_column - l_startRow) * SectorSize - OffsetY || starPosition.y > (l_column - l_startRow) * SectorSize + SectorSize - OffsetY)
			return false;

        auto& starSelectorShape = starSystem->StarSelectorShape;
        starSelectorShape.setPosition(starPosition.x, starPosition.y);

		if (!starSelectorShape.getGlobalBounds().contains(MouseWorldPosition.x, MouseWorldPosition.y))
			return false;


        DisplayStarSystemInfo(*starSystem);


		DrawQueue.push_back(std::make_unique<sf::RectangleShape>(starSelectorShape));
		return true;

	});
}



void ProccessVisibleUniverse() {
    //Variables
    sf::RenderWindow& window = *SharedData::GetWindow();
    std::mt19937_64& rng = *SharedData::GetRNG();

    // Calculate the range of sectors to render
    int64_t startSectorX = static_cast<int64_t>(UserX / SectorSize) - ViewWidth / SectorSize / 2;
    int64_t startSectorY = static_cast<int64_t>(UserY / SectorSize) - ViewHeight / SectorSize / 2;
    int64_t endSectorX = startSectorX + ViewWidth / SectorSize + 2; // +2 to cover partially visible sectors
    int64_t endSectorY = startSectorY + ViewHeight / SectorSize + 2;

    int64_t sectorX;
    int64_t sectorY;
    uint64_t count     = 0;
    uint64_t starCount = 0;
    for (int64_t x = startSectorX; x < endSectorX; ++x) {
        for (int64_t y = startSectorY; y < endSectorY; ++y) {

            //Proccessing//
            //Sector coords in the universe based on user position in the universe. The one that mouse is hovering over
            if (IsMouseOnSector(x, y, startSectorX, startSectorY)) {
				SectorCoordsOnMouse.x = x;
				SectorCoordsOnMouse.y = y;
			}
            //Checking if sector has a star system and adding it up to StarsVisible
            if (DoesSectorHaveStarSystem(x, y, startSectorX, startSectorY, rng))
                starCount++;

            //Drawing//
            DrawStarSystems(x, y, startSectorX, startSectorY);
            DrawSectorsShape(x, y, startSectorX, startSectorY);
            DrawStarSelector(x, y, startSectorX, startSectorY);

            count++;
        }
        SectorsDrawn = count;
        StarsVisible = starCount;
    }

}


void ProccessDrawQueue() {
	sf::RenderWindow& window = *SharedData::GetWindow();
	for (auto& shape : DrawQueue) 
		window.draw(*shape);
	DrawQueue.clear();
}

void ProccessVelocityAndPosition() {
    sf::Time& time = SharedData::GetTime();
    // Update user position based on velocity
    UserX += VelocityX * time.asSeconds();
    UserY += VelocityY * time.asSeconds();
    //Checking max velocity
    if (VelocityX > MaxVelocity)
        VelocityX = MaxVelocity;
    if (VelocityX < -MaxVelocity)
        VelocityX = -MaxVelocity;
    if (VelocityY > MaxVelocity)
        VelocityY = MaxVelocity;
    if (VelocityY < -MaxVelocity)
        VelocityY = -MaxVelocity;
}

void DisplayStarSystemInfo(const StarSystem& l_starSystem) {
    //Disabling resizing window
    ImGui::Begin("Star System Info", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowSize(ImVec2(SharedData::GetWindow()->getSize().x / 6, SharedData::GetWindow()->getSize().y / 8));
    sf::Vector2i windowSize = sf::Vector2i(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    sf::Vector2i sectorPositionOnScreen = (sf::Vector2i)SharedData::GetWindow()->mapCoordsToPixel(l_starSystem.StarSelectorShape.getPosition());
	ImGui::SetWindowPos(ImVec2((sectorPositionOnScreen.x - l_starSystem.StarSelectorShape.getGlobalBounds().width) - windowSize.x / 2,
                                sectorPositionOnScreen.y + l_starSystem.StarSelectorShape.getGlobalBounds().height));
    ImGui::SetWindowFontScale(2.f);
    ImGui::Text(std::string("Sector Coords: " + std::to_string(SectorCoordsOnMouse.x) + ", " + std::to_string(SectorCoordsOnMouse.y)).c_str());
    ImGui::Text(std::string("Star System Size: " + l_starSystem.GetStarSizeString()).c_str());
    ImGui::Text(std::string("Star System Color: " + l_starSystem.GetStarColorString()).c_str());
	ImGui::End();
}


bool DoesSectorHaveStarSystem(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow, std::mt19937_64& l_rng) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    l_rng.seed(SharedData::Get().GenerateSeed(l_row, l_column));
    return dist(l_rng) < StarSystemProbability;
}


bool IsMouseOnSector(int64_t l_row, int64_t l_column, int64_t l_startColumn, int64_t l_startRow) {
    //Checking if mouse is on sector considering OffsetX and OffsetY
    if (MouseWorldPosition.x >= (l_row - l_startColumn) * SectorSize - OffsetX && MouseWorldPosition.x <= (l_row - l_startColumn) * SectorSize + SectorSize - OffsetX &&
        MouseWorldPosition.y >= (l_column - l_startRow) * SectorSize - OffsetY && MouseWorldPosition.y <= (l_column - l_startRow) * SectorSize + SectorSize - OffsetY)
		return true;
    return false;
}


void ProccessInput() {
    sf::Time& time = SharedData::GetTime();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        VelocityX -= Acceleration * time.asSeconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        VelocityX += Acceleration * time.asSeconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        VelocityY -= Acceleration * time.asSeconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        VelocityY += Acceleration * time.asSeconds();
    }

    //if none of the keys are pressed, apply friction
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		VelocityX *= Friction;
		VelocityY *= Friction;
	}

}
