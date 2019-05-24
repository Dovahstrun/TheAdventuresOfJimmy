//Library Includes
#include <iostream>
#include <fstream>

//Project Includes

#include "../Headers/Level.h"
#include "../../Framework/Headers/AssetManager.h"
#include "../Headers/Ground.h"
#include "../Headers/Player.h"
#include "../Headers/Ladder.h"
#include "../Headers/Cog.h"
#include "../Headers/Screw.h"
#include "../Headers/Wood.h"
#include "../Headers/Web.h"
#include "../Headers/Spider.h"
#include "../Headers/Tool Wheel.h"
#include "../Headers/Hammer.h"
#include "../Headers/Spanner.h"
#include "../Headers/Shears.h"
#include "../Headers/ScrewNum.h"


Level::Level()
	: m_player(nullptr)
	, m_toolWheel(nullptr)
	, m_cellSize(128.0f)
	, m_pendingReload(false)
	, m_pendingLoad(false)
	, m_levelToLoad(CENTER)
	, m_background()
	, m_contents()
	, m_collisionList()
	, m_currentLevel(RIGHT)
	, m_oldLevel(RIGHT)
	, m_oldExit(Exit::TOPRIGHT)
	, m_toolIcon()
	, m_screwIcon(sf::Sprite(AssetManager::GetTexture("resources/graphics/Screw.png")))
	, m_screwNum(nullptr)
{
	m_screwIcon.setPosition(50,50);
	loadLevel(RIGHT);
}

void Level::Draw(sf::RenderTarget & _target)
{
	///Camera

	//Create and update camera
	sf::View camera = _target.getDefaultView();
	camera.setCenter(m_player->GetPosition());

	//Adjust camera as needed
	camera.zoom(0.5f);


	//Draw game world to the camera
	_target.setView(camera);


	///Background
	for (int y = 0; y < m_background.size(); ++y)
	{
		for (int x = 0; x < m_background[y].size(); ++x)
		{
			_target.draw(m_background[y][x]);
		}
	}


	///Game Objects
	
	//Draw game objects
	for (int y = 0; y < m_contents.size(); ++y)//rows
	{
		for (int x = 0; x < m_contents[y].size(); ++x)//cells
		{
			for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
			{
				m_contents[y][x][z]->Draw(_target);
			}
		}
	}

	if (m_toolWheel->isActive())
	{
		m_toolWheel->Draw(_target);
	}

	///UI
	// Draw UI to the window
	_target.setView(_target.getDefaultView());


	//Draw UI objects

	_target.draw(m_toolIcon);
	_target.draw(m_screwIcon);
	m_screwNum->Draw(_target);

}


void Level::Update(sf::Time _frameTime)
{
	// rows
	for (int y = 0; y < m_contents.size(); ++y)
	{
		// cells
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			// sticky outies (grid objects)
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				m_contents[y][x][z]->Update(_frameTime);
			}
		}
	}

	//Update the tool wheel as it isn't a grid object
	m_toolWheel->Update(_frameTime);

	m_screwNum->Update(_frameTime);

	Collision();

	//If there is a pending reload waiting
	if (m_pendingReload)
	{
		//Reload level
		loadLevel(m_currentLevel);


		//Set pending reload to false
		m_pendingReload = false;
	}

	if (m_pendingLoad)
	{

		//Load new level
		loadLevel(m_levelToLoad);

		m_pendingLoad = false;
	}
	
}

void Level::Collision()
{
	// -----------------------------------------------
	// Collision Section
	// -----------------------------------------------

	for (int i = 0; i < m_collisionList.size(); ++i)
	{
		GameObject* handler = m_collisionList[i].first;
		GameObject* collider = m_collisionList[i].second;


		if (handler->isActive() && collider->isActive())
		{
			if (handler->GetBounds().intersects(collider->GetBounds()))
			{
				handler->Collide(*collider);
			}
		}
	}
}

