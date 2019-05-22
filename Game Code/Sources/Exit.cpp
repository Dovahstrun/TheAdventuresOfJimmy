//Project Includes
#include "../Headers/Exit.h"
#include "../../Framework/Headers/AssetManager.h"

Exit::Exit()
	: GridObject()
	, exitType(TOP)
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/exit.png"));
}
