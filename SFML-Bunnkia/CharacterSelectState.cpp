#include "stdafx.h"
#include "CharacterSelectState.h"


//========================================================
//Initializer Functions
//========================================================
void CharacterSelectState::initVariables()
{
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
	this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	if (!this->backgroundTexture.loadFromFile("gamedata/Resources/Images/Backgrounds/bg1.bmp"))	
		throw"ERROR::CHARACTER_SELECT_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";

	this->background.setTexture(&this->backgroundTexture);

	//Basic Buttons
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
		this->states->push(new GameState(this->stateData));
		Sleep(50);
	}
}

void CharacterSelectState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);
	this->updateGui(dt);
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