GameObject& Level::ToolCollision(sf::FloatRect _toolRect)
{
	

	// rows
	for (int y = 0; y < m_contents.size(); ++y)
	{
		// cells
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			// sticky outies (grid objects)
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				if (_toolRect.intersects(m_contents[y][x][z]->GetBounds()))
				{
					GameObject* collider = m_contents[y][x][z];
					Player* playerCollider = dynamic_cast<Player*>(collider);
					Ladder* ladderCollider = dynamic_cast<Ladder*>(collider);
					if (playerCollider == nullptr && ladderCollider == nullptr)
						return *collider;
				}
			}
		}
	}

	GameObject* nothing = nullptr;
	return *nothing;
}

void Level::Input(sf::Event _gameEvent)
{
	for (int y = 0; y < m_contents.size(); ++y)//rows
	{
		for (int x = 0; x < m_contents[y].size(); ++x)//cells
		{
			for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
			{
				m_contents[y][x][z]->Input(_gameEvent);
			} 
		}
	}

	//If backspace is pressed, reload level
	if (_gameEvent.type == sf::Event::KeyPressed)
	{
		// Yes it was a key press!

		// What key was pressed?
		if (_gameEvent.key.code == sf::Keyboard::BackSpace)
		{
			ReloadLevel();
		}
	}

	if (_gameEvent.type == sf::Event::KeyPressed)
	{
		// Yes it was a key press!

		// What key was pressed?
		if (_gameEvent.key.code == sf::Keyboard::Escape)
		{
			exit(0);
		}
	}
}


