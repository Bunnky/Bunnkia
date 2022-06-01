#ifndef CHARCTERSELECTSTATE_H
#define CHARCTERSELECTSTATE_H

#include "GameState.h"
#include "Gui.h"
#include "AnimationComponent.h"

class CharacterSelectState :
    public State
{
private:

	//===========//
	// Variables //
	//===========//
	sf::RectangleShape background;
	sf::Texture backgroundTexture;
	sf::Font font;

	sf::RectangleShape btnBackground;
	std::map<std::string, gui::Button*> buttons;

	sf::Text titletext;
	sf::Text charactertext;

	sf::Texture spritetexture;
	sf::Sprite sprite;


	//===========//
	// Functions //
	//===========//
	void initVariables();
	void initFonts();
	void initKeybinds();
	void initCharacters();

	void initGui();

public:
	CharacterSelectState(StateData* state_data);
	virtual ~CharacterSelectState();



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