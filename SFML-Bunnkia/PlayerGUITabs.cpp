#include "stdafx.h"
#include "PlayerGUITabs.h"

void PlayerGUITabs::initTabs()
{
	this->tabs.push_back(new CharacterTab(vm, font, player));
}

void PlayerGUITabs::initKeytime()
{
	this->keyTimeMax = 0.2f;
	this->keyTimer.restart();
}

PlayerGUITabs::PlayerGUITabs(sf::VideoMode& vm, sf::Font font, Player& player)
	: vm(vm), font(font), player(player)
{
	this->initTabs();
	this->initKeytime();
}

PlayerGUITabs::~PlayerGUITabs()
{
}

const bool PlayerGUITabs::getKeytime()
{
	if (this->keyTimer.getElapsedTime().asSeconds() >= this->keyTimeMax)
	{
		this->keyTimer.restart();
		return true;
	}

	return false;
}

const bool PlayerGUITabs::tabsOpen()
{
	//Loop through tabs and check if any are open
	bool open = false;
	for (size_t i = 0; i < this->tabs.size() && !open; i++)
	{
		if (this->tabs[i]->getOpen())
			open = true;
	}

	return open;
}

void PlayerGUITabs::toggleTab(const int tab_index)
{
	if (tab_index >= 0 || tab_index < this->tabs.size())
		this->tabs[tab_index]->toggle();
}

void PlayerGUITabs::update()
{
	for (size_t i = 0; i < this->tabs.size(); i++)
	{
		if (this->tabs[i]->getOpen())
			this->tabs[i]->update();
	}
}

void PlayerGUITabs::render(sf::RenderTarget& target)
{
	for (size_t i = 0; i < this->tabs.size(); i++)
	{
		if (this->tabs[i]->getOpen())
			this->tabs[i]->render(target);
	}
}