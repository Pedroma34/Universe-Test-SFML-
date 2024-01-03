#include "pch.h"
#include "Camera.h"
#include "Universe.h"

uvy::Camera::Camera() :
	m_originalViewSize(0, 0),
	m_zoomInLimit(0, 0),
	m_zoomOutLimit(0, 0),
	m_velocity(0, 0),
	m_maxVelocity(0, 0),
	m_thresholdRight(100, 100),
	m_thresholdLeft(-100, -100),
	m_acceleration(0),
	m_friction(0),
	m_seeCameraShape(false),
	m_outOfBoundsOnWidth(false),
	m_outOfBoundsOnHeight(false)
{}

uvy::Camera::~Camera() {}

void uvy::Camera::Update(sf::View& l_view) {
	HandleCamera(l_view);
	CheckThreshold(l_view);
	UpdateCameraShape(l_view);
}

void uvy::Camera::Draw() {

	//Camera shape//
	if (SharedData::GetDebug() && m_seeCameraShape)
		SharedData::GetWindow().Draw(m_cameraShape);

	//Queue//
	if (m_drawQueue.empty())
		return;
	for (auto& drawable : m_drawQueue)
		SharedData::GetWindow().Draw(*drawable);
	m_drawQueue.clear();
}


void uvy::Camera::CheckThreshold(sf::View& l_view) {
	//Testing function//

	/*Temporary testing*/

	//Threshold in sector coordinates. The view cordinates will be this multiplied by the sector size.
	const sf::Vector2<int64_t>& rightThreshold = GetThresholdRight();
	const sf::Vector2<int64_t>& leftThreshold  = GetThresholdLeft();

	//Some other variables
	const sf::Vector2<int64_t>& sectorSize = SharedData::GetUniverse().GetSectorSize();

	//View variables from all corners
	sf::Vector2f viewCenter			   = l_view.getCenter();
	sf::Vector2f viewSize			   = l_view.getSize();
	sf::Vector2f viewBottomRight	   = viewCenter + (viewSize / 2.f);
	sf::Vector2f viewBottomLeft		   = sf::Vector2f(viewCenter.x - (viewSize.x / 2.f), viewBottomRight.y);
	sf::Vector2f viewTopRight		   = sf::Vector2f(viewBottomRight.x, viewCenter.y - (viewSize.y / 2.f));
	sf::Vector2f viewTopLeft		   = viewCenter - (viewSize / 2.f);
	sf::Vector2i viewCenterSector	   = sf::Vector2i(floor(viewCenter.x / sectorSize.x), floor(viewCenter.y / sectorSize.y));
	sf::Vector2i viewBottomRightSector = sf::Vector2i(floor(viewBottomRight.x / sectorSize.x), floor(viewBottomRight.y / sectorSize.y));
	sf::Vector2i viewBottomLeftSector  = sf::Vector2i(floor(viewBottomLeft.x / sectorSize.x), floor(viewBottomLeft.y / sectorSize.y));
	sf::Vector2i viewTopRightSector	   = sf::Vector2i(floor(viewTopRight.x / sectorSize.x), floor(viewTopRight.y / sectorSize.y));
	sf::Vector2i viewTopLeftSector	   = sf::Vector2i(floor(viewTopLeft.x / sectorSize.x), floor(viewTopLeft.y / sectorSize.y));

	//Logic//
	
	//*Temporary limiting the player within bounds*//
	
	const float pullBackForce = m_maxVelocity.x * 500;

	//If camera reaches right threshold
	if (viewBottomRightSector.x >= rightThreshold.x) {
		m_outOfBoundsOnWidth = true;
		m_velocity.x = -pullBackForce;
	}
	//If camera reaches left threshold
	else if (viewBottomLeftSector.x <= leftThreshold.x) {
		m_outOfBoundsOnWidth = true;
		m_velocity.x = pullBackForce;
	}
	else
		m_outOfBoundsOnWidth = false;

	//If camera reaches bottom threshold
	if (viewBottomRightSector.y >= rightThreshold.y) {
		m_outOfBoundsOnHeight = true;
		m_velocity.y = -pullBackForce;
	}
	//If camera reaches top threshold
	else if (viewTopRightSector.y <= leftThreshold.y) {
		m_outOfBoundsOnHeight = true;
		m_velocity.y = pullBackForce;
	}
	else
		m_outOfBoundsOnHeight = false;


}

void uvy::Camera::SetOriginalViewSize(const sf::Vector2f& l_size) {
	m_originalViewSize = l_size;
}

void uvy::Camera::SetZoomInLimit(const sf::Vector2f& l_limit) {
	m_zoomInLimit = l_limit;
}

void uvy::Camera::SetZoomOutLimit(const sf::Vector2f& l_limit) {
	m_zoomOutLimit = l_limit;
}

void uvy::Camera::SetVelocity(const sf::Vector2f& l_velocity) {
	m_velocity = l_velocity;
}

void uvy::Camera::SetMaxVelocity(const sf::Vector2f& l_maxVelocity) {
	m_maxVelocity = l_maxVelocity;
}

void uvy::Camera::SetAcceleration(const float& l_acceleration) {
	m_acceleration = l_acceleration;
}

void uvy::Camera::SetFriction(const float& l_friction) {
	m_friction = l_friction;
}

void uvy::Camera::SetSeeCameraShape(const bool& l_seeCameraShape) {
	m_seeCameraShape = l_seeCameraShape;
}

void uvy::Camera::SetThresholdRight(const sf::Vector2<int64_t>& l_thresholdRight) {
	m_thresholdRight = l_thresholdRight;
}

