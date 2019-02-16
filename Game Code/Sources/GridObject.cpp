//Project Includes

#include "../Headers/GridObject.h"
#include "../../Game Code/Headers/Level.h"

GridObject::GridObject()
	: SpriteObject()
	, m_gridPosition()
	, m_level(nullptr)
	, m_blocksMovement(false)
{
}

void GridObject::setGridPosition(sf::Vector2i _newPosition)
{
	m_gridPosition = _newPosition;
	m_sprite.setPosition(m_gridPosition.x * m_level->getCellSize(), m_gridPosition.y * m_level->getCellSize());
}

void GridObject::setGridPosition(int _x, int _y)
{
	setGridPosition(sf::Vector2i(_x, _y));
}

void GridObject::setLevel(Level * _newLevel)
{
	m_level = _newLevel;
}

sf::Vector2i GridObject::getGridPosition()
{
	return m_gridPosition;
}

bool GridObject::getBlocksMovement()
{
	return m_blocksMovement;
}
