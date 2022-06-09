#include "stdafx.h"
#include "GameState.h"
#include "MainMenuState.h"


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
	this->renderTexture.create(this->stateData->gfxSettings->resolution.width, this->stateData->gfxSettings->resolution.height);

	this->renderSprite.setTexture(this->renderTexture.getTexture());
	this->renderSprite.setTextureRect(sf::IntRect(0, 0,	this->stateData->gfxSettings->resolution.width,	this->stateData->gfxSettings->resolution.height));
}

//----------------------
//Initialize View
//----------------------
void GameState::initView()
{
	//View Zoom
	this->view.setSize(sf::Vector2f(
		static_cast<float>(this->stateData->gfxSettings->resolution.width / 2),
		static_cast<float>(this->stateData->gfxSettings->resolution.height / 2))
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
	std::ifstream ifs("gamedata/Config/gamestate_keybinds.ini");

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
	if (!this->font.loadFromFile("gamedata/Fonts/The Impostor.ttf"))
	{
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
	}
}

//----------------------
//Initialize Textures
//----------------------
void GameState::initTextures()
{
	//Player Textures

	if (!this->textures["ROGUE_SHEET"].loadFromFile("gamedata/Resources/Images/Sprites/Player/rogueSheet.png"))	
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_ROGUE_TEXTURE";
	if (!this->textures["WARRIOR_SHEET"].loadFromFile("gamedata/Resources/Images/Sprites/Player/warriorSheet.png"))
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_WARRIOR_TEXTURE";
	if (!this->textures["RANGER_SHEET"].loadFromFile("gamedata/Resources/Images/Sprites/Player/rangerSheet.png"))
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_RANGER_TEXTURE";
	

	// Enemy Textures
	if (!this->textures["GOBLIN_SHEET"].loadFromFile("gamedata/Resources/Images/Sprites/Enemy/goblin.png"))	
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_GOBLIN_TEXTURE";	
	if (!this->textures["SKELETON_SHEET"].loadFromFile("gamedata/Resources/Images/Sprites/Enemy/skeleton.png"))	
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_SKELETON_TEXTURE";	
	if (!this->textures["SLIME_SHEET"].loadFromFile("gamedata/Resources/Images/Sprites/Enemy/slime.png"))	
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_SLIME_TEXTURE";	
	if (!this->textures["SPIDER_SHEET"].loadFromFile("gamedata/Resources/Images/Sprites/Enemy/spider.png"))	
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_SPIDER_TEXTURE";
	
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
	if (!this->core_shader.loadFromFile("gamedata/vertex_shader.vert", "gamedata/fragment_shader.frag"))	
		std::cout << "ERROR::GAMESTATE::COULD NOT LOAD SHADER." << "\n";
	
}

//----------------------
//Initialize KeyTime
//----------------------
void GameState::initKeytime()
{
	this->keyTimeMax = 0.2f;
	this->keyTimer.restart();
}

//----------------------
//Initialize DebugText
//----------------------
void GameState::initDebugText()
{
	this->debugText.setFont(this->font);
	this->debugText.setFillColor(sf::Color::Red);
	this->debugText.setCharacterSize(12);
	this->debugText.setPosition(15.f, this->window->getSize().y - 250.f);
}

//----------------------
//Initialize Players
//----------------------
void GameState::initPlayers()
{
	this->player = new Player(150, 200, this->textures["WARRIOR_SHEET"]);
}

//----------------------
//Initialize PlayerGUI
//----------------------
void GameState::initPlayerGUI()
{
	this->playerGUI = new PlayerGUI(this->player, this->stateData->gfxSettings->resolution);
}

//----------------------
//Initialize EnemySystem
//----------------------
void GameState::initEnemySystem()
{
	this->enemySystem = new EnemySystem(this->activeEnemies, this->textures, *this->player);
}

//----------------------
//Initialize TileMap
//----------------------
void GameState::initTileMap()
{
	this->tileMap = new TileMap("gamedata/level.slmp");
}

//----------------------
//Initialize Systems
//----------------------
void GameState::initSystems()
{
	this->tts = new TextTagSystem("gamedata/Fonts/Dosis.ttf");
}

//========================================================
//
//Constructors/Destructors
//
//========================================================
GameState::GameState(StateData* state_data)
	: State(state_data)
{
	std::cout << green << "Initializing Deferred Render..." << "\n" << white;
	this->initDeferredRender();

	std::cout << green << "Initializing View..." << "\n" << white;
	this->initView();

	std::cout << green << "Initializing Keybinds..." << "\n" << white;
	this->initKeybinds();

	std::cout << green << "Initializing Fonts..." << "\n" << white;
	this->initFonts();

	std::cout << green << "Initializing Textures..." << "\n" << white;
	this->initTextures();

	std::cout << green << "Initializing Pause Menu..." << "\n" << white;
	this->initPauseMenu();

	std::cout << green << "Initializing Shaders..." << "\n" << white;
	this->initShaders();

	std::cout << green << "Initializing Key Time..." << "\n" << white;
	this->initKeytime();

	std::cout << yellow << "Initializing Debug Text..." << "\n" << white;
	this->initDebugText();

	std::cout << green << "Initializing Players..." << "\n" << white;
	this->initPlayers();

	std::cout << green << "Initializing Player GUI..." << "\n" << white;
	this->initPlayerGUI();

	std::cout << green << "Initializing Enemy System..." << "\n" << white;
	this->initEnemySystem();

	std::cout << green << "Initializing Tile Map..." << "\n" << white;
	this->initTileMap();

	std::cout << green << "Initializing Systems..." << "\n" << white;
	this->initSystems();

	std::cout << blue << "Initializing complete!" << "\n" << white;
}

GameState::~GameState()
{
	delete this->pmenu;
	delete this->player;
	delete this->playerGUI;
	delete this->enemySystem;
	delete this->tileMap;
	delete this->tts;

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
const bool GameState::getKeytime()
{
	if (this->keyTimer.getElapsedTime().asSeconds() >= this->keyTimeMax)
	{
		this->keyTimer.restart();
		return true;
	}

	return false;
}



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
		this->player->move(0.f, -1.f, dt);	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN"))))	
		this->player->move(0.f, 1.f, dt);

	///
	/// Dev Controls
	/// 
	
	// Gain XP
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && this->getKeytime())	
			this->player->getAttributeComponent()->gainExp(20);

	// Hard Reset Game
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && this->getKeytime())
	{
		this->states->pop();
		this->states->push(new GameState(this->stateData));
	}
}

