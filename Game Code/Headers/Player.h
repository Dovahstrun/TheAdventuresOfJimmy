#pragma once

// Project Includes
#include "MovingObject.h"

class ToolWheel;

class Player : public MovingObject
{

public: //ENUMS

	enum tools
	{
		NONE,
		HAMMER,
		SPANNER,
		SHEARS
	};

public:

	Player();

	//Overriding Functions
	void Update(sf::Time _frameTime);
	void Collide(GameObject& _collider);

	sf::String CheckTool(sf::String _tool);
	void setToolWheel(ToolWheel* _toolWheel);
	void setCurrentTool(tools _newTool);
	sf::String getCurrentTool();
	int getScrewsCollected();

private:

	void UseTool();

private:

	sf::Vector2i m_pendingMove;


	sf::Sound m_footstep;
	sf::Sound m_jump;
	sf::Sound m_boxBreak;
	sf::Sound m_spannerTwist;
	sf::Sound m_snip;


	bool m_spannerCollected;
	bool m_shearsCollected;
	bool m_hammerCollected;
	std::vector<std::pair<tools, bool>> m_collectedTools;


	bool m_hasCollideBeenRun;
	tools m_currentTool;
	ToolWheel* m_toolWheel;
	bool m_touchingLadder;
	int m_collectedScrews;

};