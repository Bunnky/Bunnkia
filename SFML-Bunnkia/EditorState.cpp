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
	if (!this->font.loadFromFile("Fonts/The Impostor.ttf"))
	{
		throw("ERROR::EDITORSTATE::COULD NOT LOAD FONT");
	}
}

//----------------------
//Initialize Keybinds
//----------------------
void EditorState::initKeybinds()
{
	std::ifstream ifs("Config/editorstate_keybinds.ini");

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

}

//----------------------
//Initialize TileMap
//----------------------
void EditorState::initTileMap()
{
	this->tileMap = new TileMap(this->stateData->gridSize, 100, 100, "Resources/Images/Tiles/tileSheet.png");
}

void EditorState::initModes()
{
	this->modes.push_back(new DefaultEditorMode(this->stateData, this->tileMap, &this->editorStateData));
}

//========================================================
//Constructors/Destructors
//========================================================
EditorState::EditorState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initEditorStateData();
	this->initView();
	this->initFonts();
	this->initKeybinds();
	this->initPauseMenu();
	this->initButtons();
	this->initTileMap();
	this->initGui();
	this->initModes();
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
		this->tileMap->saveToFile("test.slmp");
		this->paused = false;
	}		

	if (this->pmenu->isButtonPressed("LOAD"))
	{
		this->tileMap->loadFromFile("test.slmp");
		this->paused = false;
	}
		
}

void EditorState::updateModes(const float& dt)
{
	this->modes[EditorModes::DEFAULT_MODE]->update(dt);
}

void EditorState::update(const float& dt)
{
	this->updateMousePositions(&this->view);
	this->updateKeytime(dt);
	this->updateInput(dt);

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
	this->modes[EditorModes::DEFAULT_MODE]->render(target);
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

	this->renderModes(*target);

	if (this->paused) //Pause menu render
	{
		target->setView(this->window->getDefaultView());
		this->pmenu->render(*target);
	}

}

