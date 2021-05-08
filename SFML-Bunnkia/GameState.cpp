#include "stdafx.h"
#include "GameState.h"

//========================================================
// 
//Initializer Functions
// 
//========================================================
//----------------------
//Initialize Deferred Render
//----------------------
void GameState::initDeferredRender()
{
	this->renderTexture.create(
		this->stateData->gfxSettings->resolution.width,
		this->stateData->gfxSettings->resolution.height
	);

	this->renderSprite.setTexture(this->renderTexture.getTexture());
	this->renderSprite.setTextureRect(
		sf::IntRect(
			0, 
			0,
			this->stateData->gfxSettings->resolution.width,
			this->stateData->gfxSettings->resolution.height
		)
	);
}

//----------------------
//Initialize View
//----------------------
void GameState::initView()
{
	this->view.setSize(sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width),
			static_cast<float>(this->stateData->gfxSettings->resolution.height))
	);

	this->view.setCenter(sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width) / 2.f,
			static_cast<float>(this->stateData->gfxSettings->resolution.height) / 2.f)
	);
}

//----------------------
//Initialize Keybinds
//----------------------
void GameState::initKeybinds()
{
	std::ifstream ifs("Config/gamestate_keybinds.ini");

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
//Initialize Fonts
//----------------------
void GameState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/The Impostor.ttf"))
	{
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	}
}

//----------------------
//Initialize Textures
//----------------------
void GameState::initTextures()
{
	if (!this->textures["PLAYER_SHEET"].loadFromFile("Resources/Images/Sprites/Player/NEW.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_PLAYER_TEXTURE";
	}

	if (!this->textures["GOBLIN_SHEET"].loadFromFile("Resources/Images/Sprites/Enemy/goblin.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_GOBLIN_TEXTURE";
	}
}

//----------------------
//Initialize PauseMenu
//----------------------
void GameState::initPauseMenu()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;
	this->pmenu = new PauseMenu(this->stateData->gfxSettings->resolution, this->font);

	this->pmenu->addButton("QUIT", gui::p2pY(83.3f, vm), gui::p2pX(18.7f, vm), gui::p2pY(6.6f, vm), gui::calcCharSize(vm), "Quit");
}

//----------------------
//Initialize Shaders
//----------------------
void GameState::initShaders()
{
	if (!this->core_shader.loadFromFile("vertex_shader.vert", "fragment_shader.frag"))
	{
		std::cout << "ERROR::GAMESTATE::COULD NOT LOAD SHADER." << "\n";
	}
}

//----------------------
//Initialize Players
//----------------------
void GameState::initPlayers()
{
	//Player Start Position
	this->player = new Player(220, 220, this->textures["PLAYER_SHEET"]);
}

//----------------------
//Initialize PlayerGUI
//----------------------
void GameState::initPlayerGUI()
{
	this -> playerGUI = new PlayerGUI(this->player, this->stateData->gfxSettings->resolution);
}

//----------------------
//Initialize EnemySystem
//----------------------
void GameState::initEnemySystem()
{
	this->enemySystem = new EnemySystem(this->activeEnemies, this -> textures);
}

//----------------------
//Initialize TileMap
//----------------------
void GameState::initTileMap()
{
	//this->tileMap = new TileMap(this->stateData->gridSize, 100, 100, "Resources/Images/Tiles/tileSheet.png");
	//this->tileMap->loadFromFile("test.slmp");
	this->tileMap = new TileMap("test.slmp");
}

//========================================================
// 
//Constructors/Destructors
// 
//========================================================
GameState::GameState(StateData* state_data)
	: State(state_data)
{
	std::cout << green << "Initializing Deferred Render" << "\n" << white;
	this->initDeferredRender();
	std::cout << green << "Initializing View" << "\n" << white;
	this->initView();
	std::cout << green << "Initializing Keybinds" << "\n" << white;
	this->initKeybinds();
	std::cout << green << "Initializing Fonts" << "\n" << white;
	this->initFonts();
	std::cout << green << "Initializing Textures" << "\n" << white;
	this->initTextures();
	std::cout << green << "Initializing Pause Menu" << "\n" << white;
	this->initPauseMenu();
	std::cout << green << "Initializing Shaders" << "\n" << white;
	this->initShaders();

	std::cout << green << "Initializing Players" << "\n" << white;
	this->initPlayers();
	std::cout << green << "Initializing Player GUI" << "\n" << white;
	this->initPlayerGUI();
	std::cout << green << "Initializing Enemy System" << "\n" << white;
	this->initEnemySystem();
	std::cout << green << "Initializing Tile Map" << "\n" << white;
	this->initTileMap();

	std::cout << blue << "Initializing complete!" <<  "\n" << white;
}

GameState::~GameState()
{
	delete this->pmenu;
	delete this->player;
	delete this->playerGUI;
	delete this->enemySystem;
	delete this->tileMap;

	for (size_t i = 0; i < this->activeEnemies.size(); i++)
	{
		delete this->activeEnemies[i];
	}
}

//========================================================
// 
//Functions
// 
//========================================================
void GameState::updateView(const float& dt)
{
	this->view.setCenter(
		std::floor(this->player->getPosition().x + (static_cast<float>(this->mousePosWindow.x) - static_cast<float>(this->stateData->gfxSettings->resolution.width / 2)) / 10.f), 
		std::floor(this->player->getPosition().y + (static_cast<float>(this->mousePosWindow.y) - static_cast<float>(this->stateData->gfxSettings->resolution.height / 2)) / 10.f)
	);

	if (this->tileMap->getMaxSizeF().x >= this->view.getSize().x)
	{
		if (this->view.getCenter().x - this->view.getSize().x / 2.f < 0.f)
		{
			this->view.setCenter(0.f + this->view.getSize().x / 2.f, this->view.getCenter().y);
		}
		else if (this->view.getCenter().x + this->view.getSize().x / 2.f > this->tileMap->getMaxSizeF().x)
		{
			this->view.setCenter(this->tileMap->getMaxSizeF().x - this->view.getSize().x / 2.f, this->view.getCenter().y);
		}
	}

	if (this->tileMap->getMaxSizeF().y >= this->view.getSize().y)
	{
		if (this->view.getCenter().y - this->view.getSize().y / 2.f < 0.f)
		{
			this->view.setCenter(this->view.getCenter().x, 0.f + this->view.getSize().y / 2.f);
		}
		else if (this->view.getCenter().y + this->view.getSize().y / 2.f > this->tileMap->getMaxSizeF().y)
		{
			this->view.setCenter(this->view.getCenter().x, this->tileMap->getMaxSizeF().y - this->view.getSize().y / 2.f);
		}
	}


	this->viewGridPosition.x = static_cast<int>(this->view.getCenter().x) / static_cast<int>(this->stateData->gridSize);
	this->viewGridPosition.y = static_cast<int>(this->view.getCenter().y) / static_cast<int>(this->stateData->gridSize);
}

void GameState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeytime())
	{
		if (!this->paused)
			this->pausedState();
		else
			this->unpausedState();
	}
}

