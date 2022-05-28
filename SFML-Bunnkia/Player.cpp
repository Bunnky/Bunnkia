#include "stdafx.h"

#include "Player.h"


//========================================================
//Initializer Functions
//========================================================
void Player::initVariables()
{
	this->initAttack = false;
	this->attacking = false;
	this->weapon = new Sword(1, 2, 5, 60, 20, "gamedata/Resources/Images/Sprites/Player/sword4.png");
	this->weapon->generate(1, 3);

	this->damageTimerMax = 500;
}

void Player::initComponents()
{

}

void Player::initAnimations()
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

void Player::initInventory()
{

}


//========================================================
//Constructors/Destructors
//========================================================
Player::Player(float x, float y, sf::Texture& texture_sheet)
{
	this->initVariables();	

	/*This is where we alter the hitbox*/
	this->createHitboxComponent(this->sprite, 0.f, 0.f, 32.f, 32.f);
	//Player Movement speed
	this->createMovementComponent(500.f, 2000.f, 1000.f);
	this->createAnimationComponent(texture_sheet);
	this->createAttributeComponent(1);
	this->createSkillComponent();

	this->setPosition(x, y);
	this->initAnimations();

	this->initInventory();
}

Player::~Player()
{
	delete this->inventory;
	delete this->weapon;
}
//========================================================
//Accessors
//========================================================
AttributeComponent* Player::getAttributeComponent()
{
	return this->attributeComponent;
}

Weapon* Player::getWeapon() const
{
	return this->weapon;
}

const std::string Player::toStringCharacterTab() const
{
	std::stringstream ss;
	const AttributeComponent* ac = this->attributeComponent;
	const Weapon* w = this->weapon;

	ss << "Level: " << ac->level << "\n"
		<< "Experience: " << ac->exp << "\n"
		<< "Next Level: " << ac->expNext << "\n"

		<< "Weapon Level: " << w->getLevel() << "\n"
		<< "Weapon Type: " << w->getType() << "\n"
		<< "Weapon Value: " << w->getValue() << "\n"
		<< "Weapon Range: " << w->getRange() << "\n"
		<< "Weapon DamageMin: " << w->getDamageMin() + this->attributeComponent->damageMin << " (" << this->attributeComponent->damageMin << ")" << "\n"
		<< "Weapon DamageMax: " << w->getDamageMax() + this->attributeComponent->damageMax << " (" << this->attributeComponent->damageMax << ")" << "\n";

	return ss.str();
}

const bool& Player::getInitAttack() const
{
	return this->initAttack;
}

const bool Player::getDamageTimer()
{
	if (this->damageTimer.getElapsedTime().asMilliseconds() >= damageTimerMax)
	{
		this->damageTimer.restart();
		return true;
	}

	return false;
}

const unsigned Player::getDamage() const
{
	return rand() % (
		(this->attributeComponent->damageMax + this->weapon->getDamageMax())
		- (this->attributeComponent->damageMin + this->weapon->getDamageMin()) + 1)
		+ (this->attributeComponent->damageMin + this->weapon->getDamageMin());
}

void Player::setInitAttack(const bool initAttack)
{
	this->initAttack = initAttack;
}

//========================================================
//Functions
//========================================================
void Player::loseHP(const int hp)
{
	this->attributeComponent->loseHP(hp);
}

void Player::gainHP(const int hp)
{
	this->attributeComponent->gainHP(hp);
}

void Player::loseEXP(const int exp)
{
	this->attributeComponent->gainExp(exp);
}

void Player::gainEXP(const int exp)
{
	this->attributeComponent->gainExp(exp);
}

void Player::updateAnimation(const float& dt)
{
	if (this->attacking)
	{

	}
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

void Player::update(const float& dt, sf::Vector2f& mouse_pos_view, const sf::View& view)
{
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	//	this->attributeComponent->gainExp(20);

	this->movementComponent->update(dt);

	this->updateAnimation(dt);		

	this->hitboxComponent->update();

	this->weapon->update(mouse_pos_view, sf::Vector2f(this->getSpriteCenter().x, this->getSpriteCenter().y + 5.f));
}

void Player::render(sf::RenderTarget& target, sf::Shader* shader, const sf::Vector2f light_position, const bool show_hitbox)
{
	if (shader)
	{
		shader->setUniform("hasTexture", true);
		shader->setUniform("lightPos", light_position);
		target.draw(this->sprite, shader);

		shader->setUniform("hasTexture", true);
		shader->setUniform("lightPos", light_position);
		this->weapon->render(target, shader);
	}
	else
	{
		target.draw(this->sprite);
		this->weapon->render(target);
	}

	if (show_hitbox)
		this->hitboxComponent->render(target);
}
