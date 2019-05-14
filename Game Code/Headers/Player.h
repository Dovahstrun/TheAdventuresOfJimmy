#pragma once

// Project Includes
#include "MovingObject.h"

class ToolWheel;

class Player : public MovingObject
{

public:

	Player();

	//Overriding Functions
	void Update(sf::Time _frameTime);
	void Collide(GameObject& _collider);

	bool CheckTool(sf::String _tool);
	void setToolWheel(ToolWheel* _toolWheel);

private:

	void UseTool();

private:

	sf::Vector2i m_pendingMove;
	sf::Sound m_footstep;
	sf::Sound m_dig;
	sf::Sound m_bump;
	sf::Sound m_gem;
	sf::Sound m_push;
	bool m_spannerCollected;
	bool m_shearsCollected;
	bool m_hammerCollected;
	bool m_hasCollideBeenRun;
	enum tools
	{
		NONE,
		HAMMER,
		SPANNER,
		SHEARS
	};
	tools m_currentTool;
	ToolWheel* m_toolWheel;

};