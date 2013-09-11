#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <memory>
#include <fstream>

#include <list>
#include <vector>

#include "Body.h"

class Simulation
{
public:
	Simulation(void);
	~Simulation(void);

	void eventHandler(sf::RenderWindow &window);		//handles the input from the user or any necessary events
	void runOneIteration(sf::RenderWindow &window, sf::Time &elapsed, double gravConst = 10.0);		//calculates the new acceleration, velocity, and position for each body, and updates everything
	void populate(int number, int xMin, int yMin, int xMax, int yMax);								//populates the screen with randomly generated bodies
	void addPlanet(double mass, double xPosition, double yPosition, double xVelocity = 0.0, double yVelocity = 0.0);		//dynamically adds a new planet.  Used when there is a collision or user input

private:
	std::vector<Body> planetList;			//although vectors are slow at adding/removing items (which happens a lot in the program), it still has better performance than linked lists
	std::shared_ptr<Body> tempPlanet;		//a planet that is used for when the user is trying to create a new one. It is separated from the rest because it should not affect the other planets before the user releases the mouse button
	double tempPlanetMass;					//holds the mass of the temp planet.  Can be adjusted with up and down arrows.
	
	bool mouseHeldDown;					//Holds whether the mouse is held down.  true for down, false for up.

	//NOT IMPLEMENTED YET
	//std::ofstream averageSpeed;		//this will hold the file for the average speed.  Will be able to be opened with my other project, the graphing tool

	void collisionResolution();						//determines if there is a collision, and if so, does the appropriate action
	void sumForces(double gravConst);				//adds the forces given from every other body in the simulation, and determines the acceleration given by said forces
	void update(sf::Time &elapsed);					//updates the position based on the (now updated) acceleration
	void drawToWindow(sf::RenderWindow &window);	//draws all the bodies to the screen

	double Simulation::calculateDistance(double x1, double y1, double x2, double y2);		//distance formula
};