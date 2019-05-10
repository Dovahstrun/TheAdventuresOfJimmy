#pragma once

//Project includes
#include "../../Framework/Headers/SpriteObject.h"
#include "../Headers/Player.h"

class ToolWheel : public SpriteObject
{
public:

	ToolWheel();

	//Overriding functions
	void Update(sf::Time _frameTime);

	void setPlayer(Player* _player);


private:

	Player* m_player;

};