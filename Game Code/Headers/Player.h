#pragma once

// Project Includes
#include "GridObject.h"

class Player : public GridObject
{

public:

	Player();

	//Overriding Functions
	void Input(sf::Event _gameEvent);
	void Update(sf::Time _frameTime);

private:

	/*bool AttemptMove(sf::Vector2i _direction);*/

private:

	sf::Vector2i m_pendingMove;
	sf::Sound m_footstep;
	sf::Sound m_dig;
	sf::Sound m_bump;
	sf::Sound m_gem;
	sf::Sound m_push;

};