#pragma once

//Project Includes
#include "MovingObject.h"

class Wood : public MovingObject
{

public:

	Wood();

	void Update(sf::Time _frameTime);
	void Collide(GameObject& _collider);

private:

	bool m_touchingGround;
	bool m_touchingWall;
	bool m_touchingCeiling;
	bool m_hasCollideBeenRun;

};
