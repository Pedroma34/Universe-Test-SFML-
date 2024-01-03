#pragma once
#include "pch.h"
#include "StateManager.h"

namespace uvy {

	class Application {

	public:

		Application();
		~Application();

		void Update();
		void LateUpdate();

		void Render();

		//Getters//
		bool IsRunning() const;

	private:
		std::mt19937_64    m_rng;
		std::random_device m_rd;
		Window		       m_window;
		sf::Time	       m_time;
		sf::Clock	       m_clock;
		StateManager       m_stateManager;
		bool		       m_debug;
	};
}