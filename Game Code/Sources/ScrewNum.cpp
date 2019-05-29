#include "../Headers/ScrewNum.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Player.h"

ScrewNum::ScrewNum()
	: TextObject()
	, m_player(nullptr)
{
	m_text.setFont(AssetManager::GetFont("resources/fonts/mainFont.ttf"));
	m_text.setString("x 0");
	m_text.setPosition(200, 90);
}

void ScrewNum::Update(sf::Time _frameTime)
{
	//Check how many screws the player has and set the text based on this number
	m_text.setString("x " + std::to_string(m_player->getScrewsCollected()));
}

void ScrewNum::setPlayer(Player * _player)
{
	m_player = _player;
}



