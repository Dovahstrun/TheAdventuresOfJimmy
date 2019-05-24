//Library includes
#include <iostream>

// Project Includes
#include "../Headers/Player.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Level.h"
#include "../Headers/Ground.h"
#include "../Headers/Wood.h"
#include "../Headers/Tool Wheel.h"
#include "../Headers/Hammer.h"
#include "../Headers/Spanner.h"
#include "../Headers/Shears.h"
#include "../Headers/Web.h"
#include "../Headers/Exit.h"
#include "../Headers/Ladder.h"
#include "../Headers/Cog.h"
#include "../Headers/Screw.h"

//Constants
#define SPEED 220.0f
#define GRAVITY 1250.0f
#define JUMP 620.0f

Player::Player()
	: MovingObject()
	, m_footstep()
	, m_dig()
	, m_bump()
	, m_gem()
	, m_push()
	, m_spannerCollected(false)
	, m_shearsCollected(false)
	, m_hammerCollected(false)
	, m_hasCollideBeenRun(false)
	, m_currentTool(NONE)
	, m_toolWheel(nullptr)
	, m_touchingLadder(false)
	, m_collectedScrews(0)
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/player/playerSmall.png"));
	m_footstep.setBuffer(AssetManager::GetSoundBuffer("resources/audio/floor_step.wav"));
	m_dig.setBuffer(AssetManager::GetSoundBuffer("resources/audio/footstep1.ogg"));
	m_bump.setBuffer(AssetManager::GetSoundBuffer("resources/audio/bump.wav"));
	m_gem.setBuffer(AssetManager::GetSoundBuffer("resources/audio/ding.wav"));
	m_gem.setVolume(15);
	m_push.setBuffer(AssetManager::GetSoundBuffer("resources/audio/push.wav"));
	m_push.setVolume(40);
	m_blocksMovement = false;
}

void Player::Update(sf::Time _frameTime)
{	
	//Call the update function manually on the player class. This will actually move the character
	MovingObject::Update(_frameTime);
	
	//First assume no keys are pressed
	m_velocity.x = 0.0f;
	if (m_touchingLadder)
	{
		m_velocity.y = 0.0f;
	}

	//Use the keyboard function to check which keys are currently held down and to move in that direction
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) //Check if the player is going left
	{
		m_velocity.x = -SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) //Check if the player is going right
	{
		m_velocity.x = SPEED;
	}
	if (!m_touchingLadder)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_touchingGround)
		{
			m_velocity.y = -JUMP;
			m_touchingGround = false;
		}
	}
	else if (m_touchingLadder)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) //Check if the player is going left
		{
			m_velocity.y = -SPEED;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) //Check if the player is going right
		{
			m_velocity.y = SPEED;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !m_toolWheel->isActive())
	{
		UseTool();
	}


	//Apply gravity to our velocity
	if (m_touchingGround == false && !m_touchingLadder)
	{
		float velocityChange = GRAVITY * _frameTime.asSeconds();
		m_velocity.y += velocityChange;
	}

	//A checker to determine if collide has been run, and if it hasn't then it resets the touching variables.
	if (m_hasCollideBeenRun == false)
	{
		m_touchingWall = false;
		m_touchingGround = false;
		m_touchingCeiling = false;
		m_touchingLadder = false;
	}
	m_hasCollideBeenRun = false;
}

