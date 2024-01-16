#include "pch.h"
#include "Window.h"
#include "States/StateManager.h"
#include "Camera.h"
#include "Universe.h"
#include "StarSystem/StarSystem.h"

namespace uvy {
	Window::Window(const sf::Vector2<uint64_t>& l_size)
		: m_isOpen(true), m_isFullscreen(true) {
		SharedData::SetWindow(this);
		Create(l_size);
		ImGui::SFML::Init(m_window);
	}

	Window::~Window() {}

	void Window::Update() {
		sf::Event event;

		while (m_window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				m_isOpen = false;

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F11)
				SetFullscreen(!m_isFullscreen);

			else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1)
				SharedData::GetDebug() = !SharedData::GetDebug();

			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
				SetRandomLocationInUniverse();

			if (event.type == sf::Event::MouseWheelScrolled)
				ZoomIn(event.mouseWheelScroll.delta);

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				Universe& uni = SharedData::GetUniverse();
				std::shared_ptr<StarSystem> starUnderMouse = uni.GetStarUnderMouse();
				if (starUnderMouse != nullptr) {
					if (starUnderMouse->GetIsModified()) {
						uni.SetSelectedModifiedStar(starUnderMouse); //Set pointer to point to the star system that has been modified
						uni.SetSelectedStar(std::weak_ptr<StarSystem>()); //Set pointer to point to the star system that has been modified)
					}
					else {
						uni.SetSelectedStar(starUnderMouse); //Creates a pointer to the procedually generated star system that has been selected
						uni.SetSelectedModifiedStar(std::weak_ptr<StarSystem>()); //Set pointer to point to the star system that has been modified)
					}
				}
			}

		}

		ImGui::SFML::Update(m_window, sf::seconds(1.0f / 60.0f));
	}

	void Window::Destroy() {
		m_isOpen = false;
		m_window.close();
	}

	void Window::Clear() {
		m_window.clear();
	}

	void Window::Draw(const sf::Drawable& l_drawable) {
		m_window.draw(l_drawable);
	}

	void Window::Display() {
		ImGui::SFML::Render(m_window);
		m_window.display();
	}

	void Window::SetFullscreen(bool l_fullscreen) {
		m_isFullscreen = l_fullscreen;
		m_window.close();
		Create(GetSize());
	}

	void Window::SetView(const sf::View& l_view) {
		m_window.setView(l_view);
	}

	const bool Window::IsOpen() const {
		return m_isOpen;
	}

	const bool Window::IsFullscreen() const {
		return m_isFullscreen;
	}

	const sf::Vector2<uint64_t> Window::GetSize() const {
		return sf::Vector2<uint64_t>(m_window.getSize().x, m_window.getSize().y);
	}

	sf::RenderWindow& Window::GetWindow() {
		return m_window;
	}

	const sf::View& Window::GetDefaultView() const {
		return m_window.getDefaultView();
	}

	void Window::Create(const sf::Vector2<uint64_t>& l_size) {

		auto style = m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Close;
		m_window.create(sf::VideoMode(l_size.x, l_size.y), "Universy", style);
		m_window.setVerticalSyncEnabled(true);

	}

	void Window::ZoomIn(const float& l_delta) {


		StateManager&			  stateMgr = SharedData::GetStateManager();
		std::weak_ptr<StateGame> stateGame = std::static_pointer_cast<StateGame>(stateMgr.GetState("StateGame").lock());

		if (stateGame.expired())
			return;

		sf::View& gameView				  = stateGame.lock()->GetView();
		const sf::Vector2f& viewSize	  = gameView.getSize();
		auto camera						  = stateGame.lock()->GetCamera().lock();
		const sf::Vector2f& zoomOutLimit  = camera->GetZoomOutLimit();
		const sf::Vector2f& zoomInLimit   = camera->GetZoomInLimit();

		float zoomInValue = 0.95f;
		float zoomOutValue = 1.05f;

		if (l_delta > 0) {

			sf::Vector2f nextZoom(viewSize.x * zoomInValue, viewSize.y * zoomInValue);

			//Checking if next zoom is less than zoom in limit//
			if (nextZoom.x < zoomInLimit.x || nextZoom.y < zoomInLimit.y) {
				gameView.setSize(camera->GetZoomInLimit()); //Setting zoom in limit
				return;
			}

			gameView.zoom(zoomInValue);
		}
		else if (l_delta < 0) {

			sf::Vector2f nextZoom(viewSize.x * zoomOutValue, viewSize.y * zoomOutValue);

			//Checking if next zoom is less than zoom in limit//
			if (nextZoom.x > zoomOutLimit.x || nextZoom.y > zoomOutLimit.y) {
				gameView.setSize(camera->GetZoomOutLimit()); //Setting zoom in limit
				return;
			}

			gameView.zoom(zoomOutValue);
		}
	}
	void Window::SetRandomLocationInUniverse() {

		StateManager& stateMgr = SharedData::GetStateManager();
		std::weak_ptr<StateGame> stateGame = std::static_pointer_cast<StateGame>(stateMgr.GetState("StateGame").lock());

		if (stateGame.expired())
			return;

		sf::View& gameView = stateGame.lock()->GetView();
		Camera& camera     = *stateGame.lock()->GetCamera().lock();

		SharedData::GetRNG().seed(SharedData::GetRD()());
		std::uniform_int_distribution<int64_t> locationXdist(camera.GetThresholdLeft().x + 10, camera.GetThresholdRight().x - 10);
		std::uniform_int_distribution<int64_t> locationYdist(camera.GetThresholdLeft().y + 10, camera.GetThresholdRight().y - 10);

		sf::Vector2<int64_t> position(locationXdist(SharedData::GetRNG()), locationYdist(SharedData::GetRNG()));
		const sf::Vector2<int64_t>& sectorSize = SharedData::GetUniverse().GetSectorSize();
		gameView.setCenter(position.x * sectorSize.x, position.y * sectorSize.y);
	}
}
