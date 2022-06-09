#ifndef GAMESTATE_H
#define GAMESTATE_H

#include"State.h"
#include"PauseMenu.h"
#include"TileMap.h"
#include"PlayerGUI.h"
#include"Sword.h"
#include"Bow.h"
#include"TextTagSystem.h"
#include"FPS.h"

class GameState :
	public State
{
private:

	Player* player;

	FPS fps;
	PauseMenu* pmenu;
	PlayerGUI* playerGUI;
	EnemySystem* enemySystem;
	TileMap* tileMap;
	TextTagSystem* tts;

	float keyTimeMax;

	sf::View view;
	sf::Vector2i viewGridPosition;
	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;
	sf::Font font;
	sf::Clock keyTimer;
	sf::Shader core_shader;
	sf::Text debugText;
	sf::Texture texture;

	std::vector<Enemy*> activeEnemies;


	//===========//
	// Functions //
	//===========//
	void initDeferredRender();
	void initView();
	void initKeybinds();
	void initFonts();
	void initTextures();
	void initPauseMenu();
	void initShaders();
	void initKeytime();
	void initDebugText();
	void initPlayers();
	void initPlayerGUI();
	void initEnemySystem();
	void initTileMap();
	void initSystems();

public:
	GameState(StateData* state_data);
	virtual ~GameState();

	//Accessors
	const bool getKeytime();

	//===========//
	// Functions //
	//===========//
	void updateView(const float& dt);
	void updateInput(const float& dt);
	void updatePlayerInput(const float& dt);
	void updatePlayerGUI(const float& dt);
	void updatePauseMenuButtons();
	void updateTileMap(const float& dt);
	void updatePlayer(const float& dt);
	void updateCombatAndEnemies(const float& dt);
	void updateCombat(Enemy* enemy, const int index, const float& dt);
	void updateDebugText(const float& dt);
	void update(const float& dt);
	void render(sf::RenderTarget* target = NULL);
};

#endif
