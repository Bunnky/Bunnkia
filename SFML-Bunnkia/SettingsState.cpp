#include "stdafx.h"
#include "SettingsState.h"
#include "GraphicsSettings.h"

//========================================================
//Initializer Functions
//========================================================
void SettingsState::initVariables()
{
	this->modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::initFonts()
{
	if (!this->font.loadFromFile("gamedata/Fonts/The Impostor.ttf"))	
		throw("ERROR::SettingsState::COULD NOT LOAD FONT");	
}

void SettingsState::initKeybinds()
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

void SettingsState::initGui()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	//Window Background
	this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	if (!this->backgroundTexture.loadFromFile("gamedata/Resources/Images/Backgrounds/bg1.bmp"))	
		throw"ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
	
	this->background.setTexture(&this->backgroundTexture);

	//Button Background
	this->btnBackground.setSize(sf::Vector2f(static_cast<float>(vm.width / 4), static_cast<float>(vm.height - gui::p2pY(50.f, vm))));
	this->btnBackground.setPosition(gui::p2pX(38.1f, vm), gui::p2pY(25.f, vm));
	this->btnBackground.setFillColor(sf::Color(10, 10, 10, 150));

	//Basic Buttons
	this->buttons["BACK"] = new gui::Button(
		gui::p2pX(34.3f, vm), gui::p2pY(82.5f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(6.6f, vm),
		&this->font, "Back", gui::calcCharSize(vm),
		sf::Color(200, 200, 200, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 100, 100, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

	this->buttons["SAVE"] = new gui::Button(
		gui::p2pX(53.7f, vm), gui::p2pY(82.5f, vm),
		gui::p2pX(12.f, vm), gui::p2pY(6.6f, vm),
		&this->font, "Save", gui::calcCharSize(vm),
		sf::Color(200, 200, 200, 200), sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 100, 100, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

	////////////////////
	//Settings Buttons
	////////////////////

	// FullScreen
	if (!this->stateData->gfxSettings->fullscreen)
	{
		this->buttons["FULLSCREEN"] = new gui::Button(
			gui::p2pX(41.2f, vm), gui::p2pY(28.3f, vm),
			gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm),
			&this->font, "Fullscreen", gui::calcCharSize(vm, 60),
			sf::Color(200, 0, 0, 200), sf::Color(250, 0, 0, 250), sf::Color(20, 0, 0, 50),
			sf::Color(100, 0, 0, 0), sf::Color(150, 0, 0, 0), sf::Color(20, 0, 0, 0));
	}
	else {
		this->buttons["FULLSCREEN"] = new gui::Button(
			gui::p2pX(41.2f, vm), gui::p2pY(28.3f, vm),
			gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm),
			&this->font, "Fullscreen", gui::calcCharSize(vm, 60),
			sf::Color(0, 200, 0, 200), sf::Color(0, 250, 0, 250), sf::Color(0, 20, 0, 50),
			sf::Color(0, 100, 0, 0), sf::Color(0, 150, 0, 0), sf::Color(0, 20, 0, 0));
	}

	// Anti-Aliasing
	if (!this->stateData->gfxSettings->contextSettings.antialiasingLevel)
	{
		this->buttons["AA"] = new gui::Button(
			gui::p2pX(41.2f, vm), gui::p2pY(38.3f, vm),
			gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm),
			&this->font, "Anti alias", gui::calcCharSize(vm, 63),
			sf::Color(200, 0, 0, 200), sf::Color(250, 0, 0, 250), sf::Color(20, 0, 0, 50),
			sf::Color(100, 0, 0, 0), sf::Color(150, 0, 0, 0), sf::Color(20, 0, 0, 0));
	}
	else {
		this->buttons["AA"] = new gui::Button(
			gui::p2pX(41.2f, vm), gui::p2pY(38.3f, vm),
			gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm),
			&this->font, "Anti alias", gui::calcCharSize(vm, 63),
			sf::Color(0, 200, 0, 200), sf::Color(0, 250, 0, 250), sf::Color(0, 20, 0, 50),
			sf::Color(0, 100, 0, 0), sf::Color(0, 150, 0, 0), sf::Color(0, 20, 0, 0));
	}

	// V_Sync
	if (!this->stateData->gfxSettings->verticalSync)
	{
		this->buttons["VSYNC"] = new gui::Button(
			gui::p2pX(41.2f, vm), gui::p2pY(48.3f, vm),
			gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm),
			&this->font, "VSync", gui::calcCharSize(vm, 60),
			sf::Color(200, 0, 0, 200), sf::Color(250, 0, 0, 250), sf::Color(20, 0, 0, 50),
			sf::Color(100, 0, 0, 0), sf::Color(150, 0, 0, 0), sf::Color(20, 0, 0, 0));
	}
	else {
		this->buttons["VSYNC"] = new gui::Button(
			gui::p2pX(41.2f, vm), gui::p2pY(48.3f, vm),
			gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm),
			&this->font, "VSync", gui::calcCharSize(vm, 60),
			sf::Color(0, 200, 0, 200), sf::Color(0, 250, 0, 250), sf::Color(0, 20, 0, 50),
			sf::Color(0, 100, 0, 0), sf::Color(0, 150, 0, 0), sf::Color(0, 20, 0, 0));
	}

	//Old Dropdown list for resolutions.Maybe let players do this later ?
	/*
	//Modes
	std::vector<std::string> modes_str;
	for (auto& i : this->modes)
	{
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}

	//Dropdown Lists
	this->dropDownLists["RESOLUTION"] = new gui::DropDownList(
		gui::p2pX(37.5f, vm), gui::p2pY(31.7f, vm),
		gui::p2pX(20.f, vm), gui::p2pY(5.3f, vm),
		font, modes_str.data(), modes_str.size()
	);

	//Text init
	this->optionsText.setFont(this->font);
	this->optionsText.setPosition(sf::Vector2f(gui::p2pX(30.f, vm), gui::p2pY(30.f, vm)));
	this->optionsText.setCharacterSize(gui::calcCharSize(vm, 60));
	this->optionsText.setFillColor(sf::Color(255, 255, 255, 200));

	this->optionsText.setString("Fullscreen \n\nVsync \n\nAntialiasing \n ");
	*/
}