void GameState::updatePlayerGUI(const float& dt)
{
	this->playerGUI->update(dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("TOGGLE_PLAYER_TAB_CHARACTER"))) && this->getKeytime())
	{
		this->playerGUI->toggleCharacterTab();
	}
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
}

void GameState::updatePlayer(const float& dt)
{
	//Look here for weird char positioning bug
	this->player->update(dt, this->mousePosView, this->view);
}

void GameState::updateCombatAndEnemies(const float& dt)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->player->getWeapon()->getAttackTimer())
		this->player->setInitAttack(true);

	unsigned index = 0;
	for (auto* enemy : this->activeEnemies)
	{
		enemy->update(dt, this->mousePosView, this->view);

		this->tileMap->updateWorldBoundsCollision(enemy, dt);
		this->tileMap->updateTileCollision(enemy, dt);

		this->updateCombat(enemy, index, dt);

		//DANGEROUS
		if (enemy->isDead())
		{
			this->player->gainEXP(enemy->getGainExp());
			this->tts->addTextTag(EXPERIENCE_TAG, this->player->getCenter().x - 8, this->player->getCenter().y - 30.f, static_cast<int>(enemy->getGainExp()), "+", "EXP");

			this->enemySystem->removeEnemy(index);
			continue;
		}
		else if (enemy->getDespawnTimerDone())
		{
			this->enemySystem->removeEnemy(index);
			continue;
		}
		++index;
	}

	this->player->setInitAttack(false);
}

void GameState::updateCombat(Enemy* enemy, const int index, const float& dt)
{
	if (this->player->getInitAttack()
		&& enemy->getGlobalBounds().contains(this->mousePosView)
		&& enemy->getSpriteDistance(*this->player) < this->player->getWeapon()->getRange()
		&& enemy->getDamageTimerDone())
	{
		//Get to this!!
		int dmg = static_cast<int>(this->player->getDamage());
		enemy->loseHP(dmg);
		enemy->resetDamageTimer();
		this->tts->addTextTag(DEFAULT_TAG, enemy->getPosition().x, enemy->getPosition().y, dmg, "", "");
	}

	//Check for enemy damage
	if (enemy->getGlobalBounds().intersects(this->player->getGlobalBounds()) && player->getDamageTimer())
	{
		int dmg = enemy->getAttributeComp()->damageMax;
		this->player->loseHP(dmg);
		this->tts->addTextTag(NEGATIVE_TAG, player->getPosition().x - 30.f, player->getPosition().y, dmg, "-", "HP");
	}
}

void GameState::updateDebugText(const float& dt)
{
	std::stringstream ss;

	ss << "Mouse Pos View: " << this->mousePosView.x << " " << this->mousePosView.y << "\n"
		<< "Active Enemies: " << this->activeEnemies.size() << "\n"
		<< "FPS: " << fps.getFPS();

	this->fps.update();
	this->debugText.setString(ss.str());
}

void GameState::update(const float& dt)
{
	this->updateMousePositions(&this->view);
	this->updateKeytime(dt);
	this->updateInput(dt);

	this->updateDebugText(dt);

	if (!this->paused) //Unpaused update
	{
		
		this->updateView(dt);

		this->updatePlayerInput(dt);

		this->updateTileMap(dt);

		this->updatePlayer(dt);

		this->updatePlayerGUI(dt);

		//Update all enemies
		this->updateCombatAndEnemies(dt);

		//UpdateSystems
		this->tts->update(dt);
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

	this->tileMap->render( this->renderTexture,	this->viewGridPosition,	&this->core_shader,	this->player->getCenter(), false);

	//Render Enemies
	for (auto* enemy : this->activeEnemies)	
		enemy->render(this->renderTexture, &this->core_shader, this->player->getCenter(), false);	

	this->player->render(this->renderTexture, &this->core_shader, this->player->getCenter(), false);

	//Deferred Rendering
	this->tileMap->renderDeferred(this->renderTexture, &this->core_shader, this->player->getCenter());

	this->tts->render(this->renderTexture);

	//Render GUI
	this->renderTexture.setView(this->renderTexture.getDefaultView());
	this->playerGUI->render(this->renderTexture);

	if (this->paused) //Pause menu render
	{
		//this->renderTexture.setView(this->renderTexture.getDefaultView());
		this->pmenu->render(this->renderTexture);
	}
	//Debug Text
	this->renderTexture.draw(this->debugText);

	//FINAL RENDER
	this->renderTexture.display();
	this->renderSprite.setTexture(this->renderTexture.getTexture());
	target->draw(this->renderSprite);
}