//Project Includes
#include "../Headers/Hammer.h"
#include "../../Framework/Headers/AssetManager.h"

Hammer::Hammer()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Environment/hammer.png"));
}
