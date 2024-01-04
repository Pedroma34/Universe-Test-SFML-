


#include "pch.h"
#include "Application.h"



uvy::Application::Application() : 

	m_window({ sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height}),
	m_debug(false)

{
	//////////////////////////////////////////////////////////////////////////
	/// Setting random seed based on the device.
	//////////////////////////////////////////////////////////////////////////
	m_rng.seed(m_rd());
	
	//////////////////////////////////////////////////////////////////////////
	/// Setting pointers in our shared data.
	//////////////////////////////////////////////////////////////////////////
	SharedData::SetApplication(this);
	SharedData::SetTime		  (&m_time);
	SharedData::SetDebug	  (&m_debug);
	SharedData::SetRNG		  (&m_rng);
	SharedData::SetRD		  (&m_rd);

	//////////////////////////////////////////////////////////////////////////
	/// Starting domino effect with the states.
	//////////////////////////////////////////////////////////////////////////
	m_stateManager.SwitchTo<StateGame>();

}



uvy::Application::~Application() {}



void uvy::Application::Update() {

	m_window.Update();

	m_stateManager.Update();

}



void uvy::Application::LateUpdate() {

	//////////////////////////////////////////////////////////////////////////
	/// Restarting the clock and updating the time in order to have a
	/// constant time step.
	//////////////////////////////////////////////////////////////////////////
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