#pragma once
#include "pch.h"

namespace uvy {
	class Universe {
	public:
		Universe();
		~Universe();

		void Update(sf::View &l_view);
		void Render(sf::View& l_view);

		//Setters//

		void SetSectorSize(const sf::Vector2<int64_t>& l_sectorSize);
		void SetStarSystemChance(const float l_starSystemChance);
		void SetDrawSectors(const bool l_drawSectors);

		//Getters//

		const sf::Vector2<int64_t>& GetSectorSize()  const;
		const float GetStarSystemChance()			 const;
		const uint64_t& GetSectorsDrawn()		     const;
		const uint64_t& GetStarsDrawn()			     const;
		const sf::Vector2<int64_t>& GetMouseSector() const;
		const bool GetDrawSectors()					 const;

	private:
		void ProcessSectors(const sf::Vector2i& l_sectorOnTopLeftView, const sf::Vector2i& l_sectorOnBottomRightView);
		bool GenerateStarSystem(const int64_t& l_x, const int64_t& l_y, int64_t& l_starsDrawn);
		void DrawSector(const int64_t& l_x, const int64_t& l_y);
		void UpdateMousePosition(const int64_t& l_x, const int64_t& l_y);
		bool IsMouseInSector(const int64_t& l_x, const int64_t& l_y);
		void MouseHoverStar(); //Draws selector on top of the star that the mouse is hoverin on


	private:
		sf::Vector2<int64_t>					   m_sectorSize;
		float									   m_starSystemChance;
		std::vector<std::unique_ptr<sf::Drawable>> m_drawQueue;
		uint64_t								   m_sectorsDrawn;
		uint64_t								   m_starsDrawn;
		sf::Vector2<int64_t>					   m_mouseSector; //Which sector is mouse in
		bool									   m_drawSectors;
	};
}