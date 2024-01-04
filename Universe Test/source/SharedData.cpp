


#include "pch.h"
#include "SharedData.h"

uvy::SharedData::SharedData() {}

uvy::SharedData& uvy::SharedData::Get() {
	static SharedData instance;
	return instance;
}

int64_t uvy::SharedData::GenerateSeed(int64_t l_x, int64_t l_y) {
	return static_cast<int64_t>(l_x) * 0x1f1f1f1f1f1f1f1fULL ^ static_cast<int64_t>(l_y);
}

float uvy::SharedData::GetWindowFontScale() {
	const sf::Vector2<uint64_t> winSize = Get().GetWindow().GetSize();

	if (winSize.x <= 1280)
		return 1.2f;
	else if (winSize.x > 1280 && winSize.x <= 1920)
		return 1.4f;
	else
		return 2.f;
}

void uvy::SharedData::SetApplication(Application* l_application) {
	Get().m_application = l_application;
}

void uvy::SharedData::SetWindow(Window* l_window) {
	Get().m_window = l_window;
}

void uvy::SharedData::SetStateManager(StateManager* l_stateManager) {
	Get().m_stateManager = l_stateManager;
}

void uvy::SharedData::SetTime(sf::Time* l_time) {
	Get().m_time = l_time;
}

void uvy::SharedData::SetDebug(bool* l_debug) {
	Get().m_debug = l_debug;
}

void uvy::SharedData::SetRNG(std::mt19937_64* l_rng) {
	Get().m_rng = l_rng;
}

void uvy::SharedData::SetUniverse(Universe* l_universe) {
	Get().m_universe = l_universe;
}

void uvy::SharedData::SetRD(std::random_device* l_rd) {
	Get().m_rd = l_rd;
}

uvy::Application& uvy::SharedData::GetApplication() {
	return *Get().m_application;
}

uvy::Window& uvy::SharedData::GetWindow() {
	return *Get().m_window;
}

uvy::StateManager& uvy::SharedData::GetStateManager() {
	return *Get().m_stateManager;
}

sf::Time& uvy::SharedData::GetTime() {
	return *Get().m_time;
}

bool& uvy::SharedData::GetDebug() {
	return *Get().m_debug;
}

std::mt19937_64& uvy::SharedData::GetRNG() {
	return *Get().m_rng;
}

uvy::Universe& uvy::SharedData::GetUniverse() {
	return *Get().m_universe;
}

std::random_device& uvy::SharedData::GetRD() {
	return *Get().m_rd;
}
