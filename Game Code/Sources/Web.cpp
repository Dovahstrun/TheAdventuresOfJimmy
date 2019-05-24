//Project Includes
#include "../Headers/Web.h"
#include "../../Framework/Headers/AssetManager.h"

Web::Web()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Environment/Web.png"));
}