void Player::Collide(GameObject &_collider)
{
	//Only do something if thing touched was player

	//Record whether we used to be touching the ground
	bool wereTouchingGround = m_touchingGround;
	m_touchingGround = false;

	bool wereTouchingWall = m_touchingWall;
	m_touchingWall = false;

	bool wereTouchingCeiling = m_touchingCeiling;
	m_touchingCeiling = false;

	//Assume our collision will fail (that we're not touching the ground)
	//Get the collider for the player's feet
	sf::FloatRect feetCollider = m_sprite.getGlobalBounds();
	//Set the top of our feet to be 10 pixels from the bottom of the player collider
	feetCollider.top += m_sprite.getGlobalBounds().height - 10;
	//Set our feet collider height to be 10 pixels
	feetCollider.height = 10;
	//Make the width smaller so it doesn't interfere with the left and right colliders
	feetCollider.width -= 20;
	feetCollider.left += 10;

	//Get the collider for the player's left side
	sf::FloatRect leftCollider = m_sprite.getGlobalBounds();
	//Set our left collider width to be 10 pixels
	leftCollider.width = 10;
	//Make the height slightly smaller so no funny business happens
	leftCollider.height -= 4;
	leftCollider.top += 2;

	//Get the collider for the player's right
	sf::FloatRect rightCollider = m_sprite.getGlobalBounds();
	//Set the left of our right side to be 10 pixels from the right of the player collider
	rightCollider.left += m_sprite.getGlobalBounds().width - 10;
	//Set our right collider height to be 10 pixels
	rightCollider.width = 10;
	//Make the height slightly smaller so no funny business happens
	rightCollider.height -= 4;
	rightCollider.top += 2;

	//Get the collider for the player's head
	sf::FloatRect headCollider = m_sprite.getGlobalBounds();
	//Set our head collider height to be 10 pixels
	headCollider.height = 10;
	//Make the width smaller so it doesn't interfere with the left and right colliders
	headCollider.width -= 20;
	headCollider.left += 10;

	//Dynamic cast the gameObject reference into a ground pointer
	//If it succeeds, it was a ground
	Ground* groundCollider = dynamic_cast<Ground*>(&_collider);

	//If it was a ground we need to move ourselves outside the grounds bounds, AKA back where we were
	if (groundCollider != nullptr)
	{
		//the player did hit a ground
		//Go back to the position that the player was in before

		//Create platform top collider
		sf::FloatRect groundTopRect = groundCollider->GetBounds();
		groundTopRect.height = 10;

		//Create the platform left collider
		sf::FloatRect groundLeftRect = groundCollider->GetBounds();
		groundLeftRect.width = 10;

		//Create the platform right collider
		sf::FloatRect groundRightRect = groundCollider->GetBounds();
		groundRightRect.left += groundCollider->GetBounds().width - 10;
		groundRightRect.width = 10;

		//Create the platform bottom collider
		sf::FloatRect groundBottomRect = groundCollider->GetBounds();
		groundBottomRect.top += groundCollider->GetBounds().height - 10;
		groundBottomRect.height = 10;

		//Are the feet touching the top of the platform
		if (feetCollider.intersects(groundTopRect))
		{
			m_touchingGround = true;

			//Check if we are falling downward
			if (wereTouchingGround == false && m_velocity.y > 0)
			{
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, groundCollider->GetPosition().y - m_sprite.getGlobalBounds().height);
			}
		}

		//Is the player's left side touching the right of a wall
		if (leftCollider.intersects(groundRightRect))
		{
			m_touchingWall = true;
			//Check if we are moving left
			if (wereTouchingWall == false && m_velocity.x < 0)
			{
				m_velocity.x = 0;
  				m_sprite.setPosition(groundCollider->GetPosition().x + groundCollider->GetBounds().width, m_sprite.getPosition().y);
			}
		}

		//Is the player's right side touching the left of a wall
		if (rightCollider.intersects(groundLeftRect))
		{
			m_touchingWall = true;
			//Check if we are moving right
			if (wereTouchingWall == false && m_velocity.x > 0)
			{
				m_velocity.x = 0;
				m_sprite.setPosition(groundCollider->GetPosition().x - m_sprite.getGlobalBounds().width, m_sprite.getPosition().y);
				
			}
		}
		//Is the head touching the bottom of the platform
		if (headCollider.intersects(groundBottomRect))
		{
			m_touchingCeiling = true;

			//Check if we are falling downward
			if (wereTouchingCeiling == false && m_velocity.y < 0)
			{
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, groundCollider->GetPosition().y + groundCollider->GetBounds().height);
			}
		}

		m_hasCollideBeenRun = true;
		//Clumsy, results in sticky grounds but good enough for this game
		
	}

	//Check if the collider was wood, if so, do same stuff as ground
	Wood* woodCollider = dynamic_cast<Wood*>(&_collider);

	if (woodCollider != nullptr)
	{
		//the player did hit a wood
		//Go back to the position that the player was in before

		//Create platform top collider
		sf::FloatRect woodTopRect = woodCollider->GetBounds();
		woodTopRect.height = 10;

		//Create the platform left collider
		sf::FloatRect woodLeftRect = woodCollider->GetBounds();
		woodLeftRect.width = 10;

		//Create the platform right collider
		sf::FloatRect woodRightRect = woodCollider->GetBounds();
		woodRightRect.left += woodCollider->GetBounds().width - 10;
		woodRightRect.width = 10;

		//Create the platform bottom collider
		sf::FloatRect woodBottomRect = woodCollider->GetBounds();
		woodBottomRect.top += woodCollider->GetBounds().width - 10;
		woodBottomRect.height = 10;

		//Are the feet touching the top of the platform
		if (feetCollider.intersects(woodTopRect))
		{
			m_touchingGround = true;

			//Check if we are falling downward
			if (wereTouchingGround == false && m_velocity.y > 0)
			{
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, woodCollider->GetPosition().y - m_sprite.getGlobalBounds().height);
			}
		}

		//Is the player's left side touching the right of a wall
		if (leftCollider.intersects(woodRightRect))
		{
			m_touchingWall = true;
			//Check if we are moving left
			if (wereTouchingWall == false && m_velocity.x < 0)
			{
				m_velocity.x = 0;
				m_sprite.setPosition(woodCollider->GetPosition().x + woodCollider->GetBounds().width, m_sprite.getPosition().y);
			}
		}

		//Is the player's right side touching the left of a wall
		if (rightCollider.intersects(woodLeftRect))
		{
			m_touchingWall = true;
			//Check if we are moving right
			if (wereTouchingWall == false && m_velocity.x > 0)
			{
				m_velocity.x = 0;
				m_sprite.setPosition(woodCollider->GetPosition().x - m_sprite.getGlobalBounds().width, m_sprite.getPosition().y);

			}
		}
		//Is the head touching the bottom of the platform
		if (headCollider.intersects(woodBottomRect))
		{
			m_touchingCeiling = true;

			//Check if we are falling downward
			if (wereTouchingCeiling == false && m_velocity.y < 0)
			{
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, woodCollider->GetPosition().y + woodCollider->GetBounds().height);
			}
		}

		m_hasCollideBeenRun = true;
		//Clumsy, results in sticky woods but good enough for this game

	}

	//Check if the collider was a cog, if so delete it
	Cog* cogCollider = dynamic_cast<Cog*>(&_collider);

	if (cogCollider != nullptr)

	{
		//the player did hit a cog
		//Go back to the position that the player was in before

		//Create platform top collider
		sf::FloatRect cogTopRect = cogCollider->GetBounds();
		cogTopRect.height = 10;

		//Create the platform left collider
		sf::FloatRect cogLeftRect = cogCollider->GetBounds();
		cogLeftRect.width = 10;

		//Create the platform right collider
		sf::FloatRect cogRightRect = cogCollider->GetBounds();
		cogRightRect.left += cogCollider->GetBounds().width - 10;
		cogRightRect.width = 10;

		//Create the platform bottom collider
		sf::FloatRect cogBottomRect = cogCollider->GetBounds();
		cogBottomRect.top += cogCollider->GetBounds().width - 10;
		cogBottomRect.height = 10;

		//Are the feet touching the top of the platform
		if (feetCollider.intersects(cogTopRect))
		{
			m_touchingGround = true;

			//Check if we are falling downward
			if (wereTouchingGround == false && m_velocity.y > 0)
			{
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, cogCollider->GetPosition().y - m_sprite.getGlobalBounds().height);
			}
		}

		//Is the player's left side touching the right of a wall
		if (leftCollider.intersects(cogRightRect))
		{
			m_touchingWall = true;
			//Check if we are moving left
			if (wereTouchingWall == false && m_velocity.x < 0)
			{
				m_velocity.x = 0;
				m_sprite.setPosition(cogCollider->GetPosition().x + cogCollider->GetBounds().width, m_sprite.getPosition().y);
			}
		}

		//Is the player's right side touching the left of a wall
		if (rightCollider.intersects(cogLeftRect))
		{
			m_touchingWall = true;
			//Check if we are moving right
			if (wereTouchingWall == false && m_velocity.x > 0)
			{
				m_velocity.x = 0;
				m_sprite.setPosition(cogCollider->GetPosition().x - m_sprite.getGlobalBounds().width, m_sprite.getPosition().y);

			}
		}
		//Is the head touching the bottom of the platform
		if (headCollider.intersects(cogBottomRect))
		{
			m_touchingCeiling = true;

			//Check if we are falling downward
			if (wereTouchingCeiling == false && m_velocity.y < 0)
			{
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, cogCollider->GetPosition().y + cogCollider->GetBounds().height);
			}
		}

		m_hasCollideBeenRun = true;
		//Clumsy, results in sticky cogs but good enough for this game

	}

	//Check if the collider was a web, if so, move slowly
	Web* webCollider = dynamic_cast<Web*>(&_collider);

	if (webCollider != nullptr)
	{
		m_velocity.x /= 2;
		m_velocity.y /= 2;
	}

	Hammer* hammerCollider = dynamic_cast<Hammer*>(&_collider);
	if (hammerCollider != nullptr)
	{
		m_hammerCollected = true;
		m_currentTool = HAMMER;
		m_level->deleteObjectAt(hammerCollider);
		return;
	}

	Shears* shearsCollider = dynamic_cast<Shears*>(&_collider);
	if (shearsCollider != nullptr)
	{
		m_shearsCollected = true;
		m_currentTool = SHEARS;
		m_level->deleteObjectAt(shearsCollider);
		return;
	}

	Spanner* spannerCollider = dynamic_cast<Spanner*>(&_collider);
	if (spannerCollider != nullptr)
	{
		m_spannerCollected = true;
		m_currentTool = SPANNER;
		m_level->deleteObjectAt(spannerCollider);
		return;
	}

	Screw* screwCollider = dynamic_cast<Screw*>(&_collider);

	if (screwCollider != nullptr)
	{
		m_level->deleteObjectAt(screwCollider);
		m_collectedScrews += 1;
		return;
	}

	Exit* exitCollider = dynamic_cast<Exit*>(&_collider);
	
	if (exitCollider != nullptr)
	{
		/*std::cerr << "Level: " + std::to_string(m_level->GetCurrentLevel());
		std::cerr << "Exit Type: " + std::to_string(exitCollider->getExitType());*/
		switch (exitCollider->getExitType())
		{
		case Exit::TOP:
			switch (m_level->GetCurrentLevel())
			{
			case Level::TOP:
				m_level->loadNextLevel(Level::CENTER, Exit::TOP);
				break;
			case Level::CENTER:
				m_level->loadNextLevel(Level::TOP, Exit::TOP);
				break;
			}
			break;
		case Exit::TOPRIGHT:
			switch (m_level->GetCurrentLevel())
			{
			case Level::RIGHT:
				m_level->loadNextLevel(Level::CENTER, Exit::TOPRIGHT);
				break;
			case Level::CENTER:
				m_level->loadNextLevel(Level::RIGHT, Exit::TOPRIGHT);
				break;
				std::cerr << std::to_string(m_level->GetCurrentLevel());
			}
			break;
		case Exit::LEFT:
			switch (m_level->GetCurrentLevel())
			{
			case Level::LEFT:
				m_level->loadNextLevel(Level::CENTER, Exit::LEFT);
				break;
			case Level::CENTER:
				m_level->loadNextLevel(Level::LEFT, Exit::LEFT);
				break;
			}
			break;
		case Exit::RIGHT:
			switch (m_level->GetCurrentLevel())
			{
			case Level::RIGHT:
				m_level->loadNextLevel(Level::CENTER, Exit::RIGHT);
				break;
			case Level::CENTER:
				m_level->loadNextLevel(Level::RIGHT, Exit::RIGHT);
				break;
			}
			break;
		case Exit::BOTTOMLEFT:
			switch (m_level->GetCurrentLevel())
			{
			case Level::LEFT:
				m_level->loadNextLevel(Level::CENTER, Exit::BOTTOMLEFT);
				break;
			case Level::CENTER:
				m_level->loadNextLevel(Level::LEFT, Exit::BOTTOMLEFT);
				break;
			}
			break;
		case Exit::BOTTOMRIGHT:
			switch (m_level->GetCurrentLevel())
			{
			case Level::RIGHT:
				m_level->loadNextLevel(Level::CENTER, Exit::BOTTOMRIGHT);
				break;
			case Level::CENTER:
				m_level->loadNextLevel(Level::RIGHT, Exit::BOTTOMRIGHT);
				break;
			}
			break;
		case Exit::BOTTOM:
			switch (m_level->GetCurrentLevel())
			{
			case Level::BOTTOM:
				m_level->loadNextLevel(Level::CENTER, Exit::BOTTOM);
				break;
			case Level::CENTER:
				m_level->loadNextLevel(Level::BOTTOM, Exit::BOTTOM);
				break;
			}
			break;
		}
	}

	Ladder* ladderCollider = dynamic_cast<Ladder*>(&_collider);

	if (ladderCollider != nullptr)
	{
		m_touchingLadder = true;
	}
	else
	{
		m_touchingLadder = false;
	}

}

