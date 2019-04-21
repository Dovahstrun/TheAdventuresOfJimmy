//Project Includes
#include "../Headers/Ground.h"
#include "../../Framework/Headers/AssetManager.h"

Ground::Ground()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/wall.png"));
	m_blocksMovement = true;
}
