#include "stdafx.h"
#include "Weapon.h"

void Weapon::initVariables()
{

}

Weapon::Weapon(unsigned value, std::string texture_file)
	: Item(value)
{
	if (!this->weapon_texture.loadFromFile(texture_file))
		std::cout << "ERROR::PLAYER::COULD NOT LOAD WEAPON TEXTURE:: " << texture_file << "\n";

	this->weapon_sprite.setTexture(this->weapon_texture);
}

Weapon::~Weapon()
{

}
