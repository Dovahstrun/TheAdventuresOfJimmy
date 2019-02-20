//Project Includes
#include "../Headers/Screw.h"
#include "../../Framework/Headers/AssetManager.h"

Screw::Screw()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Screw.png"));
}
