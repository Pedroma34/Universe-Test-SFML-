#include "pch.h"
#include "Application.h"

uvy::Application::Application() : 
	m_window({ sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height}),
	m_debug(false)
{
	//Random//
	m_rng.seed(m_rd());
	
	//Setting SharedData//
	SharedData::SetApplication(this);
	SharedData::SetTime(&m_time);
	SharedData::SetDebug(&m_debug);
	SharedData::SetRNG(&m_rng);
	SharedData::SetRD(&m_rd);

	//StateManager//
	m_stateManager.SwitchTo<StateGame>();

}

uvy::Application::~Application() {}

void uvy::Application::Update() {
	m_window.Update();
	m_stateManager.Update();
}

void uvy::Application::LateUpdate() {
	//Time//
	m_time = m_clock.restart();

	//StateManager//
	m_stateManager.LateUpdate();
}

void uvy::Application::Render() {
	m_window.Clear();
	m_stateManager.Render();
	m_window.Display();
}

bool uvy::Application::IsRunning() const {
	return m_window.IsOpen();
}