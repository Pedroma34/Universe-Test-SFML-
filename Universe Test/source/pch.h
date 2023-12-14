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

//Hasing


struct VisibleWorldCoords {
	int64_t Row = 0;
	int64_t Column = 0;
	int64_t StartRow = 0;
	int64_t StartColumn = 0;
};

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


		// Cantor pairing function adapted for negative coordinates. Doesn't work well with large numbers. So it's not used.
	uint64_t CantorPairing(int64_t x, int64_t y) {
		uint64_t X = MapToint64_t(x);
		uint64_t Y = MapToint64_t(y);
		return 0.5 * (X + Y) * (X + Y + 1) + Y;
	}


	// Generate a seed for a given coordinate. Alternative to Cantor pairing function. Works well with large numbers.
	uint64_t GenerateSeed(int64_t x, int64_t y) {
		return static_cast<uint64_t>(x) * 0x1f1f1f1f1f1f1f1fULL ^ static_cast<uint64_t>(y);
	}
	

	//Setters
	static void SetRNG(std::mt19937_64* l_rng) {	Get().m_rng = l_rng;	}
	static void SetStarSystemProbability(double* l_starSystemProbability) {	Get().m_starSystemProbability = l_starSystemProbability;}
	static void SetSectorSize(const int64_t* l_sectorSize) {	Get().m_sectorSize = l_sectorSize;	}
	static void SetWindow(sf::RenderWindow* l_window) {	Get().m_window = l_window;	}
	static void SetTime(sf::Time* l_time) {	Get().m_time = l_time;	}
	static void SetView(sf::View* l_view) {	Get().m_view = l_view;	}
	static void SetUserX(double* l_userX) {	Get().m_userX = l_userX;	}
	static void SetUserY(double* l_userY) {	Get().m_userY = l_userY;	}

	//Getters
	static std::mt19937_64* GetRNG() {	return Get().m_rng;	}
	const static double& GetStarSystemProbability() {	return *Get().m_starSystemProbability; }
	const static int64_t& GetSectorSize() {	return *Get().m_sectorSize;	}
	static sf::RenderWindow* GetWindow() {	return Get().m_window;	}
	static sf::Time& GetTime() {	return *Get().m_time;	}
	static sf::View& GetView() {	return *Get().m_view;	}
	static double& GetUserX() {	return *Get().m_userX;	}
	static double& GetUserY() {	return *Get().m_userY;	}

private:
	// Function to map int64_tegers to non-negative int64_tegers for Cantor pairing
	uint64_t MapToint64_t(int64_t x) {
		return x >= 0 ? 2 * x : -2 * x - 1;
	}

private:
	std::mt19937_64* m_rng = nullptr;
	double* m_starSystemProbability = nullptr;
	const int64_t* m_sectorSize = nullptr;
	sf::RenderWindow* m_window = nullptr;
	sf::Time * m_time = nullptr;
	sf::View* m_view = nullptr;
	double *m_userX = nullptr;
	double *m_userY = nullptr;

};