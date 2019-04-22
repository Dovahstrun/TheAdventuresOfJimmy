//Project Includes
#include "../Headers/Tool Wheel.h"
#include "../../Framework/Headers/AssetManager.h"

ToolWheel::ToolWheel()
	: SpriteObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Tool Wheel Empty.png"));
}
