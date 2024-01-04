#pragma once



#include "pch.h"
#include "States/StateManager.h"



namespace uvy {



	//////////////////////////////////////////////////////////////////////////
	/// Encapsulates all the main components of the application.
	//////////////////////////////////////////////////////////////////////////
	class Application {
	public:



		Application();



		~Application();



		//////////////////////////////////////////////////////////////////////////
		/// Updates all the components of the application.
		//////////////////////////////////////////////////////////////////////////
		void Update();



		//////////////////////////////////////////////////////////////////////////
		/// Performs the late update of all the components of the application.
		///	For example, the late updates are useful to delete certain
		/// components that are no longer needed, such as the entities and states.
		//////////////////////////////////////////////////////////////////////////
		void LateUpdate();



		void Render();



		//Getters//



		bool IsRunning() const;
	private:



		std::mt19937_64    m_rng;			//Random engine for the universe
		std::random_device m_rd;			//Random device for true randomness when needed
		Window		       m_window;
		sf::Time	       m_time;
		sf::Clock	       m_clock;
		StateManager       m_stateManager;
		bool		       m_debug;
	
	
	
	};
}