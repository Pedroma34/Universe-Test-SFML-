#include "pch.h"
#include "StateGame.h"
#include "Universe.h"
#include "Camera.h"
#include "Application.h"
#include "StarSystem/StarSystem.h"

namespace uvy {
	void StateGame::OnCreate() {

		//Name//
		SetName("StateGame");

		//View and Camera//
		m_view.setSize(1280, 720);
		m_camera = std::make_shared<Camera>();
		m_camera->SetOriginalViewSize(m_view.getSize());
		float zoomOutModifier = 4.f;
		float zoomInModifier = 0.6f;
		m_camera->SetZoomOutLimit(sf::Vector2f(m_camera->GetOriginalViewSize().x * zoomOutModifier, m_camera->GetOriginalViewSize().y * zoomOutModifier));
		m_camera->SetZoomInLimit(sf::Vector2f(m_camera->GetOriginalViewSize().x * zoomInModifier, m_camera->GetOriginalViewSize().y * zoomInModifier));
		m_camera->SetAcceleration(2800.f);
		m_camera->SetMaxVelocity(sf::Vector2f(2000.f, 2000.f));
		m_camera->SetFriction(2800.f); // slows down in pixels per second
		m_camera->SetThresholdLeft(sf::Vector2<int64_t>(-1000, -1000));	  //Universe limit for camera
		m_camera->SetThresholdRight(sf::Vector2<int64_t>(1000, 1000));	  //Universe limit for camera

		//Universe//
        m_universe = std::make_shared<Universe>();
		m_universe->SetSectorSize(sf::Vector2<int64_t>(150, 150));
		m_universe->SetStarSystemChance(0.25f);
		
		//Setting user position in the universe//
		SharedData::GetRNG().seed(SharedData::GetRD()());
		std::uniform_int_distribution<int64_t> locationXdist(m_camera->GetThresholdLeft().x / 2, m_camera->GetThresholdRight().x / 2);
		std::uniform_int_distribution<int64_t> locationYdist(m_camera->GetThresholdLeft().y / 2, m_camera->GetThresholdRight().y / 2);

		sf::Vector2<int64_t> position(locationXdist(SharedData::GetRNG()), locationYdist(SharedData::GetRNG()));
		const sf::Vector2<int64_t>& sectorSize = m_universe->GetSectorSize();
		m_view.setCenter(position.x * sectorSize.x, position.y * sectorSize.y);

	}

	void StateGame::OnDestroy() {}

	void StateGame::Activate() {}

	void StateGame::Deactivate() {}

	void StateGame::Update() {
		
		SharedData::GetWindow().SetView(m_view);
		
		m_camera->Update(m_view);
		m_universe->Update(m_view);


		ImGui();


		HandleInput();

	}

	void StateGame::Draw() {
		m_universe->Render(m_view);
		m_camera->Draw();
	}

	std::weak_ptr<class Universe> StateGame::GetUniverse() {
		return m_universe;
	}

	std::weak_ptr<class Camera> StateGame::GetCamera() {
		return m_camera;
	}

	void StateGame::ImGui() {
		DebugImGui();
		MenuImGui();
		SelectedStarGui();
	}

