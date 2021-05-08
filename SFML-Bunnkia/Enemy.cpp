#include "stdafx.h"
#include "Enemy.h"

//========================================================
//Initializer Functions
//========================================================
void Enemy::initVariables()
{
	this->gainExp = 10;
}

void Enemy::initAnimations()
{

}


//========================================================
//Constructors/Destructors
//========================================================
Enemy::Enemy()
{
	this->initVariables();
	this->initAnimations();
}

Enemy::~Enemy()
{
}

void Enemy::loseHP(const int hp)
{
	if (this->attributeComponent)
	{
		this->attributeComponent->loseHP(hp);
	}
}

const bool Enemy::isDead() const
{
	if (this->attributeComponent)
	{
		return this->attributeComponent->isDead();
	}

	return false;
}

const unsigned& Enemy::getGainExp() const
{
	return this->gainExp;
}

const AttributeComponent* Enemy::getAttributeComp() const
{
	if (this->attributeComponent)
		return this->attributeComponent;
	else
	{
		std::cout << "ERROR::ENEMY::ATTRIBUTECOMPONENT IS NOT INITIALIZED" << "\n";
		return nullptr;
	}
		
}
