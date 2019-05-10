//Library includes
#include <iostream>

// Project Includes
#include "../Headers/Player.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Level.h"
#include "../Headers/Ground.h"
#include "../Headers/Tool Wheel.h"
#include "../Headers/Hammer.h"

//Constants
#define SPEED 140.0f
#define GRAVITY 180.0f
#define JUMP 200.0f

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
	, hasCheckBeenRun(false)
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

void Player::Input(sf::Event _gameEvent)
{
	
}

void Player::Update(sf::Time _frameTime)
{	
	//Call the update function manually on the player class. This will actually move the character
	MovingObject::Update(_frameTime);
	
	//First assume no keys are pressed
	m_velocity.x = 0.0f;

	//Use the keyboard function to check which keys are currently held down and to move in that direction
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) //Check if the player is going left
	{
		m_velocity.x = -SPEED;
		//AttemptMove(_frameTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) //Check if the player is going right
	{
		m_velocity.x = SPEED;
		//AttemptMove(_frameTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_touchingGround == true)
	{
		m_velocity.y = -JUMP;
		m_touchingGround = false;
	}

	//Apply gravity to our velocity
	if (m_touchingGround == false)
	{
		float velocityChange = GRAVITY * _frameTime.asSeconds();
		m_velocity.y += velocityChange;
		//AttemptMove(_frameTime);
	}
	if (m_hasCollideBeenRun == false)
	{
		m_touchingWall = false;
		m_touchingGround = false;
	}
	m_hasCollideBeenRun = false;

	if (hasCheckBeenRun)
	{
		std::cerr << "bob";
	}
}

void Player::Collide(GameObject &_collider)
{
	//Only do something if thing touched was player

	//Record whether we used to be touching the ground
	bool wereTouchingGround = m_touchingGround;
	m_touchingGround = false;

	bool wereTouchingWall = m_touchingWall;
	m_touchingWall = false;

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
		groundBottomRect.top += groundCollider->GetBounds().width - 10;
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
			std::cerr << wereTouchingWall;
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
			std::cerr << wereTouchingWall;
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
			m_touchingGround = true;

			//Check if we are falling downward
			if (wereTouchingGround == false && m_velocity.y < 0)
			{
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, groundCollider->GetPosition().y + groundCollider->GetBounds().height);
			}
		}

		Ground* groundCollider = dynamic_cast<Ground*>(&_collider);

		m_hasCollideBeenRun = true;
		//Clumsy, results in sticky grounds but good enough for this game
		
	}

	Hammer* hammerCollider = dynamic_cast<Hammer*>(&_collider);
	if (hammerCollider != nullptr)
	{
		m_hammerCollected = true;
		m_level->deleteObjectAt(hammerCollider);
	}

}

//void Player::AttemptMove(sf::Time _frameTime)
//{
//	sf::RectangleShape testRect;
//	testRect.setSize(m_sprite.getScale());
//	testRect.setPosition(m_sprite.getPosition());
//	testRect.move(m_velocity.x * _frameTime.asSeconds(), 0);
//	if (m_level->Collision(testRect))
//	{
//		if (m_velocity.x != 0)
//		{
//			m_velocity.x = 0;
//		}
//	}
//	testRect.setPosition(m_sprite.getPosition());
//	testRect.move(0, m_velocity.y * _frameTime.asSeconds());
//	if (m_level->Collision(testRect))
//	{
//		if (m_velocity.y != 0)
//		{
//			m_velocity.y = 0;
//		}
//	}
//}

bool Player::CheckTool(sf::String _tool)
{
	if (_tool == "Hammer")
	{
		if (m_hammerCollected)
		{
			return true;
		}
	}
	if (_tool == "Spanner")
	{
		if (m_shearsCollected)
		{
			return true;
		}
	}
	if (_tool == "Shears")
	{
		if (m_spannerCollected)
		{
			return true;
		}
	}
	return false;
}

