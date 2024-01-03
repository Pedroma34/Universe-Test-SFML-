#pragma once
#include "pch.h"

enum class StarSize				{ Small, Medium, Large, HyperLarge };
enum class StarColor			{ Red, Orange, White, Blue, Purple };
enum class StarPositionInSector { TopLeft, TopRight, BottomLeft, BottomRight, CenterTop, CenterLeft, CenterRight, Center };

namespace uvy {

	class StarSystem {

		friend class Universe;

	public:
		StarSystem(const int64_t& l_x, const int64_t& l_y);
		~StarSystem();

		void SetStarPositionInSector(const int64_t& l_x, const int64_t& l_y); //Used by Universe to set the position of the star in the sector

		//Setters//

		//Getters//

		sf::CircleShape&			GetStarShape();
		sf::RectangleShape&			GetStarSelector();
		const StarSize&				GetStarSize()					const;
		std::string					GetStarSizeString()				const;
		const StarColor&			GetStarColor()					const;
		std::string				    GetStarColorString()			const;
		const StarPositionInSector& GetStarPositionInSector()		const;
		std::string					GetStarPositionInSectorString() const;
		const int64_t&				GetID()							const;
		const float					GetStarRadius()					const;
		const bool					HasStar()					const;

	private:
		void GenerateSeed(const int64_t& l_x, const int64_t& l_y);
		bool GenerateStar();
		void DetermineStarSize();
		void DetermineStarColor();
		void DetermineStarPosition();

	private:
		sf::CircleShape      m_starShape;
		sf::RectangleShape   m_starSelector;		//If mouse is over star, this will be drawn
		StarSize		     m_starSize;
		StarColor		     m_starColor;
		StarPositionInSector m_starPositionInSector;
		int64_t			 m_id;					 //Unique ID for every star. Retrieved by unique seed.
		float			     m_starRadius;
		bool			     m_hasStar;

	};

}