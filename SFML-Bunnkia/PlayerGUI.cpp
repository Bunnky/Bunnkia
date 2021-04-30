#include "stdafx.h"
#include "PlayerGUI.h"

void PlayerGUI::initFont()
{
	this->font.loadFromFile("Fonts/The Impostor.ttf");
}

void PlayerGUI::initHPBar()
{
	float width = 150.f;
	float height = 20.f;
	float x = 20.f;
	float y = 20.f;

	this->hpBarBack.setSize(sf::Vector2f(width, height));
	this->hpBarBack.setFillColor(sf::Color(50, 50, 50, 200));
	this->hpBarBack.setPosition(x, y);

	this->hpBarInner.setSize(sf::Vector2f(width, height));
	this->hpBarInner.setFillColor(sf::Color(250, 20, 20, 200));
	this->hpBarInner.setPosition(this->hpBarBack.getPosition());
}

PlayerGUI::PlayerGUI(Player* player)
{
	this->player = player;

	this->initFont();
	this->initHPBar();
}

PlayerGUI::~PlayerGUI()
{

}

//Functions
void PlayerGUI::updateHPBar()
{

}

void PlayerGUI::update(const float& dt)
{

}

void PlayerGUI::renderHPBar(sf::RenderTarget& target)
{
	target.draw(this->hpBarBack);
	target.draw(this->hpBarInner);
}

void PlayerGUI::render(sf::RenderTarget& target)
{
	this->renderHPBar(target);
}
