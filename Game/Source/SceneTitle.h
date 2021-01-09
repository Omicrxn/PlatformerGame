#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "SceneManager.h"
#include "Window.h"

class Font;

enum class MenuSelection
{
    NONE,
    PLAY,
    CONTINUE,
    SETTINGS,
    CREDITS,
    EXIT
};

enum class SettingsSelection
{
    NONE,
    MUSICVOLUME,
    FXVOLUME,
    FULLSCREEN,
    VSYNC
};

class SceneTitle : public Scene
{
public:

    SceneTitle(Window* win, SceneManager* sceneManager);
    virtual ~SceneTitle();

    bool Load(Textures* tex);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload();

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

    // GUI: Title Screen Main Menu buttons (They have to be accessible from the diferent GuiControls)
    SDL_Texture* atlasGUI;
    
private:

    // Main menu buttons
    GuiButton* btnStart;
    GuiButton* btnContinue;
    GuiButton* btnSettings;
    GuiButton* btnCredits;
    GuiButton* btnExit;

    // Settings' sliders and checkboxes
    GuiSlider* sldrMusicVolume;
    GuiSlider* sldrFxVolume;
    GuiCheckBox* cbxFullscreen;
    GuiCheckBox* cbxVSync;

    SDL_Texture* backgroundTexture;
    SDL_Rect backgroundRect;
    SDL_Rect creditsRect;

    SceneManager* sceneManager;
    Window* window;

    MenuSelection menuCurrentSelection;
    SettingsSelection settingsCurrentSelection;

    Font* font;

    bool fullscreen = false;
};

#endif // __SCENETITLE_H__