sf::String Player::CheckTool(sf::String _tool)
{
	if (_tool == "Hammer")
	{
		if (m_hammerCollected)
		{
			return "Hammer";
		}
	}
	if (_tool == "Spanner")
	{
		if (m_spannerCollected)
		{
			return "Spanner";
		}
	}
	if (_tool == "Shears")
	{
		if (m_shearsCollected)
		{
			return "Shears";
		}
	}
	return "NULL";
}

void Player::UseTool()
{
	sf::FloatRect toolCollider = m_sprite.getGlobalBounds();
	toolCollider.top += m_sprite.getGlobalBounds().height / 2 - 5;
	toolCollider.height = 10;

	toolCollider.width += 20;
	toolCollider.left -= 10;

	GameObject& collider = m_level->ToolCollision(toolCollider);
	Wood* woodCollider = dynamic_cast<Wood*>(&collider);
	Cog* cogCollider = dynamic_cast<Cog*>(&collider);
	Web* webCollider = dynamic_cast<Web*>(&collider);

	switch (m_currentTool)
	{
	case HAMMER:
		if (woodCollider != nullptr)
		{
			m_level->deleteObjectAt(woodCollider);
			return;
		}
		break;
	case SPANNER:
		if (cogCollider != nullptr)
		{
			m_level->deleteObjectAt(cogCollider);
			return;
		}
		break;
	case SHEARS:
		if (webCollider != nullptr)
		{
			m_level->deleteObjectAt(webCollider);
			return;
		}
		break;
	}
}

void Player::setToolWheel(ToolWheel * _toolWheel)
{
	m_toolWheel = _toolWheel;
}

void Player::setCurrentTool(tools _newTool)
{
	m_currentTool = _newTool;
}

int Player::getScrewsCollected()
{
	return m_collectedScrews;
}

