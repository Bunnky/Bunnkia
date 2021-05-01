#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Entity;

class Player :
    public Entity
{
private:
    //===========//
    // Variables //
    //===========//
    bool attacking;
    sf::Texture weapon_texture;
    sf::Sprite weapon_sprite;

    //==============//
    // Initializers //
    //==============//
    void initVariables();
    void initComponents();

public:
    Player(float x, float y, sf::Texture& texture_sheet);
    virtual ~Player();

    //===========//
    // Accessors //
    //===========//
    AttributeComponent* getAttributeComponent();

    //===========//
    // Functions //
    //===========//
    void loseHP(const int hp);
    void gainHP(const int hp);
    void loseEXP(const int exp);
    void gainEXP(const int exp);

    void updateAttack();
    void updateAnimation(const float& dt);
    void update(const float& dt, sf::Vector2f& mouse_pos_view);

    void render(sf::RenderTarget& target, sf::Shader* shader = NULL, const bool show_hitbox = false);
};

#endif