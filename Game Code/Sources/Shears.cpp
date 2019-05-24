//Project Includes
#include "../Headers/Shears.h"
#include "../../Framework/Headers/AssetManager.h"

Shears::Shears()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Environment/Shears.png"));
}
