#include "stdafx.h"
#include "CharacterSelectState.h"


//========================================================
//Initializer Functions
//========================================================
void CharacterSelectState::initVariables()
{
	this->character = ROGUE;
}

void CharacterSelectState::initFonts()
{
	if (!this->font.loadFromFile("gamedata/Fonts/The Impostor.ttf"))	
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");	
}

void CharacterSelectState::initKeybinds()
{
	std::ifstream ifs("gamedata/Config/editorstate_keybinds.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2)
		{
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}

	ifs.close();
}

void CharacterSelectState::initGui()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;
	/// 
	/// Title text
	/// 
	this->titleText.setFont(this->font);
	this->titleText.setPosition(sf::Vector2f(gui::p2pX(30.f, vm), gui::p2pY(4.f, vm)));
	this->titleText.setCharacterSize(gui::calcCharSize(vm, 60));
	this->titleText.setFillColor(sf::Color(255, 255, 255, 250));
	this->titleText.setOutlineColor(sf::Color(0, 0, 0, 250));
	this->titleText.setOutlineThickness(1);
	this->titleText.setString("Choose a character");


	///
	/// Background
	///

	this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));

	if (!this->backgroundTexture.loadFromFile("gamedata/Resources/Images/Backgrounds/bg1.bmp"))	
		throw"ERROR::CHARACTER_SELECT_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";

	this->background.setTexture(&this->backgroundTexture);

	this->btnBackground.setSize(
		sf::Vector2f(
			static_cast<float>(vm.width - gui::p2pX(20.f, vm)),
			static_cast<float>(vm.height - gui::p2pY(35.f, vm))
		)
	);

	this->btnBackground.setPosition(gui::p2pX(10.f, vm), gui::p2pY(12.f, vm));
	this->btnBackground.setFillColor(sf::Color(10, 10, 10, 200));
	this->btnBackground.setOutlineColor(sf::Color(255, 255, 255, 150));
	this->btnBackground.setOutlineThickness(2);

	/// 
	/// Character Buttons
	/// 

	// TOP ROW
	this->buttons["ROGUE"] = new gui::Button(
		gui::p2pX(15.f, vm), gui::p2pY(19.f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(12.f, vm),
		"gamedata/Resources/Images/Sprites/Player/rogueSheet.png",
		&this->font, "Rogue", gui::calcCharSize(vm, 126),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 255, 20), sf::Color(0, 0, 255, 100), sf::Color(0, 0, 255, 200),
		sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50));


	this->buttons["WARRIOR"] = new gui::Button(
		gui::p2pX(34.f, vm), gui::p2pY(19.f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(12.f, vm),
		"gamedata/Resources/Images/Sprites/Player/warriorSheet.png",
		&this->font, "Warrior", gui::calcCharSize(vm, 126),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 255, 20), sf::Color(0, 0, 255, 100), sf::Color(0, 0, 255, 200),
		sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50));

	this->buttons["RANGER"] = new gui::Button(
		gui::p2pX(53.f, vm), gui::p2pY(19.f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(12.f, vm),
		"gamedata/Resources/Images/Sprites/Player/rangerSheet.png",
		&this->font, "Ranger", gui::calcCharSize(vm, 126),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 255, 20), sf::Color(0, 0, 255, 100), sf::Color(0, 0, 255, 200),
		sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50));

	this->buttons["PLACEHOLDER_THREE"] = new gui::Button(
		gui::p2pX(72.f, vm), gui::p2pY(19.f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(12.f, vm),
		"gamedata/Resources/Images/Sprites/Player/player_temp.png",
		&this->font, "PLACEHOLDER", gui::calcCharSize(vm, 126),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 255, 20), sf::Color(0, 0, 255, 100), sf::Color(0, 0, 255, 200),
		sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50));

	// MIDDLE ROW
	this->buttons["PLACEHOLDER_FOUR"] = new gui::Button(
		gui::p2pX(15.f, vm), gui::p2pY(38.f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(12.f, vm),
		"gamedata/Resources/Images/Sprites/Player/player_temp.png",
		&this->font, "PLACEHOLDER", gui::calcCharSize(vm, 126),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 255, 20), sf::Color(0, 0, 255, 100), sf::Color(0, 0, 255, 200),
		sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50));

	this->buttons["PLACEHOLDER_FIVE"] = new gui::Button(
		gui::p2pX(34.f, vm), gui::p2pY(38.f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(12.f, vm),
		"gamedata/Resources/Images/Sprites/Player/player_temp.png",
		&this->font, "PLACEHOLDER", gui::calcCharSize(vm, 126),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 255, 20), sf::Color(0, 0, 255, 100), sf::Color(0, 0, 255, 200),
		sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50));

	this->buttons["PLACEHOLDER_SIX"] = new gui::Button(
		gui::p2pX(53.f, vm), gui::p2pY(38.f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(12.f, vm),
		"gamedata/Resources/Images/Sprites/Player/player_temp.png",
		&this->font, "PLACEHOLDER", gui::calcCharSize(vm, 126),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 255, 20), sf::Color(0, 0, 255, 100), sf::Color(0, 0, 255, 200),
		sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50));

	this->buttons["PLACEHOLDER_SEVEN"] = new gui::Button(
		gui::p2pX(72.f, vm), gui::p2pY(38.f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(12.f, vm),
		"gamedata/Resources/Images/Sprites/Player/player_temp.png",
		&this->font, "PLACEHOLDER", gui::calcCharSize(vm, 126),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 255, 20), sf::Color(0, 0, 255, 100), sf::Color(0, 0, 255, 200),
		sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50));

	// BOTTOM ROW
	this->buttons["PLACEHOLDER_EIGHT"] = new gui::Button(
		gui::p2pX(15.f, vm), gui::p2pY(57.f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(12.f, vm),
		"gamedata/Resources/Images/Sprites/Player/player_temp.png",
		&this->font, "PLACEHOLDER", gui::calcCharSize(vm, 126),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 255, 20), sf::Color(0, 0, 255, 100), sf::Color(0, 0, 255, 200),
		sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50));

	this->buttons["PLACEHOLDER_NINE"] = new gui::Button(
		gui::p2pX(34.f, vm), gui::p2pY(57.f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(12.f, vm),
		"gamedata/Resources/Images/Sprites/Player/player_temp.png",
		&this->font, "PLACEHOLDER", gui::calcCharSize(vm, 126),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 255, 20), sf::Color(0, 0, 255, 100), sf::Color(0, 0, 255, 200),
		sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50));

	this->buttons["PLACEHOLDER_TEN"] = new gui::Button(
		gui::p2pX(53.f, vm), gui::p2pY(57.f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(12.f, vm),
		"gamedata/Resources/Images/Sprites/Player/player_temp.png",
		&this->font, "PLACEHOLDER", gui::calcCharSize(vm, 126),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 255, 20), sf::Color(0, 0, 255, 100), sf::Color(0, 0, 255, 200),
		sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50));

	this->buttons["PLACEHOLDER_ELEVEN"] = new gui::Button(
		gui::p2pX(72.f, vm), gui::p2pY(57.f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(12.f, vm),
		"gamedata/Resources/Images/Sprites/Player/player_temp.png",
		&this->font, "PLACEHOLDER", gui::calcCharSize(vm, 126),
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 255, 20), sf::Color(0, 0, 255, 100), sf::Color(0, 0, 255, 200),
		sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50));



	/// 
	/// Basic Buttons
	/// 
	this->buttons["BACK"] = new gui::Button(
		gui::p2pX(34.3f, vm), gui::p2pY(82.5f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(6.6f, vm),
		&this->font, "Back", gui::calcCharSize(vm),
		sf::Color(200, 200, 200, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 100, 100, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));


	this->buttons["START"] = new gui::Button(
		gui::p2pX(53.7f, vm), gui::p2pY(82.5f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(6.6f, vm),
		&this->font, "Start!", gui::calcCharSize(vm),
		sf::Color(200, 200, 200, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 100, 100, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));


}




CharacterSelectState::CharacterSelectState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initFonts();
	this->initKeybinds();
	this->initGui();

}

