//Project Includes
#include "../Headers/Time Machine.h"
#include "../../Framework/Headers/AssetManager.h"

Time_Machine::Time_Machine()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Environment/time.jpeg"));
}