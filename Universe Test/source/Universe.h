#pragma once



#include "pch.h"



namespace uvy {



	using StarSystemContainer = std::unordered_map<int64, std::shared_ptr<class StarSystem>>;



	//////////////////////////////////////////////////////////////////////////
	/// Handles all of the procedually generated universe
	//////////////////////////////////////////////////////////////////////////
	class Universe {
	public:



		Universe();



		~Universe();



		//////////////////////////////////////////////////////////////////////////
		/// Process univere sectors and its components among other things
		//////////////////////////////////////////////////////////////////////////
		void Update(sf::View &l_view);



		void Render(sf::View& l_view);



		//////////////////////////////////////////////////////////////////////////
		/// Checks if this sector has a star system that has been modified and
		/// therefore is in memory.
		/// @returns A weak pointer to the star system if it exists.
		//////////////////////////////////////////////////////////////////////////
		std::weak_ptr<class StarSystem> DoesSectorHaveStarModified(const int64_t& l_x, const int64_t& l_y);


		//////////////////////////////////////////////////////////////////////////
		///	Adds the star system to the list of star systems that 
		/// have been modified.
		/// @returns A weak pointer to the star system that was added.
		//////////////////////////////////////////////////////////////////////////
		std::weak_ptr<class StarSystem> AddStarToModify(std::weak_ptr<class StarSystem> l_starSystem);



	private:
		//////////////////////////////////////////////////////////////////////////
		/// Goes through every sector in the visible view
		/// and performs logic.
		//////////////////////////////////////////////////////////////////////////
		void ProcessSectors(const sf::Vector2i& l_sectorOnTopLeftView, const sf::Vector2i& l_sectorOnBottomRightView);



		bool GenerateStarSystem(const int64_t& l_x, const int64_t& l_y, int64_t& l_starsDrawn);



		void DrawSector(const int64_t& l_x, const int64_t& l_y);



		void UpdateMousePosition(const int64_t& l_x, const int64_t& l_y);



		bool IsMouseInSector(const int64_t& l_x, const int64_t& l_y);



		//////////////////////////////////////////////////////////////////////////
		/// Draws selector on top of the star that the mouse is hoverin on
		//////////////////////////////////////////////////////////////////////////
		void MouseHoverStar();



		//////////////////////////////////////////////////////////////////////////
		/// @returns The unique seed for the sector based on the x and y
		//////////////////////////////////////////////////////////////////////////
		int64 GetSeed(const int64_t& l_x, const int64_t& l_y);



	public:
		//Setters//

		void SetSectorSize(const sf::Vector2<int64_t>& l_sectorSize);
		void SetStarSystemChance(const float l_starSystemChance);
		void SetDrawSectors(const bool l_drawSectors);



		//Getters//

		//////////////////////////////////////////////////////////////////////////
		/// returns the star that the mouse is hovering on. Note that stars
		/// need to be added to the modified list if you want to change them.
		/// @returns A weak pointer to the star system that the mouse is hovering.
		//////////////////////////////////////////////////////////////////////////
		std::shared_ptr<class StarSystem> GetStarUnderMouse();
		const sf::Vector2<int64_t>& GetSectorSize()  const;
		const float GetStarSystemChance()			 const;
		const uint64_t& GetSectorsDrawn()		     const;
		const uint64_t& GetStarsDrawn()			     const;
		uint64 GetStarsModified()					 const;
		const sf::Vector2<int64_t>& GetMouseSector() const;
		const bool GetDrawSectors()					 const;
	private:



		sf::Vector2<int64_t>					   m_sectorSize;
		float									   m_starSystemChance;
		std::vector<std::unique_ptr<sf::Drawable>> m_drawQueue;
		uint64_t								   m_sectorsDrawn;
		uint64_t								   m_starsDrawn;
		StarSystemContainer						   m_starSystems; //Stars systems that were modified
		sf::Vector2<int64_t>					   m_mouseSector; //Which sector is mouse in
		bool									   m_drawSectors;



	};
}