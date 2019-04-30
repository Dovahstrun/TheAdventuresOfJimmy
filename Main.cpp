// Library Includes
#include <SFML/Window.hpp>	
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <cstdlib>
#include <ctime>

// Project Includes
#include "Framework/Headers/AssetManager.h"
#include "Game Code/Headers/Level.h"

// The main() Function - entry point for our program
int main()
{
	///-------------------------------------------------
	///SETUP
	///-------------------------------------------------

	// Window - to draw to the screen
	sf::RenderWindow gameWindow;
	gameWindow.create(sf::VideoMode::getDesktopMode(), "The Adventures of Jimmy", sf::Style::Titlebar | sf::Style::Close);

	// Seed our random number generator 
	srand(time(NULL));

	// Create AssetManager
	AssetManager assets;

	// Game Clock - to keep track of time passed each frame
	sf::Clock gameClock;

	//Create the game level
	Level ourLevel;

	///-------------------------------------------------
	///END SETUP
	///-------------------------------------------------



	///-------------------------------------------------
	///GAME LOOP
	///-------------------------------------------------

	while (gameWindow.isOpen()) //Beginning of the game loop
	{
		///---------------------------------------------
		///INPUT
		///---------------------------------------------

		// Check all events since the last frame and process each one until there are no more

		sf::Event gameEvent; //A variable to store any event

		while (gameWindow.pollEvent(gameEvent))
		{
			//Pass event to input objects
			ourLevel.Input(gameEvent);

			// Did the player try to close the window?
			if (gameEvent.type == sf::Event::Closed)
			{
				// If so, call the close function on the window.
				gameWindow.close();
			}

		} // End event polling loop

		///---------------------------------------------
		///END INPUT
		///---------------------------------------------



		///---------------------------------------------
		///UPDATE
		///---------------------------------------------

		// Get the time passed since the last frame and restart our game clock
		sf::Time frameTime = gameClock.restart();

		//Pass update to level
		ourLevel.Update(frameTime);

		///---------------------------------------------
		///END UPDATE
		///---------------------------------------------



		///---------------------------------------------
		///DRAW
		///---------------------------------------------

		// Clear the window to a single colour
		gameWindow.clear(sf::Color::Cyan);


		//Pass draw to level
		ourLevel.Draw(gameWindow);

		// Display the window contents on the screen
		gameWindow.display();

		///---------------------------------------------
		///END DRAW
		///---------------------------------------------

	} // End of Game Loop

	return 0;

} // End of main() Function