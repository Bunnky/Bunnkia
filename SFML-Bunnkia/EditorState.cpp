#include "stdafx.h"
#include "EditorState.h"

//========================================================
//Initializer Functions
//========================================================
//----------------------
//Initialize Variables
//----------------------
void EditorState::initVariables()
{
	this->cameraSpeed = 1000.f;
}

void EditorState::initEditorStateData()
{
	this->editorStateData.view = &this->view;
	this->editorStateData.font = &this->font;
	this->editorStateData.keytime = &this->keytime;
	this->editorStateData.keytimeMax = &this->keytimeMax;
	this->editorStateData.keybinds = &this->keybinds;
	this->editorStateData.mousePosGrid = &this->mousePosGrid;
	this->editorStateData.mousePosScreen = &this->mousePosScreen;
	this->editorStateData.mousePosView = &this->mousePosView;
	this->editorStateData.mousePosWindow = &this->mousePosWindow;
}

//----------------------
//Initialize View
//----------------------
void EditorState::initView()
{
	this->view.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width),
			static_cast<float>(this->stateData->gfxSettings->resolution.height)
		)
	);

	this->view.setCenter(
		static_cast<float>(this->stateData->gfxSettings->resolution.width) / 2.f,
		static_cast<float>(this->stateData->gfxSettings->resolution.height) / 2.f
	);
}

//----------------------
//Initialize Fonts
//----------------------
void EditorState::initFonts()
{
	if (!this->font.loadFromFile("gamedata/Fonts/The Impostor.ttf"))
	{
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	}
}

//----------------------
//Initialize Keybinds
//----------------------
void EditorState::initKeybinds()
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

//----------------------
//Initialize PauseMenu
//----------------------
void EditorState::initPauseMenu()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;

	this->pmenu = new PauseMenu(this->stateData->gfxSettings->resolution, this->font);

	this->pmenu->addButton("SAVE", gui::p2pY(50.f, vm), gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm), gui::calcCharSize(vm), "Save");
	this->pmenu->addButton("LOAD", gui::p2pY(66.7f, vm), gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm), gui::calcCharSize(vm), "Load");
	this->pmenu->addButton("QUIT", gui::p2pY(83.3f, vm), gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm), gui::calcCharSize(vm), "Quit");
}

//----------------------
//Initialize Buttons
//----------------------
void EditorState::initButtons()
{
}

//----------------------
//Initialize GUI
//----------------------
void EditorState::initGui()
{
	//Automatically Load Map
	//this->tileMap->loadFromFile("gamedata/level.slmp");
}

//----------------------
//Initialize TileMap
//----------------------
void EditorState::initTileMap()
{
	this->tileMap = new TileMap(this->stateData->gridSize, 100, 100, "gamedata/Resources/Images/Tiles/tileSheet.png");
}

//----------------------
//Initialize Modes
//----------------------
void EditorState::initModes()
{
	this->modes.push_back(new DefaultEditorMode(this->stateData, this->tileMap, &this->editorStateData));
	this->modes.push_back(new EnemyEditorMode(this->stateData, this->tileMap, &this->editorStateData));

	this->activeMode = EditorModes::DEFAULT_EDITOR_MODE;
}

void EditorState::initHelpText()
{
	this->helpText.setFont(this->font);
	this->helpText.setFillColor(sf::Color(255, 255, 255, 255));
	this->helpText.setCharacterSize(9);
	this->helpText.setPosition(this->window->getSize().x - 180.f, this->window->getSize().y / 32.f);
}

//========================================================
//Constructors/Destructors
//========================================================
EditorState::EditorState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	std::cout << yellow << "Initialized Variables" << "\n" << white;
	this->initEditorStateData();
	std::cout << yellow << "Initialized Editor State Data" << "\n" << white;
	this->initView();
	std::cout << yellow << "Initialized View" << "\n" << white;
	this->initFonts();
	std::cout << yellow << "Initialized Fonts" << "\n" << white;
	this->initKeybinds();
	std::cout << yellow << "Initialized Keybinds" << "\n" << white;
	this->initHelpText();
	std::cout << yellow << "Initialized Help Text" << "\n" << white;
	this->initPauseMenu();
	std::cout << yellow << "Initialized Pause Menu" << "\n" << white;
	this->initButtons();
	std::cout << yellow << "Initialized Buttons" << "\n" << white;
	this->initTileMap();
	std::cout << yellow << "Initialized Tilemap" << "\n" << white;
	this->initGui();
	std::cout << yellow << "Initialized Gui" << "\n" << white;
	this->initModes();
	std::cout << yellow << "Initialized Modes" << "\n" << white;
}

