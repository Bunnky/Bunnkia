#include "stdafx.h"
#include "CharacterSystem.h"

CharacterSystem::CharacterSystem(std::vector<Player*>& activeCharacter,
	std::map<std::string, sf::Texture>& textures, Player& player)
	: textures(textures), activeCharacter(activeCharacter), player(player)
{
	this->textures = textures;
	this->activeCharacter = activeCharacter;
}

CharacterSystem::~CharacterSystem()
{
}

void CharacterSystem::createCharacter(const short character)
{
	switch (character)
	{
	case CharacterTypes::ROGUE:
		this->activeCharacter.push_back(new Player(150, 200, this->textures["ROGUE_SHEET"]));
		break;
	case CharacterTypes::WARRIOR:
		this->activeCharacter.push_back(new Player(150, 200, this->textures["ROGUE_SHEET"]));
		break;
	case CharacterTypes::RANGER:
		this->activeCharacter.push_back(new Player(150, 200, this->textures["ROGUE_SHEET"]));
		break;
	case CharacterTypes::MAGE:
		this->activeCharacter.push_back(new Player(150, 200, this->textures["ROGUE_SHEET"]));
		break;
	default:
		std::cout << "ERROR::CharacterSystem::createCharacter::TYPE DOES NOT EXIST" << "\n";
		break;
	}
}

void CharacterSystem::update(const float& dt)
{
}

void CharacterSystem::render(sf::RenderTarget* target)
{
}