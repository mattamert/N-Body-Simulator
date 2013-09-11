#include "stdafx.h"
#include "Body.h"


//----------------------------------CONSTRUCTORS / DECONSTRUCTOR------------------------------------------------------

Body::Body(double givenMass, double givenXPosition, double givenYPosition, double givenXVelocity, double givenYVelocity): 
	mass(givenMass), radius(calculateRadius(givenMass)), 
	xPosition(givenXPosition), yPosition(givenYPosition),
	xVelocity(givenXVelocity), yVelocity(givenYVelocity),
	xAccel(0), yAccel(0)
{
	//initializes the sfml circle  (cannot be done in init list)
	circle.setRadius(radius);
	circle.setOrigin(radius, radius);
	circle.setFillColor(sf::Color::Magenta);
	circle.setPosition(givenXPosition, givenYPosition);
}


Body::~Body(void)
{
}

//---------------------------------------OPERATOR OVERLOADS-----------------------------------------------------------

bool Body::operator==(const Body &rhs)
{
	return (xPosition == rhs.xPosition &&
		yPosition == rhs.yPosition &&
		mass == rhs.mass &&
		xVelocity == rhs.xVelocity &&
		yVelocity == rhs.yVelocity);
}

bool Body::operator!=(const Body &rhs)
{
	return !(xPosition == rhs.xPosition &&
		yPosition == rhs.yPosition &&
		mass == rhs.mass &&
		xVelocity == rhs.xVelocity &&
		yVelocity == rhs.yVelocity);
}

Body Body::operator+(const Body &rhs)
{
	Body newBody((this->mass + rhs.mass), findWeightedAverage(xPosition, mass, rhs.xPosition, rhs.mass), findWeightedAverage(yPosition, mass, rhs.yPosition, rhs.mass) );
	newBody.xVelocity = findWeightedAverage(xVelocity, mass, rhs.xVelocity, rhs.mass);
	newBody.yVelocity = findWeightedAverage(yVelocity, mass, rhs.yVelocity, rhs.mass);

	return newBody;
}

//---------------------------------------PUBLIC MEMBER FUNCTIONS------------------------------------------------------

void Body::addForces(Body &otherBody, double gravConst)
{
	double distance = calculateDistance(xPosition, yPosition, otherBody.xPosition, otherBody.yPosition);
	double magnitude = (gravConst)*((mass)*(otherBody.mass)) / (pow(distance, 2));

	double deltaX = otherBody.xPosition - xPosition;
	double deltaY = otherBody.yPosition - yPosition;

	xAccel += (magnitude*(deltaX / distance))/mass;
	yAccel += (magnitude*(deltaY / distance))/mass;
}

void Body::updateVelocity(sf::Time &elapsed)
{
	xVelocity += xAccel * elapsed.asSeconds();
	yVelocity += yAccel * elapsed.asSeconds();
}

void Body::updatePosition(sf::Time &elapsed)
{
	xPosition += xVelocity * elapsed.asSeconds() + (double)((1.0/2.0)*(xAccel * pow(elapsed.asSeconds(), 2)));
	yPosition += yVelocity * elapsed.asSeconds() + (double)((1.0/2.0)*(yAccel * pow(elapsed.asSeconds(), 2)));

	circle.setPosition( xPosition , yPosition);
}

bool Body::hasCollided(Body &otherPlanet)
{
	return (  calculateDistance(xPosition, yPosition, otherPlanet.xPosition, otherPlanet.yPosition) 
		< (radius + otherPlanet.radius)  );
}

void Body::setVelocity(double givenXVelocity, double givenYVelocity)
{
	xVelocity = givenXVelocity;
	yVelocity = givenYVelocity;
}

//-------------------------------------------PRIVATE MEMBER FUNCTIONS------------------------------------------------------

double Body::calculateRadius(double givenMass)
{
	//return sqrt((double)(givenMass/3.14159265358979));  //this is for 2 dimensions
	return pow((double)((3.0/(4.0*3.14159265358979))*mass),(double)(1.0/3.0)); //this is for 3 dimensions
}

double Body::calculateDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow( (x2 - x1), 2) + pow( (y2 - y1), 2));
}

void Body::zeroAccel()
{
	xAccel = 0;
	yAccel = 0;
}

double Body::findWeightedAverage(double value1, double weight1, double value2, double weight2)
{
	double totalWeight = weight1 + weight2;

	double contirbution1 = value1 * (weight1 / totalWeight);
	double contribution2 = value2 * (weight2 / totalWeight);

	return (contirbution1 + contribution2);
}