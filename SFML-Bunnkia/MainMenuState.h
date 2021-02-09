#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "GameState.h"
#include "SettingsState.h"
#include "EditorState.h"
#include "Gui.h"


class MainMenuState :
    public State
{
private:
    //===========//
    // Variables //
    //===========//

    sf::Texture backgroundTexture;
    sf::RectangleShape background;
    sf::Font font;


    std::map<std::string, gui::Button*> buttons;

    //===========//
    // Functions //
    //===========//
    void initVariables();
    void initBackground();
    void initFonts();
    void initKeybinds();
    void initButtons();

public:
    MainMenuState(StateData* state_data);
    virtual ~MainMenuState();


    //===========//
    // Functions //
    //===========//
    void updateInput(const float& dt);
    void updateButtons();
    void update(const float& dt);
    void renderButtons(sf::RenderTarget& target);
    void render(sf::RenderTarget* target = NULL);
};

#endif