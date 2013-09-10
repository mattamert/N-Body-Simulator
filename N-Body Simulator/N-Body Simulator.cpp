#include "stdafx.h"

#include <SFML/Graphics.hpp>

#include "Simulation.h"
#include "Body.h"

void updateFPS(sf::Time &elapsed, sf::Text &text, sf::RenderWindow &window);

int _tmain(int argc, _TCHAR* argv[])
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(1200, 800), "SFML works!", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	sf::Font font;
	if(!font.loadFromFile("Resources/sansation.ttf"))
	{
		window.setTitle("Failed to load text");
	}

	sf::Text text;
	text.setFont(font);
	text.setString("");
	text.setColor(sf::Color::White);
	text.setPosition(0,0);

	Simulation sim;
	sim.populate(80, 0, 0, window.getSize().x, window.getSize().y);

	sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		sf::Time elapsed = clock.restart();
		window.clear();

		sim.runOneIteration(window, elapsed, 15.0);

		updateFPS(elapsed, text, window);
        window.display();
    }
    return 0;
}

void updateFPS(sf::Time &elapsed, sf::Text &text, sf::RenderWindow &window)
{
	static int framesUpdated = 1;
	static int totalTime = 0;
	
	if(framesUpdated == 5)
	{
		if(totalTime != 0)
		{
			int average = totalTime / 5;
			int fps = 1000 / average;
			std::string s = std::to_string(fps);
			text.setString(s);
		}
		framesUpdated = 0;
		totalTime = 0;
	}
	else
	{
		framesUpdated++;
		totalTime += elapsed.asMilliseconds();
	}
	window.draw(text);
}