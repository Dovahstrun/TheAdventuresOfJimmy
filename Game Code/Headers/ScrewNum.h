#pragma once

//Project Includes
#include "../../Framework/Headers/TextObject.h"

class Player;

class ScrewNum : public TextObject
{

public:

	ScrewNum();

	//Overriding Methods
	virtual void Update(sf::Time _frameTime);

	void setPlayer(Player* _player);


private:

	Player* m_player;

};
