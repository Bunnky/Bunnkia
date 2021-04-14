#pragma once

#include"Player.h"

class Player;

class PlayerGUI
{
private:
	Player* player;

public:
	PlayerGUI(Player* player);
	virtual ~PlayerGUI();

	//Functions
	void update(const float& dt);
	void render(sf::RenderTarget& target);
};