// Clears the GUI elemenats and re-initilizes the GUI
void SettingsState::resetGui()
{
	auto it = this->buttons.begin();

	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
	this->buttons.clear();

	auto it2 = this->dropDownLists.begin();
	for (it2 = this->dropDownLists.begin(); it2 != this->dropDownLists.end(); ++it2)
	{
		delete it2->second;
	}
	this->dropDownLists.clear();

	this->initGui();
}

//========================================================
//Constructors/Destructors
//========================================================
SettingsState::SettingsState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initFonts();
	this->initKeybinds();
	this->initGui();
}

SettingsState::~SettingsState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}

	auto it2 = this->dropDownLists.begin();
	for (it2 = this->dropDownLists.begin(); it2 != this->dropDownLists.end(); ++it2)
	{
		delete it2->second;
	}
}

//========================================================
//Accessors
//========================================================

//========================================================
//Functions
//========================================================
void SettingsState::updateInput(const float& dt)
{
	//if(sf::Keyboard::isKeyPressed(sf::Keyboard::G))
}

void SettingsState::updateGui(const float& dt)
{
	//Buttons
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosWindow);
	}

	//Back to menu
	if (this->buttons["BACK"]->isPressed())
	{
		this->endState();
	}

	//Apply selected settings
	if (this->buttons["SAVE"]->isPressed())
	{
		this->stateData->gfxSettings->saveToFile("gamedata/Config/graphics.ini");
		std::cout << green << "Saved!" << "\n";
		Sleep(100);
		//TEST REMOVE LATER!
		//this->stateData->gfxSettings->resolution = this->modes[this->dropDownLists["RESOLUTION"]->getActiveElementId()];
		//this->window->create(this->stateData->gfxSettings->resolution, this->stateData->gfxSettings->title, sf::Style::Default);
	}

	//Dropdown lists
	//for (auto& it : this->dropDownLists)
	//{
	//	it.second->update(this->mousePosWindow, dt);
	//}

	//Fullscreen Toggle
	if (this->buttons["FULLSCREEN"]->isPressed())
	{
		if (!this->stateData->gfxSettings->fullscreen)
		{
			this->window->create(this->stateData->gfxSettings->resolution, this->stateData->gfxSettings->title, sf::Style::Fullscreen);
			this->stateData->gfxSettings->fullscreen = true;
		}
		else if (this->stateData->gfxSettings->fullscreen)
		{
			this->window->create(this->stateData->gfxSettings->resolution, this->stateData->gfxSettings->title, sf::Style::Default);
			this->stateData->gfxSettings->fullscreen = false;
		}
		this->resetGui();
		Sleep(100);
	}

	//Antialiasing Toggle
	if (this->buttons["AA"]->isPressed())
	{
		if (!this->stateData->gfxSettings->contextSettings.antialiasingLevel)
		{
			this->stateData->gfxSettings->contextSettings.antialiasingLevel = 1;
			std::cout << "AA On!" << "\n";
		}
		else
		{
			this->stateData->gfxSettings->contextSettings.antialiasingLevel = 0;
			std::cout << "AA Off!" << "\n";
		}
		this->resetGui();
		Sleep(100);
	}

	//VSync Toggle
	if (this->buttons["VSYNC"]->isPressed())
	{
		if (!this->stateData->gfxSettings->verticalSync)
		{
			this->stateData->gfxSettings->verticalSync = 1;
			std::cout << "VSync On!" << "\n";
		}
		else
		{
			this->stateData->gfxSettings->verticalSync = 0;
			std::cout << "VSync Off!" << "\n";
		}
		this->resetGui();
		Sleep(100);
	}
}

void SettingsState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);
	this->updateGui(dt);
}

void SettingsState::renderGui(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}

	//for (auto& it : this->dropDownLists)
	//{
	//	it.second->render(target);
	//}
}

void SettingsState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->draw(this->background);
	target->draw(this->btnBackground);
	this->renderGui(*target);
	target->draw(this->optionsText);

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