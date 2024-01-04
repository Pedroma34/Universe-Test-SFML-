#pragma once



#include "pch.h"



namespace uvy {



	//////////////////////////////////////////////////////////////////////////
	/// Handles the camera movement and transformations based on the view.
	//////////////////////////////////////////////////////////////////////////
	class Camera {
	public:
		Camera();



		~Camera();



		void Update(sf::View& l_view);



		//////////////////////////////////////////////////////////////////////////
		/// Normally for debug purposes.
		//////////////////////////////////////////////////////////////////////////
		void Draw();



		//Setters//

		void SetOriginalViewSize(const sf::Vector2f& l_size);
		void SetZoomInLimit		(const sf::Vector2f& l_limit);
		void SetZoomOutLimit	(const sf::Vector2f& l_limit);
		void SetVelocity		(const sf::Vector2f& l_velocity);
		void SetMaxVelocity		(const sf::Vector2f& l_maxVelocity);
		void SetAcceleration	(const float& l_acceleration);
		void SetFriction		(const float& l_friction);
		void SetSeeCameraShape	(const bool& l_seeCameraShape);
		void SetThresholdRight	(const sf::Vector2<int64_t>& l_thresholdRight);
		void SetThresholdLeft	(const sf::Vector2<int64_t>& l_thresholdLeft);

		//Getters//

		const sf::Vector2f&		   GetOriginalViewSize()  const;
		const sf::Vector2f&		   GetZoomInLimit()		  const;
		const sf::Vector2f&		   GetZoomOutLimit()	  const;
		const sf::Vector2f&		   GetVelocity()		  const;
		const sf::Vector2f&		   GetMaxVelocity()		  const;
		const sf::Vector2<int64_t> GetThresholdRight()    const; //Right bottom and top of the universe
		const sf::Vector2<int64_t> GetThresholdLeft()     const; //Left bottom and top of the universe
		const float&			   GetAcceleration()	  const;
		const float&			   GetFriction()		  const;
		const bool&				   GetSeeCameraShape()	  const;
		const bool&				   GetOutOfBoundsOnWidth()const;
		const bool&				   GetOutOfBoundsOnHeight()const;



	private:
		//////////////////////////////////////////////////////////////////////////
		/// Checks if camera is out of bounds, sets the out of bounds bools,
		/// and applies velocity to the camera to reach behind the threshold.
		//////////////////////////////////////////////////////////////////////////
		void CheckThreshold(sf::View& l_view);



		//////////////////////////////////////////////////////////////////////////
		/// Applies various transformations to the camera.
		//////////////////////////////////////////////////////////////////////////
		void HandleCamera(sf::View& l_view);



		//////////////////////////////////////////////////////////////////////////
		/// A rectangle shape of the same size as the view that follows the camera.
		//////////////////////////////////////////////////////////////////////////
		void UpdateCameraShape(sf::View& l_view);



		//////////////////////////////////////////////////////////////////////////
		/// Process user input and applies values to movement variables.
		//////////////////////////////////////////////////////////////////////////
		void MoveCamera();



		void ApplyFriction();



		void ClampVelocity();



		//////////////////////////////////////////////////////////////////////////
		/// Applies movement to the camera.
		//////////////////////////////////////////////////////////////////////////
		void ApplyMovement(sf::View& l_view);



	private:



		sf::RectangleShape	 m_cameraShape; //physical representation of the camera, for debug purposes
		sf::Vector2f		 m_originalViewSize;
		sf::Vector2f		 m_zoomInLimit;
		sf::Vector2f		 m_zoomOutLimit;
		sf::Vector2f		 m_velocity;
		sf::Vector2f		 m_maxVelocity;
		sf::Vector2<int64_t> m_thresholdRight; //Right bottom (x) and top (y) of the universe
		sf::Vector2<int64_t> m_thresholdLeft;  //Left bottom (x) and top (y) of the universe
		std::vector<std::shared_ptr<sf::Drawable>> m_drawQueue;
		bool  m_seeCameraShape;
		bool  m_outOfBoundsOnWidth;
		bool  m_outOfBoundsOnHeight;
		float m_acceleration;
		float m_friction;



	};

}