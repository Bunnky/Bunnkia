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
Enemy::Enemy(float x, float y, sf::Texture& texture_sheet)
{
	this->initVariables();

	/*This is where we alter the hitbox*/
	this->createHitboxComponent(this->sprite, 0.f, 0.f, 32.f, 32.f);
	this->createMovementComponent(500.f, 2000.f, 1000.f);
	this->createAnimationComponent(texture_sheet);
	this->createAttributeComponent(1);
	this->createSkillComponent();

	this->setPosition(x, y);
	this->initAnimations();
}

Enemy::~Enemy()
{

}

void Enemy::update(const float& dt, sf::Vector2f& mouse_pos_view)
{
	this->movementComponent->update(dt);

	//this->updateAttack();

	//this->updateAnimation(dt);

	this->hitboxComponent->update();
}

void Enemy::render(sf::RenderTarget& target, sf::Shader* shader, const bool show_hitbox)
{
	if (shader)
	{
		shader->setUniform("hasTexture", true);
		shader->setUniform("lightPos", this->getCenter());
		target.draw(this->sprite, shader);
	}
	else
	{
		target.draw(this->sprite);
	}

	if (show_hitbox)
		this->hitboxComponent->render(target);
}
