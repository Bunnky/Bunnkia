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

	this->animationComponent->addAnimation("IDLE", 15.f, 0, 0, 3, 0, 32, 32);
	this->animationComponent->addAnimation("WALK_LEFT", 6.f, 0, 1, 3, 1, 32, 32);
	this->animationComponent->addAnimation("WALK_RIGHT", 6.f, 4, 1, 7, 1, 32, 32);
	this->animationComponent->addAnimation("WALK_DOWN", 6.f, 0, 1, 3, 1, 32, 32);
	this->animationComponent->addAnimation("WALK_UP", 6.f, 4, 1, 7, 1, 32, 32);
	this->animationComponent->addAnimation("ATTACK", 5.f, 0, 2, 4, 2, 32 * 2, 32);

	//Visual Weapon
	if (!this->weapon_texture.loadFromFile("Resources/Images/Sprites/Player/sword.png"))
		std::cout << "ERROR::PLAYER::COULD NOT LOAD WEAPON TEXTURE." << "\n";

	this->weapon_sprite.setTexture(this->weapon_texture);

	this->weapon_sprite.setOrigin(
		this->weapon_sprite.getGlobalBounds().width / 2.f,
		this->weapon_sprite.getGlobalBounds().height
	);

	//========================================================
	//CHANGE THESE IF WE WANT TO DO DIRECTIONAL PLAYER SPRITES
	//========================================================
	//this->animationComponent->addAnimation("WALK_DOWN", 6.f, 0, 1, 3, 1, 32, 32);
	//this->animationComponent->addAnimation("WALK_LEFT", 6.f, 0, 1, 3, 1, 32, 32);
	//this->animationComponent->addAnimation("WALK_RIGHT", 6.f, 0, 1, 3, 1, 32, 32);
	//this->animationComponent->addAnimation("WALK_UP", 6.f, 0, 1, 3, 1, 32, 32);
	
}

Player::~Player()
{

}
//========================================================
//Accessors
//========================================================
AttributeComponent* Player::getAttributeComponent()
{
	return this->attributeComponent;
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

void Player::updateAttack()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//this->attacking = true;
	}
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

void Player::update(const float& dt, sf::Vector2f& mouse_pos_view)
{
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	//	this->attributeComponent->gainExp(20);

	this->movementComponent->update(dt);

	this->updateAttack();

	this->updateAnimation(dt);		

	this->hitboxComponent->update();

	//Update visual weapon
	this->weapon_sprite.setPosition(this->getCenter().x, this->getPosition().y + this->getGlobalBounds().height);

	float dX = mouse_pos_view.x - this->weapon_sprite.getPosition().x;
	float dY = mouse_pos_view.y - this->weapon_sprite.getPosition().y;

	const float PI = 3.14159265;
	float deg = atan2(dY, dX) * 180 / PI;

	this->weapon_sprite.setRotation(deg + 90.f);
}

void Player::render(sf::RenderTarget& target, sf::Shader* shader, const bool show_hitbox)
{
	if (shader)
	{
		shader->setUniform("hasTexture", true);
		shader->setUniform("lightPos", this->getCenter());
		target.draw(this->sprite, shader);

		shader->setUniform("hasTexture", true);
		shader->setUniform("lightPos", this->getCenter());
		target.draw(this->weapon_sprite, shader);
	}
	else
	{
		target.draw(this->sprite);
		target.draw(this->weapon_sprite);
	}
		


	if (show_hitbox)
		this->hitboxComponent->render(target);
}
