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
	m_sprite.setPosition(m_player->GetPosition().x - m_sprite.getGlobalBounds().width / 2 + m_player->GetBounds().width / 2, m_player->GetPosition().y - m_sprite.getGlobalBounds().height / 2 + m_player->GetBounds().height / 2);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		m_active = true;
	}
	else
	{
		m_active = false;
	}

	if (m_player->CheckTool("Hammer") && !m_player->CheckTool("Shears") && !m_player->CheckTool("Spanner"))
	{
		m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Tool wheel/Tool Wheel Hammer.png"));
	}
}

void ToolWheel::setPlayer(Player* _player)
{
	m_player = _player;
}
