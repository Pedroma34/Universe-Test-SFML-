#pragma once



#include "pch.h"


namespace uvy {



	//////////////////////////////////////////////////////////////////////////
	/// Singleton class that holds all the pointers to the
	/// main components of the application.
	//////////////////////////////////////////////////////////////////////////
	class SharedData {
	private:



		SharedData();



		SharedData(const SharedData&) = delete;



		SharedData& operator=(const SharedData&) = delete;



	public:
		static SharedData& Get();



		//////////////////////////////////////////////////////////////////////////
		/// Generates an unique seed based on x and y
		//////////////////////////////////////////////////////////////////////////
		int64_t GenerateSeed(int64_t l_x, int64_t l_y);



		//////////////////////////////////////////////////////////////////////////
		/// Returns a scaled font size based on the window size
		//////////////////////////////////////////////////////////////////////////
		static float GetWindowFontScale();



		//Setters//

		static void SetApplication (class Application*        l_application );
		static void SetWindow	   (class Window*             l_window	    );
		static void SetStateManager(class StateManager*       l_stateManager);
		static void SetTime		   (class sf::Time*           l_time		);
		static void SetDebug       (bool*				      l_debug		);
		static void SetRNG		   (std::mt19937_64*          l_rng			);
		static void SetUniverse	   (class Universe*           l_universe	);
		static void SetRD		   (class std::random_device* l_rd		    );



		//Getters

		static class Application&  GetApplication();
		static class Window&	   GetWindow();
		static class StateManager& GetStateManager();
		static sf::Time&		   GetTime();
		static bool&			   GetDebug();
		static std::mt19937_64&    GetRNG();
		static class Universe&     GetUniverse();
		static std::random_device& GetRD();




	private:
		class Application* m_application   = nullptr;
		class Window* m_window			   = nullptr;
		class StateManager* m_stateManager = nullptr;
		class sf::Time* m_time			   = nullptr;
		std::mt19937_64* m_rng			   = nullptr;
		class std::random_device* m_rd	   = nullptr;
		bool* m_debug					   = nullptr;
		class Universe* m_universe		   = nullptr;
	};

}