#include "stdafx.h"
#include "Goblin.h"

//========================================================
//Initializer Functions
//========================================================
void Goblin::initVariables()
{

}

void Goblin::initAnimations()
{
	this->animationComponent->addAnimation("IDLE", 15.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("WALK_LEFT", 6.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("WALK_RIGHT", 6.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("WALK_DOWN", 6.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("WALK_UP", 6.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("ATTACK", 5.f, 0, 0, 3, 0, 32, 32);
}

//========================================================
//Constructors/Destructors
//========================================================
Goblin::Goblin(float x, float y, sf::Texture& texture_sheet)
	: Enemy()
{
	this->initVariables();

	/*This is where we alter the hitbox*/
	this->createHitboxComponent(this->sprite, 0.f, 0.f, 32.f, 32.f);
	this->createMovementComponent(50.f, 1600.f, 1000.f);
	this->createAnimationComponent(texture_sheet);
	this->createAttributeComponent(1);

	this->setPosition(x, y);
	this->initAnimations();
}

Goblin::~Goblin()
{

}

void Goblin::takeDamage(const int damage)
{
	this->attributeComponent->loseHP(damage);
}

void Goblin::updateAnimation(const float& dt)
{
	if (this->movementComponent->getState(IDLE))
	{
		this->animationComponent->play("IDLE", dt);
	}
	else if (this->movementComponent->getState(MOVING_LEFT))
	{
		this->animationComponent->play("WALK_LEFT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
	}
	else if (this->movementComponent->getState(MOVING_RIGHT))
	{
		this->animationComponent->play("WALK_RIGHT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
	}
	else if (this->movementComponent->getState(MOVING_UP))
	{
		this->animationComponent->play("WALK_UP", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
	}
	else if (this->movementComponent->getState(MOVING_DOWN))
	{
		this->animationComponent->play("WALK_DOWN", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
	}
}

void Goblin::update(const float& dt, sf::Vector2f& mouse_pos_view)
{
	this->movementComponent->update(dt);

	//this->updateAttack();

	this->updateAnimation(dt);

	this->hitboxComponent->update();
}

void Goblin::render(sf::RenderTarget& target, sf::Shader* shader, const sf::Vector2f light_position, const bool show_hitbox)
{
	if (shader)
	{
		shader->setUniform("hasTexture", true);
		shader->setUniform("lightPos", light_position);
		target.draw(this->sprite, shader);
	}
	else
	{
		target.draw(this->sprite);
	}

	if (show_hitbox)
		this->hitboxComponent->render(target);
}