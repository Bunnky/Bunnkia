#include "stdafx.h"
#include "MainMenuState.h"

//========================================================
//Initializer Functions
//========================================================
void MainMenuState::initVariables()
{
}

void MainMenuState::initFonts()
{
	if (!this->font.loadFromFile("gamedata/Fonts/The Impostor.ttf"))	
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");	
}

void MainMenuState::initKeybinds()
{
	std::ifstream ifs("gamedata/Config/mainmenustate_keybinds.ini");

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

void MainMenuState::initGui()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	//Background
	this->background.setSize(
		sf::Vector2f
		(
			static_cast<float>(vm.width),
			static_cast<float>(vm.height)
		)
	);

	if (!this->backgroundTexture.loadFromFile("gamedata/Resources/Images/Backgrounds/bg1.bmp"))	
		throw"ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";	

	this->background.setTexture(&this->backgroundTexture);

	//Menu Background
	this->btnBackground.setSize(
		sf::Vector2f(
			static_cast<float>(vm.width / 4),
			static_cast<float>(vm.height - gui::p2pY(50.f, vm))
		)
	);

	this->btnBackground.setPosition(gui::p2pX(38.1f, vm), gui::p2pY(25.f, vm));
	this->btnBackground.setFillColor(sf::Color(10, 10, 10, 150));

	//Buttons
	this->buttons["NEW_GAME"] = new gui::Button(
		gui::p2pX(41.2f, vm), gui::p2pY(28.3f, vm),
		gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm),
		&this->font, "New Game", gui::calcCharSize(vm),
		sf::Color(200, 200, 200, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

	this->buttons["SETTINGS_STATE"] = new gui::Button(
		gui::p2pX(41.2f, vm), gui::p2pY(40.8f, vm),
		gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm),
		&this->font, "Settings", gui::calcCharSize(vm),
		sf::Color(200, 200, 200, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

	this->buttons["EDITOR_STATE"] = new gui::Button(
		gui::p2pX(41.2f, vm), gui::p2pY(53.3f, vm),
		gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm),
		&this->font, "Editor", gui::calcCharSize(vm),
		sf::Color(200, 200, 200, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

	this->buttons["EXIT_STATE"] = new gui::Button(
		gui::p2pX(41.2f, vm), gui::p2pY(65.8f, vm),
		gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm),
		&this->font, "Quit", gui::calcCharSize(vm),
		sf::Color(200, 200, 200, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 100, 100, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));
}

void MainMenuState::resetGui()
{
	/*
	* Clears the GUI elemenats and re-initilizes the GUI
	*
	* @return void
	*
	*/
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
	this->buttons.clear();

	this->initGui();
}

//========================================================
//Constructors/Destructors
//========================================================
MainMenuState::MainMenuState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initFonts();
	this->initKeybinds();
	this->initGui();
	this->resetGui();
}

MainMenuState::~MainMenuState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

//========================================================
//Functions
//========================================================
void MainMenuState::updateInput(const float& dt)
{
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::G))
}

void MainMenuState::updateButtons()
{
	/*Updates all the buttons the the state and handles their functionality*/

	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosWindow);
	}

	//New game
	if (this->buttons["NEW_GAME"]->isPressed())
	{
		this->states->push(new CharacterSelectState(this->stateData));
		Sleep(50);
	}

	//Settings
	if (this->buttons["SETTINGS_STATE"]->isPressed())
	{
		this->states->push(new SettingsState(this->stateData));
		Sleep(50);
	}

	//Editor
	if (this->buttons["EDITOR_STATE"]->isPressed())
	{
		this->states->push(new EditorState(this->stateData));
		Sleep(50);
	}

	//Quit the game
	if (this->buttons["EXIT_STATE"]->isPressed())
	{
		this->endState();
	}
}

void MainMenuState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);

	this->updateButtons();
}

void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->draw(this->background);

	target->draw(this->btnBackground);

	this->renderButtons(*target);

	//DEBUG MOUSE POS
	//sf::Text mouseText;
	//mouseText.setPosition(this->mousePosView.x -20, this->mousePosView.y - 20);
	//mouseText.setFont(this->font);
	//mouseText.setOutlineColor(sf::Color::Black);
	//mouseText.setOutlineThickness(1);
	//mouseText.setCharacterSize(12);
	//std::stringstream ss;
	//ss << this->mousePosView.x << " " << this->mousePosView.y;
	//mouseText.setString(ss.str());
	//target->draw(mouseText);
}