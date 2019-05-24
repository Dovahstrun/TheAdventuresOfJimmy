//Project Includes
#include "../Headers/Wood.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Ground.h"

#define GRAVITY 1250.0f

Wood::Wood()
	: MovingObject()
	, m_touchingGround(false)
	, m_touchingWall(false)
	, m_touchingCeiling(false)
	, m_hasCollideBeenRun(false)
{
	m_sprite.setTexture(AssetManager::GetTexture("resources/graphics/Environment/Wooden Box.png"));
}

void Wood::Update(sf::Time _frameTime)
{
	//Call the update function manually on the player class. This will actually move the character
	MovingObject::Update(_frameTime);

	m_velocity.x = 0.0f;

	//Apply gravity to our velocity
	if (m_touchingGround == false)
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
	}
	m_hasCollideBeenRun = false;
}

void Wood::Collide(GameObject & _collider)
{
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
}
