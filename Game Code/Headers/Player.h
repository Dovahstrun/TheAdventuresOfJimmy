#pragma once

// Project Includes
#include "MovingObject.h"

class Player : public MovingObject
{

public:

	Player();

	//Overriding Functions
	void Input(sf::Event _gameEvent);
	void Update(sf::Time _frameTime);
	void Collide(GameObject& _collider);

private:

	/*bool AttemptMove(sf::Vector2i _direction);*/
	//void AttemptMove(sf::Time _frameTime);

private:

	sf::Vector2i m_pendingMove;
	sf::Sound m_footstep;
	sf::Sound m_dig;
	sf::Sound m_bump;
	sf::Sound m_gem;
	sf::Sound m_push;

};