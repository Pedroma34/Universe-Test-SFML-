#pragma once
#include "pch.h"

namespace uvy {
	class Window {
	public:

		Window(const sf::Vector2<uint64_t>& l_size);
		~Window();

		///////////////////////////
		///	Update window events
		///////////////////////////
		void Update();


		void Clear();
		void Draw(const sf::Drawable& l_drawable);
		void Display();

		//Setters//

		void SetFullscreen(bool l_fullscreen);
		void SetView(const sf::View& l_view);

		//Getters//

		const bool			  IsOpen()		 const;
		const bool		      IsFullscreen() const;
		const sf::Vector2<uint64_t> GetSize()		 const;
		sf::RenderWindow& GetWindow();
		const sf::View& GetDefaultView() const;

	private:
		void Create(const sf::Vector2<uint64_t>& l_size);
		void ZoomIn(const float& l_delta);
		void SetRandomLocationInUniverse();

	private:
		sf::RenderWindow m_window;
		bool			 m_isOpen;
		bool			 m_isFullscreen;
	};
}