void uvy::Camera::SetThresholdLeft(const sf::Vector2<int64_t>& l_thresholdLeft) {
	m_thresholdLeft = l_thresholdLeft;
}

const sf::Vector2f& uvy::Camera::GetOriginalViewSize() const {
	return m_originalViewSize;
}

const sf::Vector2f& uvy::Camera::GetZoomInLimit() const {
	return m_zoomInLimit;
}

const sf::Vector2f& uvy::Camera::GetZoomOutLimit() const {
	return m_zoomOutLimit;
}

const sf::Vector2f& uvy::Camera::GetVelocity() const {
	return m_velocity;
}

const sf::Vector2f& uvy::Camera::GetMaxVelocity() const {
	return m_maxVelocity;
}

const sf::Vector2<int64_t> uvy::Camera::GetThresholdRight() const {
	return m_thresholdRight;
}

const sf::Vector2<int64_t> uvy::Camera::GetThresholdLeft() const {
	return m_thresholdLeft;
}

const float& uvy::Camera::GetAcceleration() const {
	return m_acceleration;
}

const float& uvy::Camera::GetFriction() const {
	return m_friction;
}

const bool& uvy::Camera::GetSeeCameraShape() const {
	return m_seeCameraShape;
}

const bool& uvy::Camera::GetOutOfBoundsOnWidth() const {
	return m_outOfBoundsOnWidth;
}

const bool& uvy::Camera::GetOutOfBoundsOnHeight() const {
	return m_outOfBoundsOnHeight;
}

void uvy::Camera::HandleCamera(sf::View& l_view) {
	MoveCamera();
	ApplyFriction();
	ClampVelocity();
	ApplyMovement(l_view);
}

void uvy::Camera::UpdateCameraShape(sf::View& l_view) {

	/////////////////////////////////////////////////////////////////////////////
	//Camera shape needs to be updated every time the view moves or changes size. 
	//For now, let's do every game tick
	/////////////////////////////////////////////////////////////////////////////
	
	m_cameraShape.setSize(sf::Vector2f(l_view.getSize().x, l_view.getSize().y));
	m_cameraShape.setOrigin(m_cameraShape.getGlobalBounds().width / 2, m_cameraShape.getGlobalBounds().height / 2);
	m_cameraShape.setPosition(l_view.getCenter());
	m_cameraShape.setFillColor(sf::Color(255, 0, 0, 110));

}

void uvy::Camera::MoveCamera() {

	sf::Time& time = SharedData::GetTime();

	//Moving//
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		SetVelocity(sf::Vector2f(m_velocity.x - m_acceleration * time.asSeconds(), m_velocity.y));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		SetVelocity(sf::Vector2f(m_velocity.x + m_acceleration * time.asSeconds(), m_velocity.y));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		SetVelocity(sf::Vector2f(m_velocity.x, m_velocity.y - m_acceleration * time.asSeconds()));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		SetVelocity(sf::Vector2f(m_velocity.x, m_velocity.y + m_acceleration * time.asSeconds()));

}

void uvy::Camera::ApplyFriction() {

	//If camera is out of bounds, don't apply friction
	if(m_outOfBoundsOnWidth || m_outOfBoundsOnHeight)
		return;

	sf::Time& time = SharedData::GetTime();

	//Applaying Friction//
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		float frictionEffect = m_friction * time.asSeconds();

		// Apply friction only if the absolute velocity is greater than the friction effect
		if (std::abs(m_velocity.x) > frictionEffect) {
			if (m_velocity.x > 0)
				SetVelocity(sf::Vector2f(m_velocity.x - frictionEffect, m_velocity.y));
			else
				SetVelocity(sf::Vector2f(m_velocity.x + frictionEffect, m_velocity.y));
		}
		else
			// Set velocity to zero if friction effect is greater than velocity
			SetVelocity(sf::Vector2f(0, m_velocity.y));
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		float frictionEffect = m_friction * time.asSeconds();

		// Apply friction only if the absolute velocity is greater than the friction effect
		if (std::abs(m_velocity.y) > frictionEffect) {
			if (m_velocity.y > 0)
				SetVelocity(sf::Vector2f(m_velocity.x, m_velocity.y - frictionEffect));
			else
				SetVelocity(sf::Vector2f(m_velocity.x, m_velocity.y + frictionEffect));
		}
		else
			// Set velocity to zero if friction effect is greater than velocity
			SetVelocity(sf::Vector2f(m_velocity.x, 0));
	}

}

void uvy::Camera::ClampVelocity() {

	//If camera is out of bounds, don't clamp velocity
	if(m_outOfBoundsOnWidth || m_outOfBoundsOnHeight)
		return;

	if (m_velocity.x > GetMaxVelocity().x)
		SetVelocity(sf::Vector2f(GetMaxVelocity().x, m_velocity.y));
	else if (m_velocity.x < -GetMaxVelocity().x)
		SetVelocity(sf::Vector2f(-GetMaxVelocity().x, m_velocity.y));
	if (m_velocity.y > GetMaxVelocity().y)
		SetVelocity(sf::Vector2f(m_velocity.x, GetMaxVelocity().y));
	else if (m_velocity.y < -GetMaxVelocity().y)
		SetVelocity(sf::Vector2f(m_velocity.x, -GetMaxVelocity().y));

}

void uvy::Camera::ApplyMovement(sf::View& l_view) {

	sf::Time& time = SharedData::GetTime();
	l_view.move(m_velocity * time.asSeconds());

}
