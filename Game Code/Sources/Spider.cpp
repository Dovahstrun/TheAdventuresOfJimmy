//Project Includes
#include "../Headers/Spider.h"
#include "../../Framework/Headers/AssetManager.h"

Spider::Spider()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Environment/spider.png"));
}
