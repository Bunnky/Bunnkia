#include "stdafx.h"
#include "Enemy.h"

//========================================================
//Initializer Functions
//========================================================
void Enemy::initVariables()
{
	
}

void Enemy::initAnimations()
{
	this->animationComponent->addAnimation("IDLE", 15.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("WALK_LEFT", 6.f, 0, 1, 3, 1, 32, 32);
	this->animationComponent->addAnimation("WALK_RIGHT", 6.f, 4, 1, 7, 1, 32, 32);
	this->animationComponent->addAnimation("WALK_DOWN", 6.f, 0, 1, 3, 1, 32, 32);
	this->animationComponent->addAnimation("WALK_UP", 6.f, 4, 1, 7, 1, 32, 32);
	this->animationComponent->addAnimation("ATTACK", 5.f, 0, 2, 4, 2, 32 * 2, 32);

	//========================================================
	//CHANGE THESE IF WE WANT TO DO DIRECTIONAL PLAYER SPRITES
	//========================================================
	//this->animationComponent->addAnimation("WALK_DOWN", 6.f, 0, 1, 3, 1, 32, 32);
	//this->animationComponent->addAnimation("WALK_LEFT", 6.f, 0, 1, 3, 1, 32, 32);
	//this->animationComponent->addAnimation("WALK_RIGHT", 6.f, 0, 1, 3, 1, 32, 32);
	//this->animationComponent->addAnimation("WALK_UP", 6.f, 0, 1, 3, 1, 32, 32);
}


//========================================================
//Constructors/Destructors
//========================================================
Enemy::Enemy(EnemySpawner& enemy_spawner, float x, float y, sf::Texture& texture_sheet)
	: enemySpawner(enemy_spawner)
{
	this->initVariables();

	/*This is where we alter the hitbox*/
	this->createHitboxComponent(this->sprite, 0.f, 0.f, 32.f, 32.f);
	this->createMovementComponent(200.f, 50.f, 25.f);
	this->createAnimationComponent(texture_sheet);

	this->setPosition(x, y);
	this->initAnimations();
}

Enemy::~Enemy()
{

}

void Enemy::updateAnimation(const float& dt)
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

void Enemy::update(const float& dt, sf::Vector2f& mouse_pos_view)
{
	this->movementComponent->update(dt);

	//this->updateAttack();

	this->updateAnimation(dt);

	this->hitboxComponent->update();
}

void Enemy::render(sf::RenderTarget& target, sf::Shader* shader, const sf::Vector2f light_position, const bool show_hitbox)
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
