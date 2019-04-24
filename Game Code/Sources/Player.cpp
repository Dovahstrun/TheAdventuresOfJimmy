// Project Includes
#include "../Headers/Player.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Level.h"
#include "../Headers/Ground.h"
#include "../Headers/Tool Wheel.h"

//Constants
#define SPEED 100.0f
#define GRAVITY 40.0f
#define JUMP 50.0f

Player::Player()
	: MovingObject()
	, m_footstep()
	, m_dig()
	, m_bump()
	, m_gem()
	, m_push()
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
	////read the input from the keyboard and convert it to a direction to move in

	////was the event a key press
	//if (_gameEvent.type == sf::Event::KeyPressed)
	//{
	//	// Yes it was a key press!

	//	// What key was pressed?
	//	if (_gameEvent.key.code == sf::Keyboard::W || _gameEvent.key.code == sf::Keyboard::Up)
	//	{
	//		// It was W!
	//		// Move up
	//		m_pendingMove = sf::Vector2i(0, -1);
	//		m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/player/playerStandUp.png"));
	//	}
	//	else if (_gameEvent.key.code == sf::Keyboard::A || _gameEvent.key.code == sf::Keyboard::Left)
	//	{
	//		// It was A!
	//		// Move left
	//		m_pendingMove = sf::Vector2i(-1, 0);
	//		m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/player/playerStandLeft.png"));
	//	}
	//	else if (_gameEvent.key.code == sf::Keyboard::S || _gameEvent.key.code == sf::Keyboard::Down)
	//	{
	//		// It was S!
	//		// Move down
	//		m_pendingMove = sf::Vector2i(0, 1);
	//		m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/player/playerStandDown.png"));
	//	}
	//	else if (_gameEvent.key.code == sf::Keyboard::D || _gameEvent.key.code == sf::Keyboard::Right)
	//	{
	//		// It was D!
	//		// Move right
	//		m_pendingMove = sf::Vector2i(1, 0);
	//		m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/player/playerStandRight.png"));
	//	}
	//}
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
	
}

void Player::Collide(GameObject &_collider)
{
	//Only do something if thing touched was player

	//Dynamic cast the gameObject reference into a ground pointer
	//If it succeeds, it was a ground
	Ground* groundCollider = dynamic_cast<Ground*>(&_collider);

	//If it was a ground we need to move ourselves outside the grounds bounds, AKA back where we were
	if (groundCollider != nullptr)
	{
		//the player did hit a ground
		//Go back to the position that the player was in before
		
		if (m_velocity.x != 0)
		{
			m_sprite.setPosition(m_oldPosition.x, m_sprite.getPosition().y);
			m_velocity.x = 0.0f;
		}

		if (m_velocity.y > 0)
		{
			m_touchingGround = true;
			m_velocity.y = 0;
			m_sprite.setPosition(m_sprite.getPosition().x, m_oldPosition.y);
		}

		//Clumsy, results in sticky grounds but good enough for this game
	}
}

bool Player::CheckTool(sf::Text _tool)
{
	return false;
}

void Player::AttemptMove(sf::Time _frameTime)
{
	sf::RectangleShape testRect;
	testRect.setSize(m_sprite.getScale());
	testRect.setPosition(m_sprite.getPosition());
	testRect.move(m_velocity.x * _frameTime.asSeconds(), 0);
	if (m_level->Collision(testRect))
	{
		if (m_velocity.x != 0)
		{
			m_velocity.x = 0;
		}
	}
	testRect.setPosition(m_sprite.getPosition());
	testRect.move(0, m_velocity.y * _frameTime.asSeconds());
	if (m_level->Collision(testRect))
	{
		if (m_velocity.y != 0)
		{
			m_velocity.y = 0;
		}
	}
}


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
