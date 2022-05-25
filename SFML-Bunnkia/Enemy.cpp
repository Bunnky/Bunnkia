#include "stdafx.h"
#include "Enemy.h"

//========================================================
//Initializer Functions
//========================================================
void Enemy::initVariables()
{
	this->gainExp = 10;
	this->damageTimerMax = 1000;
	this->despawnTimerMax = 1000;
}

void Enemy::initAnimations()
{

}


//========================================================
//Constructors/Destructors
//========================================================
Enemy::Enemy(EnemySpawnerTile& enemy_spawner_tile)
	: enemySpawnerTile(enemy_spawner_tile)
{
	this->initVariables();
	this->initAnimations();
}

Enemy::~Enemy()
{
}

void Enemy::generateAttributes(const unsigned level)
{
	this->gainExp = level * (rand() % 5 + 1);
}

void Enemy::loseHP(const int hp)
{
	if (this->attributeComponent)
	{
		this->attributeComponent->loseHP(hp);
	}
}

EnemySpawnerTile& Enemy::getEnemySpawnerTile()
{
	return this->enemySpawnerTile;
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

const bool Enemy::getDamageTimerDone() const
{
	return this->damageTimer.getElapsedTime().asMilliseconds() >= this->damageTimerMax;
}

void Enemy::resetDamageTimer()
{
	this->damageTimer.restart();
}

const bool Enemy::getDespawnTimerDone() const
{
	return this->despawnTimer.getElapsedTime().asMilliseconds() >= this->despawnTimerMax;
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

void Enemy::update(const float& dt, sf::Vector2f& mouse_pos_view, const sf::View& view)
{
	if (vectorDistance(this->getPosition(), view.getCenter()) < 1000.f)
		this->despawnTimer.restart();
}