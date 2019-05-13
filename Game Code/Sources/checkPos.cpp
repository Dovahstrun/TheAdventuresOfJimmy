#include "..\Headers\checkPos.h"
#include "../../Framework/Headers/AssetManager.h"

checkPos::checkPos()
	: TextObject()
{
	m_text.setFont(AssetManager::GetFont("fonts/mainFont.ttf"));
	m_text.setString("Pos: 0, 0");
	m_text.setPosition(100, 100);
}

void checkPos::Update(sf::Time _frameTime)
{
}

void checkPos::setText(sf::Vector2i _playerPos)
{
	m_text.setString("Pos: " + std::to_string(_playerPos.x) + ", " + std::to_string(_playerPos.y));
}
