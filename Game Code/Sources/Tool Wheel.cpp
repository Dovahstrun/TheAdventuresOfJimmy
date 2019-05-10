//Project Includes
#include "../Headers/Tool Wheel.h"
#include "../../Framework/Headers/AssetManager.h"


ToolWheel::ToolWheel()
	: SpriteObject()
	, m_player(nullptr)
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Tool wheel/Tool Wheel Empty.png"));
}

void ToolWheel::Update(sf::Time _frameTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		m_active = true;
	}
	else
	{
		m_active = false;
	}

	m_player->CheckTool("Hammer");
}

void ToolWheel::setPlayer(Player* _player)
{
	///make fake player
	Player* player = new Player();
	
	m_player = _player;
	
}
