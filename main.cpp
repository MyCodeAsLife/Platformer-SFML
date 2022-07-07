#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "Map.h"
#include "Anim.h"
#include "Player.h"


const int WINDOW_HEIGHT(250);
const int WINDOW_WIDTH(400);

int main()
{
	
	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "Platformer");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(240);

	// Tile Set
	Texture tileSet_texture;
	tileSet_texture.loadFromFile("resources/images/Mario_Tileset.png");
	// Анимация персонажа и сам персонаж
	Texture q;
	q.loadFromFile("resources/images/fang.png");
	AnimationManager anim;
	anim.create("walk", q, 0, 245, 39, 50, 6, 0.01f, 41);
	anim.create("jump", q, 0, 527, 32, 32, 4, 0.01f, 35);
	anim.create("duck", q, 7, 438, 60, 18, 1, 0.01f);
	anim.create("stay", q, 0, 191, 42, 52, 3, 0.004f, 44);
	anim.create("shoot", q, 0, 575, 44, 48, 5, 0.008f, 44);
	Player p1(anim);

	// Карта
	Sprite tile_sprite(tileSet_texture);

	// Камера
	View camera;
	camera.reset(FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	Clock clock;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMilliseconds();
		clock.restart();

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


		// Обработка столкновения с врагом
		if (p1.m_rect.intersects(en1.m_rect))	// Если спрайт игрока и врага пересекаются
			if (en1.m_life)						// Если враг жив
				if (p1.m_dy > 0)				// Если игрок падает
				{
					en1.m_life = false;
					en1.m_dx = 0.f;
					p1.m_dy = -0.8f;
				}
				else                            // Если игрок не падает 
				{
					p1.m_sprite.setColor(Color::Red);
				}

		anim.set("stay");
		if (Keyboard::isKeyPressed(Keyboard::Right))
			anim.set("walk");
		if (Keyboard::isKeyPressed(Keyboard::Left))
			anim.set("walk");
		if (Keyboard::isKeyPressed(Keyboard::Up))
			anim.set("jump");
		if (Keyboard::isKeyPressed(Keyboard::Down))
			anim.set("duck");
		if (Keyboard::isKeyPressed(Keyboard::Space))
			anim.set("shoot");

		anim.tick(time);

		// Clear the window
		window.clear(sf::Color(50, 200, 50));

		anim.draw(window);

		// Display things on screen
		window.display();
	}

	return 0;
}
