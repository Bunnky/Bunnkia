#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H

#include<iostream>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<vector>
#include<stack>
#include<map>

#include"SFML\System.hpp"
#include"SFML\Window.hpp"
#include"SFML\Graphics.hpp"
#include"SFML\Audio.hpp"
#include"SFML\Network.hpp"

class MovementComponent
{
private:
	float maxVelocity;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	sf::Vector2f deceleration;

	//Initializer functions


public:
	MovementComponent(float maxVelocity);
	virtual ~MovementComponent();

	//Accessors
	const sf::Vector2f& getVelocity() const;

	//Functions

	void move(const float x, const float y);
	void update(const float& dt);
};

#endif