void GameState::updatePlayerInput(const float& dt)
{

	//Update player input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT"))))
		this->player->move(-1.f, 0.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT"))))
		this->player->move(1.f, 0.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP"))))
	{
		this->player->move(0.f, -1.f, dt);
		//if (this->getKeytime())
		//	this->player->gainEXP(10);
	}		
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN"))))
	{
		this->player->move(0.f, 1.f, dt);
		//if (this->getKeytime())
		//	this->player->loseEXP(10);
	}
			
}

void GameState::updatePlayerGUI(const float& dt)
{
	this->playerGUI->update(dt);
}

void GameState::updatePauseMenuButtons()
{
	if (this->pmenu->isButtonPressed("QUIT"))
		this->endState();
}

void GameState::updateTileMap(const float& dt)
{
	this->tileMap->updateWorldBoundsCollision(this->player, dt);
	this->tileMap->updateTileCollision(this->player, dt);
	this->tileMap->updateTiles(this->player, dt, *this->enemySystem);

	for (auto* i : this->activeEnemies)
	{
		this->tileMap->updateWorldBoundsCollision(i, dt);
		this->tileMap->updateTileCollision(i, dt);
	}
}

void GameState::updatePlayer(const float& dt)
{
}

void GameState::updateEnemies(const float& dt)
{
	//this->activeEnemies.push_back(new Goblin(200.f, 100.f, this->textures["GOBLIN_SHEET"]));
}

void GameState::update(const float& dt)
{
	this->updateMousePositions(&this->view);
	this->updateKeytime(dt);
	this->updateInput(dt);

	if (!this->paused) //Unpaused update
	{
		this->updateView(dt);

		this->updatePlayerInput(dt);

		this->updateTileMap(dt);

		this->player->update(dt, this->mousePosView);

		this->playerGUI->update(dt);

		for (auto* i : this->activeEnemies)
		{
			i->update(dt, this->mousePosView);
		}

	}
	else //Paused update
	{		
		this->pmenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}

}

void GameState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	this->renderTexture.clear();

	this->renderTexture.setView(this->view);

	this->tileMap->render
	(
		this->renderTexture, 
		this->viewGridPosition,
		&this->core_shader,
		this->player->getCenter(),
		false
	);

	//Render Enemies
	for (auto* i : this->activeEnemies)
	{
		i->render(this->renderTexture, &this->core_shader, this->player->getCenter(), false);
	}

	this->player->render(this->renderTexture, &this->core_shader, this->player->getCenter(), false);

	//Deferred Rendering
	this->tileMap->renderDeferred(this->renderTexture, &this->core_shader, this->player->getCenter());

	//Render GUI
	this->renderTexture.setView(this->renderTexture.getDefaultView());
	this->playerGUI->render(this->renderTexture);
	
	if (this->paused) //Pause menu render
	{
		//this->renderTexture.setView(this->renderTexture.getDefaultView());
		this->pmenu->render(this->renderTexture);
	}

	//FINAL RENDER
	this->renderTexture.display();
	this->renderSprite.setTexture(this->renderTexture.getTexture());
	target->draw(this->renderSprite);
}
