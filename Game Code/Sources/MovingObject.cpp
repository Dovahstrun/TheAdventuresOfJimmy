
// Project Includes
#include "../Headers/MovingObject.h"

MovingObject::MovingObject()
	: GridObject() // always intiialise base class
	, m_velocity	(0.0f, 0.0f)
	, m_oldPosition (0.0f, 0.0f)
	, m_touchingGround(false)
	, m_touchingWall(false)
	, m_touchingCeiling(false)
{

}

void MovingObject::Update(sf::Time _frameTime)
{
	// Get the current position
	sf::Vector2f currentPosition = m_sprite.getPosition();

	//Record our current position as old position
	m_oldPosition = currentPosition;

	// Calculate the amount moved
	sf::Vector2f amountMoved = m_velocity * _frameTime.asSeconds();

	// Calculate new position
	sf::Vector2f newPosition = currentPosition + amountMoved;

	// Set our sprite to this new position
	m_sprite.setPosition(newPosition);

	
}

void MovingObject::newCollide()
{

}
