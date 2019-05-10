#pragma once

// Project Includes
#include "GridObject.h"

class MovingObject : public GridObject
{

public:

	MovingObject();

	// Overriding Methods
	void Update(sf::Time _frameTime);

protected:

	sf::Vector2f m_velocity; // pixels per second
	sf::Vector2f m_oldPosition; //The previous position that will allow it to jump back when it collides with something
	bool m_touchingGround;
	bool m_touchingWall;
	bool m_touchingCeiling;
};