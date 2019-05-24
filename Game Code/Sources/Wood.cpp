//Project Includes
#include "../Headers/Wood.h"
#include "../../Framework/Headers/AssetManager.h"

Wood::Wood()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Environment/Wooden Box.png"));
}
