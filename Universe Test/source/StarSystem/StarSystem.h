#pragma once



#include "pch.h"



namespace uvy {



	enum class StarSize {
		Small,
		Medium,
		Large,
		HyperLarge
	};



	enum class StarColor {
		Red,
		Orange,
		White,
		Blue,
		Purple
	};



	enum class StarPositionInSector {
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		CenterTop,
		CenterLeft,
		CenterRight,
		Center
	};



	//////////////////////////////////////////////////////////////////////////
	/// Star systems will have a chance to generate a star.
	/// If it has a star, it will generate a number of planets as well.
	//////////////////////////////////////////////////////////////////////////
	class StarSystem {
		friend class Universe;
	public:



		//////////////////////////////////////////////////////////////////////////
		/// Will check if star is to be generated or not by using the x and y
		/// coordinates of the sector as an unique and consistent seed.
		/// If star is to be generated, it will generate a random size, color and
		/// position in the sector as well as planets.
		//////////////////////////////////////////////////////////////////////////
		StarSystem(const int64_t& l_x, const int64_t& l_y);
		
		
		
		~StarSystem();



		//Setters//



		void SetModified(const bool l_modified);



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
		const sf::Vector2<int64_t>& GetSectorPosition()					const;
		const float					GetStarRadius()					const;
		const bool					HasStar()					const;
		const bool					GetIsModified() const;

	private:



		//////////////////////////////////////////////////////////////////////////
		/// Generates and sets a seed based on the x and y 
		/// coordinates of the sector.
		//////////////////////////////////////////////////////////////////////////
		void GenerateSeed(const int64_t& l_x, const int64_t& l_y);
		
		
		
		bool GenerateStar();
		
		
		
		void DetermineStarSize();
		
		
		
		void DetermineStarColor();
		
		
		
		void DetermineStarPosition();



		//////////////////////////////////////////////////////////////////////////
		/// Places the star shape inside of the sector based on where it was
		/// determined.
		//////////////////////////////////////////////////////////////////////////
		void SetStarPositionInSector(const int64_t& l_x, const int64_t& l_y); //Used by Universe to set the position of the star in the sector
	private:



		sf::CircleShape      m_starShape;
		sf::RectangleShape   m_starSelector;		 //If mouse is over star, this will be drawn
		StarSize		     m_starSize;
		StarColor		     m_starColor;
		StarPositionInSector m_starPositionInSector;
		int64_t				 m_id;					 //Unique ID for every star. Retrieved by unique seed.
		sf::Vector2<int64_t> m_sectorPosition;  	 //Position of sector where star is located
		float			     m_starRadius;
		bool			     m_hasStar;				 //If star was generated or not
		bool                 m_isModified;
	};

}