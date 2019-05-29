//Project Includes
#include "../Headers/Tool Wheel.h"
#include "../../Framework/Headers/AssetManager.h"


ToolWheel::ToolWheel()
	: SpriteObject()
	, m_player(nullptr)
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Tool wheel/Tool Wheel NULL & NULL & NULL.png"));
}

void ToolWheel::Update(sf::Time _frameTime)
{
	//Set the position so the player is at the center of the wheel
	m_sprite.setPosition(m_player->GetPosition().x - m_sprite.getGlobalBounds().width / 2 + m_player->GetBounds().width / 2, m_player->GetPosition().y - m_sprite.getGlobalBounds().height / 2 + m_player->GetBounds().height / 2);

	//Activate the tool wheel when E is pressed, and allow the player to choose their tool.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		m_active = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			m_player->setCurrentTool(Player::SPANNER);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			m_player->setCurrentTool(Player::HAMMER);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			m_player->setCurrentTool(Player::SHEARS);
		}

	}
	else
	{
		m_active = false;
	}
	
	//Change the texture based on what tools have been collected
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Tool wheel/Tool Wheel " + m_player->CheckTool("Spanner") + " & " + m_player->CheckTool("Shears") + " & " + m_player->CheckTool("Hammer") + ".png"));
	
}

void ToolWheel::setPlayer(Player* _player)
{
	m_player = _player;
}
