#ifndef PLAYERGUITABS_H
#define PLAYERGUITABS_H

#include"CharacterTab.h"

enum PlayerTabs {CHARCTER_TAB = 0, INVENTORY_TAB };

class PlayerGUITabs
{
private:

	std::vector<Tab*> tabs;

	//Core
	sf::VideoMode& vm;
	sf::Font font;
	Player& player;

	sf::Clock keyTimer;
	float keyTimeMax;

	//Private Functions
	void initTabs();
	void initKeyTime();

public:
	PlayerGUITabs(sf::VideoMode& vm, sf::Font font, Player& player);
	virtual ~PlayerGUITabs();

	const bool getKeyTime();
	const bool tabsOpen();

	void toggleTab(const int tab_index);

	void update();
	void render(sf::RenderTarget& target);
};

#endif // !PLAYERGUITABS_H