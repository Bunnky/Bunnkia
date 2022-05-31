#ifndef SPIDER_H
#define SPIDER_H

#include "Enemy.h"
#include"AIFollow.h"
#include "PlayerGUI.h"

class Spider :
	public Enemy
{
private:
	void initVariables();
	void initAnimations();
	void initAI();
	void initGUI();

	sf::RectangleShape hpBar;
	sf::RectangleShape hpBarBack;

	AIFollow* follow;

public:
	Spider(float x, float y, sf::Texture& texture_sheet, EnemySpawnerTile& enemy_spawner_tile, Entity& player);
	virtual ~Spider();

	//Functions
	void updateAnimation(const float& dt);
	void update(const float& dt, sf::Vector2f& mouse_pos_view, const sf::View& view);

	void render(sf::RenderTarget& target, sf::Shader* shader, const sf::Vector2f light_position, const bool show_hitbox);
};

#endif // !SPIDER_H