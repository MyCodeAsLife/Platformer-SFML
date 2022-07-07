#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>

const int gameWidth(250);
const int gameHeight(150);

int main()
{
	
	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "Platformer",
		sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);


	while (window.isOpen())
	{
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Window closed or escape key pressed: exit
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
		}

		// Clear the window
		window.clear(sf::Color(50, 200, 50));

		// Display things on screen
		window.display();
	}

	return EXIT_SUCCESS;
}
