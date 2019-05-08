#pragma once

//Library Includes
#include <SFML/Graphics.hpp>
#include "GridObject.h"

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

	GameObject* m_player;
	GameObject* m_toolWheel;
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
