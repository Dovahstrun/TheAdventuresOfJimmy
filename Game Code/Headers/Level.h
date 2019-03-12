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
	void Input(sf::Event _gameEvent);

	void loadLevel(int _levelToLoad);
	void ReloadLevel();
	void loadNextLevel();
	int GetCurrentLevel();

	int getCellSize();

private:

	GameObject* m_player;
	const float m_cellSize;
	int m_currentLevel;
	bool m_pendingReload;
	std::vector<std::vector<sf::Sprite> > m_background;
	std::vector<std::vector<std::vector<GridObject*> > > m_contents;
	std::vector<std::pair<GameObject*, GameObject*> > m_collisionList;
};
