#ifndef PLAYERGUITABS_H
#define PLAYERGUITABS_H

#include"CharacterTab.h"

class PlayerGUITabs
{
private:

	CharacterTab characterTab;

	//Core
	sf::VideoMode& vm;
	sf::Font font;
	Player& player;

public:
	PlayerGUITabs(sf::VideoMode& vm, sf::Font font, Player& player);
	virtual ~PlayerGUITabs();

	void update();
	void render(sf::RenderTarget* target);
};

#endif // !PLAYERGUITABS_H