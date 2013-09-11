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

	void addForces(Body &otherPlanet, double gravitationalConstant);		//adds the force from the given body
	void updateVelocity(sf::Time &elapsed);		//updates velocity from the new accel value
	void updatePosition(sf::Time &elapsed);		//updates position from the new velocity value
	bool hasCollided(Body &otherPlanet);		//determines whether this body has collided with another, given body
	void setVelocity(double xVelocity, double yVelocity);

private:
	double mass, radius,
          xPosition, yPosition,
          xVelocity, yVelocity,
	      xAccel, yAccel;

	double calculateRadius(double mass);		//calculates the radius from the given mass
	double calculateDistance(double x1, double y1, double x2, double y2);		//distance formula
	void zeroAccel();							//sets the acceleration values to 0.
	double findWeightedAverage(double value1, double weight1, double value2, double weight2);
			// find WeightedAverage returns a value based on 2 given values and 2 given weights.  value1 will affect the
			//  average more if its weight is higher, and visa versa
};

