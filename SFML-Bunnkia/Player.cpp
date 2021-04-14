#include "stdafx.h"
#include "Player.h"


//========================================================
//Initializer Functions
//========================================================
void Player::initVariables()
{
	this->attacking = false;
}

void Player::initComponents()
{

}


//========================================================
//Constructors/Destructors
//========================================================
Player::Player(float x, float y, sf::Texture& texture_sheet)
{
	this->initVariables();	

	this->setPosition(x, y);

	/*This is where we alter the hitbox*/
	this->createHitboxComponent(this->sprite, 0.f, 0.f, 32.f, 32.f);
	this->createMovementComponent(500.f, 2000.f, 1000.f);
	this->createAnimationComponent(texture_sheet);
	this->createAttributeComponent(1);

	this->animationComponent->addAnimation("IDLE", 11.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("WALK", 6.f, 0, 1, 3, 1, 32, 32);
	this->animationComponent->addAnimation("ATTACK", 5.f, 0, 2, 4, 2, 32 * 2, 32);
}

Player::~Player()
{

}

AttributeComponent* Player::getAttributeComponent()
{
	return this->attributeComponent;
}

//========================================================
//Functions
//========================================================
void Player::updateAttack()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->attacking = true;
	}
}
void Player::updateAnimation(const float& dt)
{
	if (this->attacking)
	{
		//Set origin depending on direction
		if (this->sprite.getScale().x > 0.f) //Facing left
		{
			this->sprite.setOrigin(32.f, 0.f);
		}
		else //Facing right
		{
			this->sprite.setOrigin(32.f + 32.f + 0.f, 0.f);
		}

		//Animate and check for animation end
		if (this->animationComponent->play("ATTACK", dt, true))
		{
			this->attacking = false;
			
			if (this->sprite.getScale().x > 0.f) //Facing left
			{
				this->sprite.setOrigin(0.f, 0.f);
			}
			else //Facing right
			{
				this->sprite.setOrigin(32.f + 0.f, 0.f);
			}
		}
	}

	if (this->movementComponent->getState(IDLE))
	{
		this->animationComponent->play("IDLE", dt);
	}
	else if (this->movementComponent->getState(MOVING_LEFT))
	{
		if (this->sprite.getScale().x < 0.f)
		{
			this->sprite.setOrigin(0.f, 0.f);
			this->sprite.setScale(1.f, 1.f);
		}

		this->animationComponent->play("WALK", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
	}
	else if (this->movementComponent->getState(MOVING_RIGHT))
	{
		if (this->sprite.getScale().x > 0.f)
		{
			this->sprite.setOrigin(32.f, 0.f);
			this->sprite.setScale(-1.f, 1.f);
		}

		this->animationComponent->play("WALK", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
	}
	else if (this->movementComponent->getState(MOVING_UP))
	{
		this->animationComponent->play("WALK", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
	}
	else if (this->movementComponent->getState(MOVING_DOWN))
	{
		this->animationComponent->play("WALK", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
	}
}

void Player::update(const float& dt)
{
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	//	this->attributeComponent->gainExp(20);

	this->movementComponent->update(dt);

	this->updateAttack();

	this->updateAnimation(dt);		

	this->hitboxComponent->update();
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);

	if (this->hitboxComponent)
		this->hitboxComponent->render(target);
}