void Level::loadLevel(levelenum _levelToLoad)
{

	//Save the player information for when you pass between levels
	Player tempactualplayer;
	if (m_player != nullptr)
	{
		tempactualplayer = *m_player;
	}

	///Cleanup the old level

	//Delete any data already in the level
	for (int y = 0; y < m_contents.size(); ++y)//rows
	{
		for (int x = 0; x < m_contents[y].size(); ++x)//cells
		{
			for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
			{
				delete m_contents[y][x][z];
			}
		}
	}

	delete m_toolWheel;
	

	//Clear out the lists
	m_background.clear();
	m_contents.clear();
	m_collisionList.clear();


	///Setup everything

	//Set current level
	m_oldLevel = m_currentLevel;
	m_currentLevel = _levelToLoad;

	sf::String nameOfLevel = std::to_string(m_currentLevel);
		switch (m_currentLevel)
		{
			case LEFT:
				nameOfLevel = "LEFT";
				break;
			case RIGHT:
				nameOfLevel = "RIGHT";
				break;
			case TOP:
				nameOfLevel = "TOP";
				break;
			case BOTTOM:
				nameOfLevel = "BOTTOM";
				break;
			case CENTER: 
				nameOfLevel = "CENTER";
				break;
		}
	



	//Set up all the game objects
	//Open our file for reading
	std::ifstream inFile;
	std::string fileName = "levels/Level " + nameOfLevel + ".txt";
	inFile.open(fileName);

	//Make sure the file was opened
	if (!inFile)
	{
		std::cerr << "Unable to open file " + fileName;
		exit(1); //Call system to stop program with error	
	}

	//Set the starting x and y coords used to position level objects
	int x = 0;
	int y = 0;

	//Create the first row in our grid
	m_background.push_back(std::vector<sf::Sprite>());
	m_contents.push_back(std::vector<std::vector<GridObject*> >());

	//setting up our player first
	Player* player = new Player();
	if (m_player != nullptr)
	{
		*player = tempactualplayer;
	}
	m_player = player;

	//Reading each character one by one from the file...
	char ch;
	//Each time, try to read the next character, execute body of loop
	while (inFile >> std::noskipws >> ch)//the noskipws means we will include the white space (spaces)
	{
		//Perform actions based on what character was read in
		if (ch == ' ')
		{
			++x;
		}
		else if (ch == '\n')
		{
			++y;
			x = 0;

			//Create a new row in our grid
			m_background.push_back(std::vector<sf::Sprite>());
			m_contents.push_back(std::vector<std::vector<GridObject*> >());
		}
		else
		{
			//Create background sprite (this is going to be some object/empty space, so we need a background)
			m_background[y].push_back(sf::Sprite(AssetManager::GetTexture("resources/graphics/environment/panels128r3.png")));
			m_background[y][x].setPosition(x*m_cellSize, y*m_cellSize);

			//Create an empty vector for our grid contents in this cell
			m_contents[y].push_back(std::vector<GridObject*>());

			if (ch == '-')
			{

			}
			else if (ch == 'G')
			{
				Ground* ground = new Ground();
				ground->setLevel(this);
				ground->setGridPosition(x, y);
				m_contents[y][x].push_back(ground);
				m_collisionList.push_back(std::make_pair(player, ground));
			}
			else if (ch == 'L')
			{
				Ladder* ladder = new Ladder();
				ladder->setLevel(this);
				ladder->setGridPosition(x, y);
				m_contents[y][x].push_back(ladder);
				m_collisionList.push_back(std::make_pair(player, ladder));
			}
			else if (ch == 'S')
			{
				Cog* cog = new Cog();
				cog->setLevel(this);
				cog->setGridPosition(x, y);
				m_contents[y][x].push_back(cog);
				m_collisionList.push_back(std::make_pair(player, cog));
			}
			else if (ch == 'N')
			{
				Screw* screw = new Screw();
				screw->setLevel(this);
				screw->setGridPosition(x, y);
				m_contents[y][x].push_back(screw);
				m_collisionList.push_back(std::make_pair(player, screw));
			}
			else if (ch == 'H')
			{
				Wood* wood = new Wood();
				wood->setLevel(this);
				wood->setGridPosition(x, y);
				m_contents[y][x].push_back(wood);
				m_collisionList.push_back(std::make_pair(player, wood));
			}
			else if (ch == 'W')
			{
				Web* web = new Web();
				web->setLevel(this);
				web->setGridPosition(x, y);
				m_contents[y][x].push_back(web);
				m_collisionList.push_back(std::make_pair(player, web));
			}
			else if (ch == 'A')
			{
				Spider* spider = new Spider();
				spider->setLevel(this);
				spider->setGridPosition(x, y);
				m_contents[y][x].push_back(spider);
			}
			else if (ch == '1')
			{
				Exit* exit = new Exit();
				exit->setLevel(this);
				exit->setGridPosition(x, y);
				exit->setExitType(1);
				m_contents[y][x].push_back(exit);
				m_collisionList.push_back(std::make_pair(player, exit));
			}
			else if (ch == '2')
			{
				Exit* exit = new Exit();
				exit->setLevel(this);
				exit->setGridPosition(x, y);
				exit->setExitType(2);
				m_contents[y][x].push_back(exit);
				m_collisionList.push_back(std::make_pair(player, exit));
			}
			else if (ch == '3')
			{
				Exit* exit = new Exit();
				exit->setLevel(this);
				exit->setGridPosition(x, y);
				exit->setExitType(3);
				m_contents[y][x].push_back(exit);
				m_collisionList.push_back(std::make_pair(player, exit));
			}
			else if (ch == '4')
			{
				Exit* exit = new Exit();
				exit->setLevel(this);
				exit->setGridPosition(x, y);
				exit->setExitType(4);
				m_contents[y][x].push_back(exit);
				m_collisionList.push_back(std::make_pair(player, exit));
			}
			else if (ch == '5')
			{
				Exit* exit = new Exit();
				exit->setLevel(this);
				exit->setGridPosition(x, y);
				exit->setExitType(5);
				m_contents[y][x].push_back(exit);
				m_collisionList.push_back(std::make_pair(player, exit));
			}
			else if (ch == '6')
			{
				Exit* exit = new Exit();
				exit->setLevel(this);
				exit->setGridPosition(x, y);
				exit->setExitType(6);
				m_contents[y][x].push_back(exit);
				m_collisionList.push_back(std::make_pair(player, exit));
			}
			else if (ch == '7')
			{
				Exit* exit = new Exit();
				exit->setLevel(this);
				exit->setGridPosition(x, y);
				exit->setExitType(7);
				m_contents[y][x].push_back(exit);
				m_collisionList.push_back(std::make_pair(player, exit));
			}
			else if (ch == 'h') //Lowercase h as opposed to an uppercase H for the wood
			{
				Hammer* hammer = new Hammer();
				hammer->setLevel(this);
				hammer->setGridPosition(x, y);
				m_contents[y][x].push_back(hammer);
				m_collisionList.push_back(std::make_pair(player, hammer));
			}
			else if (ch == 's') //Lowercase s as opposed to an uppercase S for the cog
			{
			Spanner* spanner = new Spanner();
			spanner->setLevel(this);
			spanner->setGridPosition(x, y);
			m_contents[y][x].push_back(spanner);
			m_collisionList.push_back(std::make_pair(player, spanner));
			}
			else if (ch == 'c') //Lowercase c as opposed to an uppercase H to keep in line with the other tools
			{
			Shears* shears = new Shears();
			shears->setLevel(this);
			shears->setGridPosition(x, y);
			m_contents[y][x].push_back(shears);
			m_collisionList.push_back(std::make_pair(player, shears));
			}
			else if (ch == 'P')
			{
				player->setLevel(this);
				player->setGridPosition(x, y);
				m_contents[y][x].push_back(player);
			}
			else
			{
				std::cerr << "Unrecognised character in level file: " << ch;
			}
		}	
	}

	//Close the file now that we're done
	inFile.close();

	///Tool Wheel
	ToolWheel* toolWheel = new ToolWheel();
	m_toolWheel = toolWheel;
	m_toolWheel->setPlayer(m_player);
	m_player->setToolWheel(m_toolWheel);

	///Screw Num
	ScrewNum* screwsCollected = new ScrewNum();
	m_screwNum = screwsCollected;
	m_screwNum->setPlayer(m_player);

	//Set the player position based on what exit it came out of, but not when the game first starts
	if (m_oldLevel != m_currentLevel)
	{
		switch (m_currentLevel)
		{
		case LEFT:
			switch (m_oldExit)
			{
			case Exit::LEFT:
				for (int y = 0; y < m_contents.size(); ++y)//rows
				{
					for (int x = 0; x < m_contents[y].size(); ++x)//cells
					{
						for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
						{
							GameObject* potentialExit = m_contents[y][x][z];
							Exit* exitSearch = dynamic_cast<Exit*>(potentialExit);
							if (exitSearch != nullptr && exitSearch->getExitType() == Exit::LEFT)
							{
								m_player->SetPosition(exitSearch->GetPosition().x - m_player->GetBounds().width * 2, exitSearch->GetPosition().y);
							}
						}
					}
				}
				break;
			case Exit::BOTTOMLEFT:
				for (int y = 0; y < m_contents.size(); ++y)//rows
				{
					for (int x = 0; x < m_contents[y].size(); ++x)//cells
					{
						for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
						{
							GameObject* potentialExit = m_contents[y][x][z];
							Exit* exitSearch = dynamic_cast<Exit*>(potentialExit);
							if (exitSearch != nullptr && exitSearch->getExitType() == Exit::BOTTOMLEFT)
							{
								m_player->SetPosition(exitSearch->GetPosition().x - m_player->GetBounds().width * 2, exitSearch->GetPosition().y);
							}
						}
					}
				}
				break;
			}
			break;
		case RIGHT:
			switch (m_oldExit)
			{
			case Exit::TOPRIGHT:
				for (int y = 0; y < m_contents.size(); ++y)//rows
				{
					for (int x = 0; x < m_contents[y].size(); ++x)//cells
					{
						for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
						{
							GameObject* potentialExit = m_contents[y][x][z];
							Exit* exitSearch = dynamic_cast<Exit*>(potentialExit);
							if (exitSearch != nullptr && exitSearch->getExitType() == Exit::TOPRIGHT)
							{
								m_player->SetPosition(exitSearch->GetPosition().x + exitSearch->GetBounds().width + m_player->GetBounds().width, exitSearch->GetPosition().y);
							}
						}
					}
				}
				break;
			case Exit::RIGHT:
				for (int y = 0; y < m_contents.size(); ++y)//rows
				{
					for (int x = 0; x < m_contents[y].size(); ++x)//cells
					{
						for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
						{
							GameObject* potentialExit = m_contents[y][x][z];
							Exit* exitSearch = dynamic_cast<Exit*>(potentialExit);
							if (exitSearch != nullptr && exitSearch->getExitType() == Exit::RIGHT)
							{
								m_player->SetPosition(exitSearch->GetPosition().x + exitSearch->GetBounds().width + m_player->GetBounds().width, exitSearch->GetPosition().y);
							}
						}
					}
				}
				break;
			case Exit::BOTTOMRIGHT:
				for (int y = 0; y < m_contents.size(); ++y)//rows
				{
					for (int x = 0; x < m_contents[y].size(); ++x)//cells
					{
						for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
						{
							GameObject* potentialExit = m_contents[y][x][z];
							Exit* exitSearch = dynamic_cast<Exit*>(potentialExit);
							if (exitSearch != nullptr && exitSearch->getExitType() == Exit::BOTTOMRIGHT)
							{
								m_player->SetPosition(exitSearch->GetPosition().x + exitSearch->GetBounds().width + m_player->GetBounds().width, exitSearch->GetPosition().y);
							}
						}
					}
				}
				break;
			}
			break;
		case CENTER:
			switch (m_oldExit)
			{
			case Exit::TOP:
				for (int y = 0; y < m_contents.size(); ++y)//rows
				{
					for (int x = 0; x < m_contents[y].size(); ++x)//cells
					{
						for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
						{
							GameObject* potentialExit = m_contents[y][x][z];
							Exit* exitSearch = dynamic_cast<Exit*>(potentialExit);
							if (exitSearch != nullptr && exitSearch->getExitType() == Exit::TOP)
							{
								m_player->SetPosition(exitSearch->GetPosition().x, exitSearch->GetPosition().y + exitSearch->GetBounds().height + m_player->GetBounds().height);
							}
						}
					}
				}
				break;
			case Exit::TOPRIGHT:
				for (int y = 0; y < m_contents.size(); ++y)//rows
				{
					for (int x = 0; x < m_contents[y].size(); ++x)//cells
					{
						for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
						{
							GameObject* potentialExit = m_contents[y][x][z];
							Exit* exitSearch = dynamic_cast<Exit*>(potentialExit);
							if (exitSearch != nullptr && exitSearch->getExitType() == Exit::TOPRIGHT)
							{
								m_player->SetPosition(exitSearch->GetPosition().x - m_player->GetBounds().width * 2, exitSearch->GetPosition().y);
							}
						}
					}
				}
				break;
			case Exit::LEFT:
				for (int y = 0; y < m_contents.size(); ++y)//rows
				{
					for (int x = 0; x < m_contents[y].size(); ++x)//cells
					{
						for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
						{
							GameObject* potentialExit = m_contents[y][x][z];
							Exit* exitSearch = dynamic_cast<Exit*>(potentialExit);
							if (exitSearch != nullptr && exitSearch->getExitType() == Exit::LEFT)
							{
								m_player->SetPosition(exitSearch->GetPosition().x + exitSearch->GetBounds().width + m_player->GetBounds().width, exitSearch->GetPosition().y);
							}
						}
					}
				}
				break;
			case Exit::RIGHT:
				for (int y = 0; y < m_contents.size(); ++y)//rows
				{
					for (int x = 0; x < m_contents[y].size(); ++x)//cells
					{
						for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
						{
							GameObject* potentialExit = m_contents[y][x][z];
							Exit* exitSearch = dynamic_cast<Exit*>(potentialExit);
							if (exitSearch != nullptr && exitSearch->getExitType() == Exit::RIGHT)
							{
								m_player->SetPosition(exitSearch->GetPosition().x - m_player->GetBounds().width * 2, exitSearch->GetPosition().y);
							}
						}
					}
				}
				break;
			case Exit::BOTTOMLEFT:
				for (int y = 0; y < m_contents.size(); ++y)//rows
				{
					for (int x = 0; x < m_contents[y].size(); ++x)//cells
					{
						for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
						{
							GameObject* potentialExit = m_contents[y][x][z];
							Exit* exitSearch = dynamic_cast<Exit*>(potentialExit);
							if (exitSearch != nullptr && exitSearch->getExitType() == Exit::BOTTOMLEFT)
							{
								m_player->SetPosition(exitSearch->GetPosition().x + exitSearch->GetBounds().width + m_player->GetBounds().width, exitSearch->GetPosition().y);
							}
						}
					}
				}
				break;
			case Exit::BOTTOMRIGHT:
				for (int y = 0; y < m_contents.size(); ++y)//rows
				{
					for (int x = 0; x < m_contents[y].size(); ++x)//cells
					{
						for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
						{
							GameObject* potentialExit = m_contents[y][x][z];
							Exit* exitSearch = dynamic_cast<Exit*>(potentialExit);
							if (exitSearch != nullptr && exitSearch->getExitType() == Exit::BOTTOMRIGHT)
							{
								m_player->SetPosition(exitSearch->GetPosition().x - m_player->GetBounds().width * 2, exitSearch->GetPosition().y);
							}
						}
					}
				}
				break;
			case Exit::BOTTOM:
				for (int y = 0; y < m_contents.size(); ++y)//rows
				{
					for (int x = 0; x < m_contents[y].size(); ++x)//cells
					{
						for (int z = 0; z < m_contents[y][x].size(); ++z) //Sticky outies (grid objects)
						{
							GameObject* potentialExit = m_contents[y][x][z];
							Exit* exitSearch = dynamic_cast<Exit*>(potentialExit);
							if (exitSearch != nullptr && exitSearch->getExitType() == Exit::BOTTOM)
							{
								m_player->SetPosition(exitSearch->GetPosition().x, exitSearch->GetPosition().y - m_player->GetBounds().height * 2);
							}
						}
					}
				}
				break;
			}
			break;
		}
	}

	

}

