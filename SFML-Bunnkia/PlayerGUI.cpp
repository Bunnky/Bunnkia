#include "stdafx.h"
#include "PlayerGUI.h"

void PlayerGUI::initFont()
{
	this->font.loadFromFile("Fonts/Consolas.ttf");
}

void PlayerGUI::initLevelBar()
{
	float width = gui::p2pX(2.0f, this->vm);
	float height = gui::p2pY(3.0f, this->vm);
	float x = gui::p2pX(2.5f, this->vm);
	float y = gui::p2pY(10.f, this->vm);

	this->levelBarBack.setSize(sf::Vector2f(width, height));
	this->levelBarBack.setFillColor(sf::Color(50, 50, 50, 200));
	this->levelBarBack.setPosition(x, y);

	this->levelBarText.setFont(this->font);
	this->levelBarText.setCharacterSize(gui::calcCharSize(this->vm, 120));
	this->levelBarText.setPosition(
		this->levelBarBack.getPosition().x + gui::p2pX(0.53f, this->vm),
		this->levelBarBack.getPosition().y + gui::p2pY(0.5f, this->vm)
	);
}

void PlayerGUI::initEXPBar()
{
	this->expBar = new gui::ProgressBar(
		2.5f, 6.7f, 18.7f, 2.5f,
		this->player->getAttributeComponent()->expNext,
		sf::Color::Blue, 160,
		this->vm, &this->font);
}

void PlayerGUI::initHPBar()
{
	this->hpBar = new gui::ProgressBar(
		2.5f, 3.3f, 18.7f, 3.3f, 
		this->player->getAttributeComponent()->hpMax, 
		sf::Color::Red, 120,
		this->vm, &this->font);
}

void PlayerGUI::initPlayerTabs(sf::VideoMode& vm, sf::Font& font, Player& player)
{
	this->playerTabs = new PlayerGUITabs(vm, font, player);
}


PlayerGUI::PlayerGUI(Player* player, sf::VideoMode& vm)
	: vm(vm)
{
	this->player = player;

	this->initFont();
	this->initLevelBar();
	this->initEXPBar();
	this->initHPBar();
	this->initPlayerTabs(vm, font, *player);
}

PlayerGUI::~PlayerGUI()
{
	delete this->hpBar;
	delete this->expBar;
	delete this->playerTabs;
}

//Functions
void PlayerGUI::updateLevelBar()
{
	this->levelBarString = std::to_string(this->player->getAttributeComponent()->level);
	this->levelBarText.setString(this->levelBarString);
}

void PlayerGUI::updateEXPBar()
{
	this->expBar->update(this->player->getAttributeComponent()->exp);
}

void PlayerGUI::updateHPBar()
{
	this->hpBar->update(this->player->getAttributeComponent()->hp);
}

void PlayerGUI::update(const float& dt)
{
	this->updateLevelBar();
	this->updateEXPBar();
	this->updateHPBar();
}

void PlayerGUI::renderLevelBar(sf::RenderTarget& target)
{
	target.draw(this->levelBarBack);
	target.draw(this->levelBarText);
}

void PlayerGUI::renderEXPBar(sf::RenderTarget& target)
{
	this->expBar->render(target);
}

void PlayerGUI::renderHPBar(sf::RenderTarget& target)
{
	this->hpBar->render(target);
}

//Tabs
void PlayerGUI::render(sf::RenderTarget& target)
{
	this->renderLevelBar(target);
	this->renderEXPBar(target);
	this->renderHPBar(target);
}
