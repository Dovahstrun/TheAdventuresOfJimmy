#include "..\Headers\ScrewNum.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Player.h"

ScrewNum::ScrewNum()
	: TextObject()
{
	m_text.setFont(AssetManager::GetFont("fonts/mainFont.ttf"));
	m_text.setString("x 0");
	m_text.setPosition(100, 50);
}

void ScrewNum::Update(sf::Time _frameTime)
{
	m_text.setString("x " + std::to_string(m_player->getScrewsCollected()));
}

void ScrewNum::setPlayer(Player * _player)
{
	m_player = _player;
}
