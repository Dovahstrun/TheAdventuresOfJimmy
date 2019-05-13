#pragma once

//Project Includes
#include "../../Framework/Headers/TextObject.h"

class checkPos : public TextObject
{

public:

	checkPos();

	//Overriding Methods
	virtual void Update(sf::Time _frameTime);

	void setText(sf::Vector2i _playerPos);


private:


};
