#include "stdafx.h"
#include "Simulation.h"

//----------------------------------CONSTRUCTORS / DECONSTRUCTOR------------------------------------------------------

Simulation::Simulation(void) : tempPlanet(nullptr), tempPlanetMass(200), mouseHeldDown(false), averageSpeed("averageSpeed.txt")
{
	//Will be added soon
	//if(!averageSpeed.is_open())
	//{
	//	
	//}
}


Simulation::~Simulation(void)
{
}

//---------------------------------------OPERATOR OVERLOADS-----------------------------------------------------------

//-------------------------------------PUBLIC MEMBER FUNCTIONS--------------------------------------------------------

void Simulation::eventHandler(sf::RenderWindow &window)
{
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left))		//Since there is no function for button released, we must create our own.
	{
		if(!mouseHeldDown)		//code that is executed when the button is first pressed
		{
			mouseHeldDown = true;
			auto position = sf::Mouse::getPosition(window);
			tempPlanet = std::make_shared<Body>(tempPlanetMass, position.x, position.y);
		}
		else    //code that is executed while the button is being pressed
		{
			if(tempPlanet != nullptr)
			{
				auto cursorPosition = sf::Mouse::getPosition(window);
				sf::Vertex line[] =
				{
					sf::Vertex(sf::Vector2f(tempPlanet->xPosition, tempPlanet->yPosition)),
					sf::Vertex(sf::Vector2f(cursorPosition.x, cursorPosition.y))
				};
				window.draw(line, 2, sf::Lines);
			}
		}
	}
	else
	{
		if(mouseHeldDown)		//code that is exected when the button is released
		{
			auto cursorPosition = sf::Mouse::getPosition(window);
			double distance = calculateDistance(tempPlanet->xPosition, tempPlanet->yPosition, cursorPosition.x, cursorPosition.y);
			
			double deltaX = tempPlanet->xPosition - cursorPosition.x;
			double deltaY = tempPlanet->yPosition - cursorPosition.y;

			tempPlanet->setVelocity(deltaX*500/(tempPlanet->mass), deltaY*300/(tempPlanet->mass));

			planetList.push_back(*tempPlanet);
			tempPlanet = nullptr;
			mouseHeldDown = false;
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		tempPlanetMass += 25;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if(tempPlanetMass > 25)
			tempPlanetMass -= 25;
	}

}

void Simulation::runOneIteration(sf::RenderWindow &window, sf::Time &elapsed, double gravConst)
{
	eventHandler(window);
	collisionResolution();		//Each of these functions modifies the members of planetList in a specific way.
	sumForces(gravConst);
	update(elapsed);

	drawToWindow(window);
}

void Simulation::populate(int number, int xMin, int yMin, int xMax, int yMax)		//populates the planetList with randomly generated planets
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> massDistribution(10, 1000);
	std::uniform_int_distribution<int> xDistribution(xMin, xMax);
	std::uniform_int_distribution<int> yDistribution(yMin, yMax);

	for (int i = 0; i < number; i++)
	{
		planetList.emplace_back(massDistribution(generator), xDistribution(generator), yDistribution(generator));
	}
}

void Simulation::addPlanet(double mass, double xPosition, double yPosition, double xVelocity, double yVelocity)
{
	planetList.emplace_back(mass, xPosition, yPosition);
}

//------------------------------------PRIVATE MEMBER FUNCTIONS--------------------------------------------------------

void Simulation::collisionResolution()
{
	bool collisionDetected = false;
	auto first = planetList.begin();
	while(first != planetList.end())
	{
		for(auto second = planetList.begin(); second != planetList.end(); second++)
		{
			if( (*first) != (*second) && first->hasCollided(*second))
			{
				planetList.push_back((*first) + (*second));
				planetList.erase(second);
				first = planetList.erase(first);			//erase the two collided planets.
				collisionDetected = true;
				break;
			}
		}
		if(collisionDetected)		//If there is a collision, then "first" automatically points to the next value, so there is no need to increment it.
			collisionDetected = false;
		else
			first++;
	}
}

void Simulation::sumForces(double gravConst)
{
	for(Body &c : planetList)
	{
		for(Body &d : planetList)
		{
			if(c != d)
				c.addForces(d, gravConst);
		}
	}
}

void Simulation::update(sf::Time &elapsed)
{
	for(Body &c : planetList)
	{
		c.updateVelocity(elapsed);
		c.updatePosition(elapsed);
		c.zeroAccel();
	}
}

void Simulation::drawToWindow(sf::RenderWindow &window)
{
	for (Body &c : planetList)
	{
		window.draw(c.circle);
	}

	if(tempPlanet != nullptr)
	{
		window.draw(tempPlanet->circle);
	}
}

double Simulation::calculateDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow( (x2 - x1), 2) + pow( (y2 - y1), 2));
}