void Level::ReloadLevel()
{
	loadLevel(m_currentLevel);
}

void Level::loadNextLevel(levelenum _newLevel, Exit::exittypes _oldType)
{
	m_oldExit = _oldType;
	m_levelToLoad = _newLevel;
	m_pendingLoad = true;
}

Level::levelenum Level::GetCurrentLevel()
{
	return m_currentLevel;
}

void Level::deleteObjectAt(GridObject * _toDelete)
{
	//Don't trust other code. Make sure _toDelete is a valid pointer
	if (_toDelete != nullptr)
	{

		//Get the current position of our grid object
		sf::Vector2i Pos = _toDelete->getGridPosition();

		//Find the object in the list using an iterator and the find method
		auto it = std::find(m_contents[Pos.y][Pos.x].begin(),
			m_contents[Pos.y][Pos.x].end(),
			_toDelete);

		//If we found the object at this location, it will NOT equal the end of the vector
		if (it != m_contents[Pos.y][Pos.x].end())
		{
			//We found the object!

			//Delete the dirt
			delete *it;
			//Remove it from the old position
			m_contents[Pos.y][Pos.x].erase(it);

		}

		//Find the object in the list using an iterator and the find method
		for (auto it = m_collisionList.begin(); it != m_collisionList.end(); )
		{
			// if second thing in pair is the "to be deleted" object, then, delete the pair
			if (it->second == _toDelete)
				it = m_collisionList.erase(it); // returns pointer to next thing in list, so we don't want to add to it ourselves
			else
				++it; // we didnt delete so add to it to go to the next thing in list
		}
	}

}

void Level::checkLevelBounds()
{
}

int Level::getCellSize()
{
	return m_cellSize;
}