CharacterSelectState::~CharacterSelectState()
{

	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

void CharacterSelectState::updateInput(const float& dt)
{

}

void CharacterSelectState::updateGui(const float& dt)
{
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosWindow);
	}


	//Back to menu
	if (this->buttons["BACK"]->isPressed())	
		this->endState();
	
	// Start Game
	if (this->buttons["START"]->isPressed())
	{
		this->states->pop();
		this->states->push(new GameState(this->stateData));
	}

	if (this->buttons["ROGUE"]->isPressed())
	{
			this->character = ROGUE;
			std::cout << getCharacter() << "\n";
	}

	if (this->buttons["WARRIOR"]->isPressed())
	{
			this->character = WARRIOR;
			std::cout << getCharacter() << "\n";
	}

}

void CharacterSelectState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);
	this->updateGui(dt);
	this->getCharacter();

}

void CharacterSelectState::renderGui(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void CharacterSelectState::render(sf::RenderTarget* target)
{

	if (!target)
		target = this->window;

	target->draw(this->background);
	target->draw(this->btnBackground);
	target->draw(this->titleText);
	this->renderGui(*target);



	//DEBUG MOUSE POS
	sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x - 20, this->mousePosView.y - 20);
	mouseText.setFont(this->font);
	mouseText.setOutlineColor(sf::Color::Black);
	mouseText.setOutlineThickness(1);
	mouseText.setCharacterSize(12);
	std::stringstream ss;
	ss << this->mousePosView.x << " " << this->mousePosView.y;
	mouseText.setString(ss.str());
	target->draw(mouseText);
}
