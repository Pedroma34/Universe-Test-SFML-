#pragma once

//SFML
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"


//Log
#include "spdlog/spdlog.h"

//ImGui
#include "imgui.h"
#include "imgui-SFML.h"

//String
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ostream>
#include <typeindex>


//Memory
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <array>
#include <stack>


//Math
#include <algorithm>
#include <random>
#include <limits>

//Windows
#include <Windows.h>

//Mine
#include "Window.h"

namespace uvy {

	//Singleton shared data
	class SharedData {

	private:
		SharedData() {}
		SharedData(const SharedData&) = delete;
		SharedData& operator=(const SharedData&) = delete;

	public:
		static SharedData& Get() {
			static SharedData instance;
			return instance;
		}

		// Generate a seed for a given coordinate
		int64_t GenerateSeed(int64_t x, int64_t y) {
			return static_cast<int64_t>(x) * 0x1f1f1f1f1f1f1f1fULL ^ static_cast<int64_t>(y);
		}

		//Returns a scale of ImGui window relative to the window size
		static float GetWindowFontScale() {
			const sf::Vector2<uint64_t> winSize = Get().GetWindow().GetSize();

			if (winSize.x <= 1280)
				return 1.2f;
			else if (winSize.x > 1280 && winSize.x <= 1920)
				return 1.4f;
			else
				return 2.f;
		}

		//Setters//

		static void SetApplication(class Application* l_application)	{ Get().m_application = l_application; }
		static void SetWindow(class Window* l_window)					{ Get().m_window = l_window; }
		static void SetStateManager(class StateManager* l_stateManager) { Get().m_stateManager = l_stateManager; }
		static void SetTime(sf::Time* l_time)							{ Get().m_time   = l_time;   }
		static void SetDebug(bool* l_debug)								{ Get().m_debug  = l_debug;  }
		static void SetRNG(std::mt19937_64* l_rng)						{ Get().m_rng    = l_rng;    }
		static void SetUniverse(class Universe* l_universe)				{ Get().m_universe = l_universe; }
		static void SetRD(std::random_device* l_rd)						{ Get().m_rd = l_rd; }

		//Getters

		static class Application& GetApplication()	 { return *Get().m_application; }
		static class Window& GetWindow()			 { return *Get().m_window; }
		static class StateManager& GetStateManager() { return *Get().m_stateManager; }
		static sf::Time& GetTime()					 { return *Get().m_time;   }
		static bool& GetDebug()						 { return *Get().m_debug;  }
		static std::mt19937_64& GetRNG()			 { return *Get().m_rng;    }
		static class Universe& GetUniverse()	     { return *Get().m_universe; }
		static std::random_device& GetRD()			 { return *Get().m_rd; }

	private:
		class Application*	  m_application		 = nullptr;
		class Window*		  m_window			 = nullptr;
		class StateManager*   m_stateManager	 = nullptr;
		sf::Time*			  m_time		     = nullptr;
		std::mt19937_64*	  m_rng				 = nullptr;
		std::random_device*	  m_rd			     = nullptr;
		bool*				  m_debug		     = nullptr;
		class Universe*		  m_universe= nullptr;
	};

}