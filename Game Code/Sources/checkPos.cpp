#include "..\Headers\checkPos.h"
#include "../../Framework/Headers/AssetManager.h"

checkPos::checkPos()
	: TextObject()
{
	m_text.setFont(AssetManager::GetFont("fonts/mainFont.ttf"));
	m_text.setString("Pos: 0");
	m_text.setPosition(500, 500);
}

void checkPos::Update(sf::Time _frameTime)
{

}
