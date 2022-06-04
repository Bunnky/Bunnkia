#include "stdafx.h"
#include "LoadingScreen.h"

//========================================================
//Constructors/Destructors
//========================================================
LoadingScreen::LoadingScreen(sf::VideoMode& vm, sf::Font& font)
	: font(font)
{
	//Init background
	this->background.setSize(sf::Vector2f(static_cast<float>(vm.width),	static_cast<float>(vm.height)));
	this->background.setFillColor(sf::Color(20, 20, 20, 100));

	//Init containter
	this->container.setSize(sf::Vector2f(static_cast<float>(vm.width) / 4.f, static_cast<float>(vm.height) / 4.f));
	this->container.setPosition(static_cast<float>(vm.width) / 2.f - this->container.getSize().x / 2.f,
		static_cast<float>(vm.height) / 2.f - this->container.getSize().y / 2.f);
	this->container.setFillColor(sf::Color(250, 250, 250, 250));

	//Init text
	this->loadingText.setFont(font);
	this->loadingText.setFillColor(sf::Color(0, 0, 0, 200));
	this->loadingText.setCharacterSize(gui::calcCharSize(vm));
	this->loadingText.setString("LOADING...");
	this->loadingText.setOutlineColor(sf::Color::Red);
	this->loadingText.setOutlineThickness(1);
	this->loadingText.setPosition(
		this->container.getPosition().x + this->container.getSize().x / 2.f - this->loadingText.getGlobalBounds().width / 2.f,
		this->container.getPosition().y + this->container.getSize().y / 2.f - this->loadingText.getGlobalBounds().height / 2.f);
	
}

LoadingScreen::~LoadingScreen()
{
}


//========================================================
//Functions
//========================================================
void LoadingScreen::update(const sf::Vector2i& mousePosWindow)
{
}

void LoadingScreen::render(sf::RenderTarget& target)
{
	target.draw(this->background);
	target.draw(this->container);
	target.draw(this->loadingText);
}