EditorState::~EditorState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}

	delete this->pmenu;
	delete this->tileMap;

	for (size_t i = 0; i < this->modes.size(); i++)
	{
		delete this->modes[i];
	}
}

//========================================================
//Functions
//========================================================
void EditorState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeytime())
	{
		if (!this->paused)
			this->pausedState();
		else
			this->unpausedState();
	}
}

void EditorState::updateEditorInput(const float& dt)
{
	//Move view
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_UP"))))
	{
		this->view.move(0.f, -this->cameraSpeed * dt);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_DOWN"))))
	{
		this->view.move(0.f, this->cameraSpeed * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_LEFT"))))
	{
		this->view.move(-this->cameraSpeed * dt, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_RIGHT"))))
	{
		this->view.move(this->cameraSpeed * dt, 0.f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MODE_UP"))) && this->getKeytime())
	{
		if (this->activeMode < this->modes.size() - 1)
		{
			this->activeMode++;
		}
		else
		{
			std::cout << "ERROR::EDITORSTATE::CANNOT CHANGE MODE UP!" << "\n";
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MODE_DOWN"))) && this->getKeytime())
	{
		if (this->activeMode > 0)
		{
			this->activeMode--;
		}
		else
		{
			std::cout << "ERROR::EDITORSTATE::CANNOT CHANGE MODE DOWN!" << "\n";
		}
	}
}

void EditorState::updateButtons()
{
	/*Updates all the buttons the the state and handles their functionality*/

	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosWindow);
	}
}

void EditorState::updateGui(const float& dt)
{
}

void EditorState::updatePauseMenuButtons()
{
	if (this->pmenu->isButtonPressed("QUIT"))
		this->endState();

	if (this->pmenu->isButtonPressed("SAVE"))
	{
		this->tileMap->saveToFile("gamedata/level.slmp");
		this->paused = false;
	}

	if (this->pmenu->isButtonPressed("LOAD"))
	{
		this->tileMap->loadFromFile("gamedata/level.slmp");
		this->paused = false;
	}
}

void EditorState::updateModes(const float& dt)
{
	this->modes[this->activeMode]->update(dt);
}

void EditorState::updateHelpText(const float& dt)
{
	std::stringstream ss;

	ss
		<< "Switch Mode: Page Up+Dwn" << "\n"
		<< "Lock Tile Placing: L" << "\n"
		<< "Test Line 3: " << "\n"
		<< "Test Line 4: " << "\n";

	this->helpText.setString(ss.str());
}

void EditorState::update(const float& dt)
{
	this->updateMousePositions(&this->view);
	this->updateKeytime(dt);
	this->updateInput(dt);

	this->updateHelpText(dt);

	if (!this->paused) //Unpaused
	{
		this->updateButtons();
		this->updateGui(dt);
		this->updateEditorInput(dt);
		this->updateModes(dt);
	}
	else //Paused
	{
		this->pmenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
}

//========================================================
//Rendering
//========================================================

void EditorState::renderButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void EditorState::renderGui(sf::RenderTarget& target)
{
}

void EditorState::renderModes(sf::RenderTarget& target)
{
	this->modes[this->activeMode]->render(target);
}

void EditorState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->setView(this->view);
	this->tileMap->render(*target, this->mousePosGrid, NULL, sf::Vector2f(), true);
	this->tileMap->renderDeferred(*target);
	target->setView(this->window->getDefaultView());
	this->renderButtons(*target);
	this->renderGui(*target);
	target->draw(this->helpText);
	this->renderModes(*target);

	if (this->paused) //Pause menu render
	{
		target->setView(this->window->getDefaultView());
		this->pmenu->render(*target);
	}
}