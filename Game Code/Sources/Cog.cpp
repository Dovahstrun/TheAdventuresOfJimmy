//Project Includes
#include "../Headers/Cog.h"
#include "../../Framework/Headers/AssetManager.h"

Cog::Cog()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Environment/Cog.png"));
}