	void StateGame::DebugImGui() {
		if (!SharedData::GetDebug())
			return;

		const sf::Vector2<uint64_t> winSize = SharedData::GetWindow().GetSize();
		const sf::Vector2<int64_t>& sectorSize = m_universe->GetSectorSize();
		auto			             position = sf::Vector2<int64_t>(floor(m_view.getCenter().x / sectorSize.x), floor(m_view.getCenter().y / sectorSize.y));

		ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		ImGui::SetWindowFontScale(SharedData::GetWindowFontScale());
		//ImGui::SetWindowSize(ImVec2(winSize.x * 0.20, winSize.y * 0.80));
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::Text("Sectors drawn: %d", m_universe->GetSectorsDrawn());
		ImGui::Text("Stars drawn: %d", m_universe->GetStarsDrawn());
		ImGui::Text("Stars modified: %d", m_universe->GetStarsModified());
		std::weak_ptr<StarSystem> selectedStar = m_universe->GetSelectedStar();
		ImGui::Text("Has star been selected: %s", selectedStar.expired() ? "false" : "true");
		static bool drawSectors = m_universe->GetDrawSectors();
		if (ImGui::Checkbox("Draw Sectors", &drawSectors))
			m_universe->SetDrawSectors(drawSectors);
		static float chanceSlider = SharedData::GetUniverse().GetStarSystemChance();
		if (ImGui::SliderFloat("Star system chance", &chanceSlider, 0.0f, 1.0f, "%.2f"))
			SharedData::GetUniverse().SetStarSystemChance(chanceSlider);
		ImGui::Text("View Size: %.1f, %.1f", m_view.getSize().x, m_view.getSize().y);
		ImGui::Text("Sector Under Mouse: %d, %d", m_universe->GetMouseSector().x, m_universe->GetMouseSector().y);
		static sf::Vector2<int32_t> sectorInput = static_cast<sf::Vector2<int32_t>>(m_universe->GetSectorSize());
		ImGui::InputInt("Sector Size x", &sectorInput.x);
		ImGui::InputInt("Sector Size y", &sectorInput.y);
		if (ImGui::Button("Set Sector Size"))
			m_universe->SetSectorSize(sf::Vector2<int64_t>(sectorInput.x, sectorInput.y));
		ImGui::Text("Camera Position in Universe: %d, %d", position.x, position.y);
		ImGui::Text("Camera Position: %.1f, %1.f", m_view.getCenter().x, m_view.getCenter().y);
		ImGui::Text("Camera Zoom: %.1f", m_view.getSize().x / m_camera->GetOriginalViewSize().x);
		ImGui::Text("Camera Velocity: %.1f, %.1f", m_camera->GetVelocity().x, m_camera->GetVelocity().y);
		ImGui::Text("Camera Max Velocity: %.1f, %.1f", m_camera->GetMaxVelocity().x, m_camera->GetMaxVelocity().y);
		ImGui::Text("Camera Acceleration: %.1f", m_camera->GetAcceleration());
		ImGui::Text("Camera Friction: %.1f", m_camera->GetFriction());
		static float acc = m_camera->GetAcceleration();
		if (ImGui::SliderFloat("Camera Acceleration", &acc, 0.f, 10000.f, "%.1f"))
			m_camera->SetAcceleration(acc);
		static float fric = m_camera->GetFriction();
		if (ImGui::SliderFloat("Camera Friction", &fric, 0.f, 10000.f, "%.1f"))
			m_camera->SetFriction(fric);
		static float maxVel = m_camera->GetMaxVelocity().x;
		if (ImGui::SliderFloat("Camera Max Velocity", &maxVel, 0.f, 10000.f, "%.1f"))
			m_camera->SetMaxVelocity(sf::Vector2f(maxVel, maxVel));
		ImGui::Text("Zoom Out Limit: %.1f, %.1f", m_camera->GetZoomOutLimit().x, m_camera->GetZoomOutLimit().y);
		ImGui::Text("Zoom In Limit: %.1f, %.1f", m_camera->GetZoomInLimit().x, m_camera->GetZoomInLimit().y);
		static bool seeCameraShape = m_camera->GetSeeCameraShape();
		if (ImGui::Checkbox("See Camera Shape", &seeCameraShape))
			m_camera->SetSeeCameraShape(seeCameraShape);
		ImGui::Text("Is Out Of Bounds On Width: %s", m_camera->GetOutOfBoundsOnWidth() ? "true" : "false");
		ImGui::Text("Is Out Of Bounds On Height: %s", m_camera->GetOutOfBoundsOnHeight() ? "true" : "false");
		ImGui::End();
	}

	void StateGame::MenuImGui() {

		const sf::Vector2<uint64_t> winSize    = SharedData::GetWindow().GetSize();
		const sf::Vector2<int64_t>& sectorSize = m_universe->GetSectorSize();
		static sf::Vector2<int32_t> position(0, 0);

		ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove);
		ImGui::SetWindowFontScale(SharedData::GetWindowFontScale());
		//ImGui::SetWindowSize(ImVec2(winSize.x * 0.20, winSize.y * 0.10));
		ImVec2 thisWindowSize = ImGui::GetWindowSize();
		ImGui::SetWindowPos(ImVec2(winSize.x - thisWindowSize.x, 0));
		ImGui::Checkbox("Debug (or F1)", &SharedData::GetDebug());
		ImGui::InputInt("X", &position.x);
		ImGui::InputInt("Y", &position.y);
		if (ImGui::Button("Set Position"))
			m_view.setCenter(position.x * sectorSize.x, position.y * sectorSize.y);
		//Showing controls
		ImGui::Text("Controls:");
		ImGui::Text("WASD: Move camera");
		ImGui::Text("Mouse Wheel: Zoom in/out");
		ImGui::Text("F1: Toggle debug");
		ImGui::Text("F11: Toggle fullscreen");
		ImGui::End();


	}

	void StateGame::SelectedStarGui() {
		
		Universe& uni = SharedData::GetUniverse();

		std::shared_ptr<StarSystem> selectedStar = uni.GetSelectedStar();

		if (selectedStar == nullptr)
			return;

		const sf::Vector2<uint64_t> winSize = SharedData::GetWindow().GetSize();

		ImGui::Begin("Selected Star", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | 
		ImGuiWindowFlags_NoFocusOnAppearing);
		ImGui::SetWindowFontScale(SharedData::GetWindowFontScale());
		ImVec2 thisWindowSize = ImGui::GetWindowSize();
		ImGui::SetWindowSize(ImVec2(winSize.x * 0.30, winSize.y * 0.30));
		ImGui::SetWindowPos(ImVec2(winSize.x / 2 - thisWindowSize.x / 2, winSize.y - thisWindowSize.y));
		if(SharedData::GetDebug())
			ImGui::Text("Star ID: %d", selectedStar->GetID());
		ImGui::Text("Star Position: %d, %d", selectedStar->GetSectorPosition().x, selectedStar->GetSectorPosition().y);
		ImGui::Text("Star Size: %s", selectedStar->GetStarSizeString().c_str());
		ImGui::Text("Star Color: %s", selectedStar->GetStarColorString().c_str());
		if (ImGui::Button("Close"))
			uni.SetSelectedStar(std::weak_ptr<StarSystem>());
		ImGui::End();
	}

	void StateGame::HandleInput() {

	}

}
