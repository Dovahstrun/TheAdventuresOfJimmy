#pragma once

//Library Includes
#include <SFML/Graphics.hpp>
#include "GridObject.h"

class Player;

class ToolWheel;

class checkPos;

class Level
{

public:

	Level();

	virtual void Draw(sf::RenderTarget& _target);
	virtual void Update(sf::Time _frameTime);
	virtual void Collision();
	virtual bool Collision(sf::RectangleShape _testRect);
	void Input(sf::Event _gameEvent);

	void loadLevel(int _levelToLoad);
	void ReloadLevel();
	void loadNextLevel();
	int GetCurrentLevel();
	void deleteObjectAt(GridObject* _toDelete);

	int getCellSize();

private:

	Player* m_player;
	ToolWheel* m_toolWheel;
	checkPos* m_gridPos;
	const float m_cellSize;
	int m_currentLevel;
	bool m_pendingReload;
	std::vector<std::vector<sf::Sprite> > m_background;
	std::vector<std::vector<std::vector<GridObject*> > > m_contents;
	std::vector<std::pair<GameObject*, GameObject*> > m_collisionList;
	enum levelenum
	{
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
		CENTER
	};
	levelenum currentLevel;
};
