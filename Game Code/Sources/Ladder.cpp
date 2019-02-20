//Project Includes
#include "../Headers/Ladder.h"
#include "../../Framework/Headers/AssetManager.h"

Ladder::Ladder()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/environment/Ladder.png"));
}
