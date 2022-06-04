#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include "Gui.h"

class Gui;

class LoadingScreen
{
private:
	sf::Font& font;
	sf::Text loadingText;

	sf::RectangleShape background;
	sf::RectangleShape container;
	//===========//
	// Functions //
	//===========//

public:
	LoadingScreen(sf::VideoMode& vm, sf::Font& font);
	virtual ~LoadingScreen();

	//===========//
	// Accessors //
	//===========//

	//===========//
	// Functions //
	//===========//
	void update(const sf::Vector2i& mousePosWindow);
	void render(sf::RenderTarget& target);
};

#endif //LOADINGSCREEN_H
