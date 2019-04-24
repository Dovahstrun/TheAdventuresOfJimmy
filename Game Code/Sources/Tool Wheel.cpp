//Project Includes
#include "../Headers/Tool Wheel.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Player.h"

ToolWheel::ToolWheel()
	: SpriteObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Tool wheel/Tool Wheel Empty.png"));
}

void ToolWheel::Update(sf::Time _frameTime)
{
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		m_active = true;
	}
	else
	{
		m_active = false;
	}*/
}
