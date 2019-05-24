//Project Includes
#include "../Headers/Spanner.h"
#include "../../Framework/Headers/AssetManager.h"

Spanner::Spanner()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Environment/Spanner.png"));
}
