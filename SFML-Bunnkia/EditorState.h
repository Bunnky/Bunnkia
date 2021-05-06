#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include"State.h"
#include"Gui.h"
#include "PauseMenu.h"
#include "TileMap.h"
#include "DefaultEditorMode.h"

class State;
class StateData;
class Gui;
class PauseMenu;
class TileMap;
class Tile;
class EditorMode;
class DefaultEditorMode;
class EditorStateData;

enum EditorModes {DEFAULT_MODE = 0, ENEMY_MODE};

class EditorState :
    public State
{
private:
    //===========//
    // Variables //
    //===========//
    EditorStateData editorStateData;

    sf::View view;
    float cameraSpeed;
    sf::Font font;    
    PauseMenu* pmenu;

    std::map<std::string, gui::Button*> buttons;

    TileMap* tileMap;

    std::vector<EditorMode*> modes;

    //===========//
    // Functions //
    //===========//
    void initVariables();
    void initEditorStateData();
    void initView();
    void initFonts();
    void initKeybinds();
    void initPauseMenu();
    void initButtons();
    void initTileMap();
    void initGui();
    void initModes();

public:
    EditorState(StateData* state_data);
    virtual ~EditorState();

    //===========//
    // Functions //
    //===========//
    void updateInput(const float& dt);
    void updateEditorInput(const float& dt);
    void updateButtons();
    void updateGui(const float& dt);
    void updatePauseMenuButtons();
    void updateModes(const float& dt);
    void update(const float& dt);
    void renderButtons(sf::RenderTarget& target);
    void renderGui(sf::RenderTarget& target);
    void renderModes(sf::RenderTarget& target);
    void render(sf::RenderTarget* target = NULL);
};


#endif

