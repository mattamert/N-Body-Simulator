#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

class Body
{
public:
	friend class Simulation;
	Body(double givenMass, double givenXPosition, double givenYPosition, double givenXVelocity = 0, double givenYVelocity = 0);
	~Body(void);

	bool operator==(const Body &rhs);
	bool operator!=(const Body &rhs);
	Body operator+(const Body &rhs);

	sf::CircleShape circle;

	void addForces(Body &otherPlanet, double gravitationalConstant);
	void updateVelocity(sf::Time &elapsed);
	void updatePosition(sf::Time &elapsed);
	bool hasCollided(Body &otherPlanet);
	void setVelocity(double xVelocity, double yVelocity);

private:
	double mass, radius,
          xPosition, yPosition,
          xVelocity, yVelocity,
	      xAccel, yAccel;

	double calculateRadius(double mass);
	double calculateDistance(double x1, double y1, double x2, double y2);
	void zeroAccel();
	double findWeightedAverage(double value1, double weight1, double value2, double weight2);
};

