#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <random>
#include <memory>
#include <fstream>

#include "Body.h"

class Simulation
{
public:
	Simulation(void);
	~Simulation(void);

	void eventHandler(sf::RenderWindow &window);
	void runOneIteration(sf::RenderWindow &window, sf::Time &elapsed, double gravConst = 10.0);
	void populate(int number, int xMin, int yMin, int xMax, int yMax);
	void addPlanet(double mass, double xPosition, double yPosition, double xVelocity = 0.0, double yVelocity = 0.0);

private:
	std::list<Body> planetList;
	std::shared_ptr<Body> tempPlanet;
	double tempPlanetMass;
	bool mouseHeldDown;

	std::ofstream averageSpeed;

	void collisionResolution();
	void sumForces(double gravConst);
	void update(sf::Time &elapsed);
	void drawToWindow(sf::RenderWindow &window);
	double Simulation::calculateDistance(double x1, double y1, double x2, double y2);
};