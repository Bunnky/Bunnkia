#ifndef CHARCTERSELECTSTATE_H
#define CHARCTERSELECTSTATE_H

#include "GameState.h"
#include "Gui.h"
#include "AnimationComponent.h"


enum CharacterTypes { ROGUE = 0, WARRIOR, RANGER, MAGE };

class CharacterSelectState :
    public State
{
private:

	//===========//
	// Variables //
	//===========//
	CharacterTypes character;

	sf::RectangleShape background;
	sf::Texture backgroundTexture;
	sf::Font font;

	sf::RectangleShape btnBackground;
	std::map<std::string, gui::Button*> buttons;

	sf::Text titleText;

	sf::Texture spritetexture;
	sf::Sprite sprite;


	//===========//
	// Functions //
	//===========//
	void initVariables();
	void initFonts();
	void initKeybinds();

	void initGui();

public:
	CharacterSelectState(StateData* state_data);
	virtual ~CharacterSelectState();



	//===========//
	// Accessors //
	//===========//
	const short unsigned& getCharacter() const { return this->character; }

	//===========//
	// Functions //
	//===========//
	void updateInput(const float& dt);
	void updateGui(const float& dt);
	void update(const float& dt);
	void renderGui(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = NULL);


};


#endif //CHARCTERSELECTSTATE_H