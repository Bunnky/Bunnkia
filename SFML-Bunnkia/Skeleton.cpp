#include "stdafx.h"
#include "Skeleton.h"

//========================================================
//Initializer Functions
//========================================================
void Skeleton::initVariables()
{
}

void Skeleton::initAnimations()
{
	this->animationComponent->addAnimation("IDLE", 15.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("WALK_LEFT", 6.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("WALK_RIGHT", 6.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("WALK_DOWN", 6.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("WALK_UP", 6.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("ATTACK", 5.f, 0, 0, 3, 0, 32, 32);
}

void Skeleton::initAI()
{
}

void Skeleton::initGUI()
{
	this->hpBarBack.setFillColor(sf::Color::Black);
	this->hpBarBack.setSize(sf::Vector2f(64.f, 3.f));
	this->hpBarBack.setPosition(this->sprite.getPosition());
	this->hpBarBack.setOutlineColor(sf::Color(220, 220, 220, 250));
	this->hpBarBack.setOutlineThickness(0.5f);

	this->hpBar.setFillColor(sf::Color::Red);
	this->hpBar.setSize(sf::Vector2f(64.f, 3.f));
	this->hpBar.setPosition(this->sprite.getPosition());
}

//========================================================
//Constructors/Destructors
//========================================================
Skeleton::Skeleton(float x, float y, sf::Texture& texture_sheet, EnemySpawnerTile& enemy_spawner_tile, Entity& player)
	: Enemy(enemy_spawner_tile)
{
	this->initVariables();
	this->initGUI();
	this->createAnimationComponent(texture_sheet);
	this->createHitboxComponent(this->sprite, 0.f, 0.f, 32.f, 32.f);

	this->createMovementComponent(100.f, 1600.f, 1000.f);
	this->createAttributeComponent(5);
	this->generateAttributes(this->attributeComponent->level);

	std::cout << "=========NEW_SPAWN=========" << "\n";
	std::cout << green << "Spawned Skeleton!" << "\n" << white;
	std::cout << "Max Vel: " << this->getMovementComponent()->getMaxVelocity() << "\n";
	std::cout << "Level: " << this->getAttributeComponent()->level << "\n";
	std::cout << "Max HP: " << this->getAttributeComponent()->hpMax << "\n";
	std::cout << "Vitality: " << this->getAttributeComponent()->vitality << "\n";
	std::cout << "Strength: " << this->getAttributeComponent()->strength << "\n";
	std::cout << "Dex: " << this->getAttributeComponent()->dexterity << "\n";
	std::cout << "Agility: " << this->getAttributeComponent()->agility << "\n";
	std::cout << "Intelligence: " << this->getAttributeComponent()->intelligence << "\n";

	this->setPosition(x, y);
	this->initAnimations();

	this->follow = new AIFollow(*this, player);
}

Skeleton::~Skeleton()
{
	delete this->follow;
}

void Skeleton::updateAnimation(const float& dt)
{
	if (this->movementComponent->getState(IDLE))
		this->animationComponent->play("IDLE", dt);
	else if (this->movementComponent->getState(MOVING_LEFT))
		this->animationComponent->play("WALK_LEFT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
	else if (this->movementComponent->getState(MOVING_RIGHT))
		this->animationComponent->play("WALK_RIGHT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
	else if (this->movementComponent->getState(MOVING_UP))
		this->animationComponent->play("WALK_UP", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
	else if (this->movementComponent->getState(MOVING_DOWN))
		this->animationComponent->play("WALK_DOWN", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());

	if (this->damageTimer.getElapsedTime().asMilliseconds() <= this->damageTimerMax)
		this->sprite.setColor(sf::Color::Red);
	else
		this->sprite.setColor(sf::Color::White);
}

void Skeleton::update(const float& dt, sf::Vector2f& mouse_pos_view, const sf::View& view)
{
	Enemy::update(dt, mouse_pos_view, view);

	this->movementComponent->update(dt);

	//Update GUI REMOVE THIS!!!
	this->hpBar.setSize(sf::Vector2f(32.f * (static_cast<float>(this->attributeComponent->hp) / this->attributeComponent->hpMax), 3.f));
	this->hpBar.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + 32.f);

	this->hpBarBack.setSize(sf::Vector2f(32.f, 3.f));
	this->hpBarBack.setPosition(this->sprite.getPosition().x, this->sprite.getPosition().y + 32.f);

	//this->updateAttack();

	this->updateAnimation(dt);

	this->hitboxComponent->update();
	this->follow->update(dt);
}

void Skeleton::render(sf::RenderTarget& target, sf::Shader* shader, const sf::Vector2f light_position, const bool show_hitbox)
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
	target.draw(this->hpBarBack);
	target.draw(this->hpBar);

	if (show_hitbox)
		this->hitboxComponent->render(target);
}