//void Player::AttemptMove(sf::Time _frameTime)
//{
//	sf::RectangleShape testRect;
//	testRect.setSize(m_sprite.getScale());
//	testRect.setPosition(m_sprite.getPosition());
//	testRect.move(m_velocity.x * _frameTime.asSeconds(), 0);
//	if (m_level->Collision(testRect))
//	{
//		if (m_velocity.x != 0)
//		{
//			m_velocity.x = 0;
//		}
//	}
//	testRect.setPosition(m_sprite.getPosition());
//	testRect.move(0, m_velocity.y * _frameTime.asSeconds());
//	if (m_level->Collision(testRect))
//	{
//		if (m_velocity.y != 0)
//		{
//			m_velocity.y = 0;
//		}
//	}
//}



//bool Player::AttemptMove(sf::Vector2i _direction)
//{
//	//Attempt to move in the given direction
//
//	//Get your current position
//	//Calculate target position
//	sf::Vector2i targetPos = m_gridPosition + _direction;
//
//	//TODO: Check if the space is empty
//	//Get list of objects in target position (targetpos)
//	std::vector<GridObject*> targetCellContents = m_level->getObjectAt(targetPos);
//	//Check if any of those objects block movement
//	bool blocked = false;
//	GridObject* blocker = nullptr;
//	for (int i = 0; i < targetCellContents.size(); ++i)
//	{
//		if (targetCellContents[i]->getBlocksMovement() == true)
//		{
//			blocked = true;
//			blocker = targetCellContents[i];
//		}
//	}
//
//	//If empty move there
//
//	if (!blocked)
//	{
//		m_footstep.play();
//		return m_level->MoveObjectTo(this, targetPos);
//	}
//	else
//	{
//		//We were blocked!
//
//		//Can we interact with the thing blocking us?
//		//Do a dynamic cast to dirt to see if we can dig it
//		Dirt* dugDirt = dynamic_cast<Dirt*>(blocker);
//
//		//If so, attempt to dig (the blocker is dirt, not nullptr)
//		if (dugDirt != nullptr)
//		{
//			//Delete the dirt
//			m_level->deleteObjectAt(dugDirt);
//			
//			//Play dig sound
//			m_dig.play();
//
//			//Move to new spot (where dirt was)
//			return m_level->MoveObjectTo(this, targetPos);
//		}
//
//		//Can we interact with the thing blocking us?
//		//Do a dynamic cast to diamond to see if we can collect it
//		Diamond* diamond = dynamic_cast<Diamond*>(blocker);
//
//		//If so, attempt to collect (the blocker is a diamond, not nullptr)
//		if (diamond != nullptr)
//		{
//			//Delete the diamond
//			m_level->deleteObjectAt(diamond);
//
//			//Play gem sound
//			m_gem.play();
//
//			//Check if the level is complete
//			m_level->canExitOpen();
//
//			//Move to new spot (where diamond was)
//			return m_level->MoveObjectTo(this, targetPos);
//		}
//
//		//Do a dynamic cast to boulder to see if we can move it
//		Boulder* boulder = dynamic_cast<Boulder*>(blocker);
//
//		//If so, attempt to push (the blocker is a boulder, not nullptr)
//		if (boulder != nullptr)
//		{
//			//Move the boulder
//			bool boulderMove = boulder->AttemptPush(_direction);
//
//			//If the boulder moved, move the player
//			if (boulderMove)
//			{
//				//Play push sound
//				m_push.play();
//
//				//Move player
//				return m_level->MoveObjectTo(this, targetPos);
//			}
//		}
//
//		//Do a dynamic cast to theExit to see if we can move it
//		Exit* theExit = dynamic_cast<Exit*>(blocker);
//
//		//If so, attempt to push (the blocker is an exit, not nullptr)
//		if (theExit != nullptr)
//		{
//			m_level->checkComplete();
//
//			//Move player
//			return m_level->MoveObjectTo(this, targetPos);
//		}
//
//	}
//
//	//If movement is blocked, do nothing, return false
//	m_bump.play();
//	return false;
//}
