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

	this->setPostition(x, y);

	this->createMovementComponent(500.f, 20.f, 10.f);
	this->createAnimationComponent(texture_sheet);

	this->animationComponent->addAnimation("IDLE_LEFT", 10.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("WALK_LEFT", 10.f, 0, 1, 3, 1, 32, 32);
}

Player::~Player()
{
}
//Functions
void Player::update(const float& dt)
{
	this->movementComponent->update(dt);

	if(this->movementComponent->getState(IDLE))
		this->animationComponent->play("IDLE_LEFT", dt);
	else if(this->movementComponent->getState(MOVING_LEFT))
		this->animationComponent->play("WALK_LEFT", dt);
}
