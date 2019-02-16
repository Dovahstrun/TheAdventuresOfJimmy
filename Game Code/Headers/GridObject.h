#pragma once

//Project includes
#include "../../Framework/Headers/SpriteObject.h"

//Forward Declaration (this only works with pointers and references)
class Level;



class GridObject : public SpriteObject
{
public:

	GridObject();

	void setGridPosition(sf::Vector2i _newPosition);
	void setGridPosition(int _x, int _y);

	void setLevel(Level* _newLevel);

	sf::Vector2i getGridPosition();
	bool getBlocksMovement();

protected:

	sf::Vector2i m_gridPosition;
	Level* m_level;
	bool m_blocksMovement;

};