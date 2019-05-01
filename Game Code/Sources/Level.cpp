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

Level::Level()
	: m_player(nullptr)
	, m_toolWheel(nullptr)
	, m_cellSize(64.0f)
	, m_currentLevel(0)
	, m_pendingReload(false)
	, m_background()
	, m_contents()
	, m_collisionList()
	, currentLevel(RIGHT)
{
	loadLevel(1);
}

void Level::Draw(sf::RenderTarget & _target)
{
	///Camera

	//Create and update camera
	sf::View camera = _target.getDefaultView();
	camera.setCenter(m_player->GetPosition());
	//camera.setCenter(0, 0);

	//TODO: Adjust camera as needed
	camera.zoom(1);


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

	///UI
	// Draw UI to the window
	_target.setView(_target.getDefaultView());


	//Draw UI objects
	if (m_toolWheel->isActive())
	{
		m_toolWheel->SetPosition(camera.getSize().x / 2 - m_toolWheel->GetBounds().width / 2 + m_player->GetBounds().width / 2, 
			camera.getSize().y / 2 - m_toolWheel->GetBounds().height / 2 + m_player->GetBounds().height / 2);
		m_toolWheel->Draw(_target);
	}


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
	//m_toolWheel->Position(m_player->GetPosition());
	m_toolWheel->Update(_frameTime);

	Collision();

	//If there is a pending reload waiting
	if (m_pendingReload)
	{
		//Reload level
		loadLevel(m_currentLevel);


		//Set pending reload to false
		m_pendingReload = false;
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

bool Level::Collision(sf::RectangleShape _testRect)
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
				if (_testRect.getGlobalBounds().intersects(m_contents[y][x][z]->GetBounds()))
				{
					if (m_contents[y][x][z]->getBlocksMovement())
					{
						return true;
					}
				}
			}
		}
	}

	return false;
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


void Level::loadLevel(int _levelToLoad)
{

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
	

	//Clear out the lists
	m_background.clear();
	m_contents.clear();


	///Setup everything

	//Set current level
	m_currentLevel = _levelToLoad;

	//Set up all the game objects
	//Open our file for reading
	std::ifstream inFile;
	std::string fileName = "levels/Level" + std::to_string(m_currentLevel) + ".txt";
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
			m_background[y].push_back(sf::Sprite(AssetManager::GetTexture("resources/graphics/ground.png")));
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
			}
			else if (ch == 'S')
			{
				Cog* cog = new Cog();
				cog->setLevel(this);
				cog->setGridPosition(x, y);
				m_contents[y][x].push_back(cog);
			}
			else if (ch == 'N')
			{
				Screw* screw = new Screw();
				screw->setLevel(this);
				screw->setGridPosition(x, y);
				m_contents[y][x].push_back(screw);
			}
			else if (ch == 'H')
			{
				Wood* wood = new Wood();
				wood->setLevel(this);
				wood->setGridPosition(x, y);
				m_contents[y][x].push_back(wood);
			}
			else if (ch == 'W')
			{
				Web* web = new Web();
				web->setLevel(this);
				web->setGridPosition(x, y);
				m_contents[y][x].push_back(web);
			}
			else if (ch == 'A')
			{
				Spider* spider = new Spider();
				spider->setLevel(this);
				spider->setGridPosition(x, y);
				m_contents[y][x].push_back(spider);
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

	//Close the file now that we're done

	///Tool Wheel
	ToolWheel* toolWheel = new ToolWheel();
	m_toolWheel = toolWheel;
	
	

	
}

void Level::ReloadLevel()
{
	loadLevel(m_currentLevel);
}

void Level::loadNextLevel()
{
}

int Level::GetCurrentLevel()
{
	return m_currentLevel;
}

int Level::getCellSize()
{
	return m_cellSize;
}
