//Project Includes
#include "../Headers/Exit.h"
#include "../../Framework/Headers/AssetManager.h"

Exit::Exit()
	: GridObject()
	, exitType(TOP)
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Environment/door.png"));
}

void Exit::setExitType(int _type)
{
	//Recieve the exit type from the level and set it
	switch (_type)
	{
		case 1:
			exitType = TOP;
			break;
		case 2:
			exitType = TOPRIGHT;
			break;
		case 3:
			exitType = LEFT;
			break;
		case 4:
			exitType = RIGHT;
			break;
		case 5:
			exitType = BOTTOMLEFT;
			break;
		case 6:
			exitType = BOTTOMRIGHT;
			break;
		case 7:
			exitType = BOTTOM;
			break;

	}
}

Exit::exittypes Exit::getExitType()
{
	return exitType;
}
