#ifndef CHARACTERSYSTEM_H
#define CHARACTERSYSTEM_H

#include "Entity.h"
#include "Player.h"

enum CharacterTypes { ROGUE = 0, WARRIOR, RANGER, MAGE };

class CharacterSystem
{
private:
	std::map<std::string, sf::Texture>& textures;
	std::vector<Player*>& activeCharacter;
	Player& player;

public:
	CharacterSystem(std::vector<Player*>& activeCharacter,
		std::map<std::string, sf::Texture>& textures, Player& player);
	virtual ~CharacterSystem();

	//Accessors

	//Modifiers

	//Functions
	void createCharacter(const short character);

	void update(const float& dt);
	void render(sf::RenderTarget* target);
};

#endif //CHARACTERSYSTEM_H
