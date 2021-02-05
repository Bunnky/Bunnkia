#include "Player.h"


//Initializer functions
void Player::initVariables()
{

}

void Player::initComponents()
{

}


//Constructures/Destructors
Player::Player(float x, float y, sf::Texture& texture_sheet)
{
	this->initVariables();	

	this->setTexture(texture_sheet);
	this->setPostition(x, y);

	this->createMovementComponent(500.f, 20.f, 10.f);
	this->createAnimationComponent(this->sprite, texture_sheet);

	this->animationComponent->addAnimation("IDLE_LEFT", 100.f, 1, 1, 4, 1, 32, 32);
}

Player::~Player()
{
}
