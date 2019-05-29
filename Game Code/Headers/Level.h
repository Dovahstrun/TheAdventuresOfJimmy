#pragma once

//Library Includes
#include <SFML/Graphics.hpp>
#include "GridObject.h"
#include "../Headers/Exit.h"

class Player;

class ToolWheel;

class ScrewNum;

class Level
{

public: //ENUMS

	enum levelenum
	{
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
		CENTER
	};

public:

	Level();

	virtual void Draw(sf::RenderTarget& _target);
	virtual void Update(sf::Time _frameTime);
	virtual void Collision();
	virtual GameObject& ToolCollision(sf::FloatRect _toolRect);
	void Input(sf::Event _gameEvent);

	void loadLevel(levelenum _levelToLoad);
	void ReloadLevel();
	void loadNextLevel(levelenum _newLevel, Exit::exittypes _oldType);
	levelenum GetCurrentLevel();
	void deleteObjectAt(GridObject* _toDelete);

	int getCellSize();

private:

	Player* m_player;
	ToolWheel* m_toolWheel;
	const float m_cellSize;
	bool m_pendingReload;
	bool m_pendingLoad;
	levelenum m_levelToLoad;
	std::vector<std::vector<sf::Sprite> > m_background;
	std::vector<std::vector<std::vector<GridObject*> > > m_contents;
	std::vector<std::pair<GameObject*, GameObject*> > m_collisionList;
	levelenum m_currentLevel;
	levelenum m_oldLevel;
	Exit::exittypes m_oldExit;
	sf::Sprite m_toolIcon;
	sf::Sprite m_screwIcon;
	ScrewNum* m_screwNum;
	sf::Music m_ambience;
};
