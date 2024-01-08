#pragma once

#include "State.h"


namespace uvy {
	
	class StateGame : public State {
	public:
		StateGame() {}
		~StateGame() {}

		void OnCreate();
		void OnDestroy();
		void Activate();
		void Deactivate();
		void Update();
		void Draw();

		//Getters//

		std::weak_ptr<class Universe> GetUniverse();
		std::weak_ptr<class Camera>	  GetCamera();

	private:
		void ImGui();
		void DebugImGui();
		void MenuImGui();
		void SelectedStarGui();
		void HandleInput();

	private:
		std::shared_ptr<class Universe> m_universe;
		std::shared_ptr<class Camera>	m_camera;
	};
}