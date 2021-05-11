#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "EnemySpawnerTile.h"

class Enemy :
    public Entity
{
private:
    //Variables
    EnemySpawnerTile& enemySpawnerTile;
    unsigned gainExp;

    //Initializer functions
    virtual void initVariables();
    virtual void initAnimations();

public:
    Enemy(EnemySpawnerTile& enemy_spawner_tile);
    virtual ~Enemy();

    //Accessors
    const unsigned& getGainExp() const;
    EnemySpawnerTile& getEnemySpawnerTile();

    //Functions
    virtual void generateAttributes(const unsigned level);

    virtual void loseHP(const int hp);
    virtual const bool isDead() const;

    virtual const AttributeComponent* getAttributeComp() const;

    virtual void updateAnimation(const float& dt) = 0;

    virtual void update(const float& dt, sf::Vector2f& mouse_pos_view) = 0;
    virtual void render(sf::RenderTarget& target, sf::Shader* shader = NULL, const sf::Vector2f light_position = sf::Vector2f(), const bool show_hitbox = false) = 0;

};

#endif //ENEMY_H