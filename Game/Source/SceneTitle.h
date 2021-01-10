#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

class App;
class SceneManager;
class Window;
class Audio;
class Render;

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

    SceneTitle(Window* win, SceneManager* sceneManager, AudioManager* audio, Render* render, App* app, GuiManager* guiManager);
    virtual ~SceneTitle();

    bool Load(Textures* tex);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio);

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);
    
private:

    // Main menu buttons
    GuiButton* btnPlay;
    GuiButton* btnContinue;
    GuiButton* btnSettings;
    GuiButton* btnCredits;
    GuiButton* btnExit;

    // Settings' sliders and checkboxes
    GuiSlider* sldrMusicVolume;
    GuiSlider* sldrFxVolume;
    GuiCheckBox* cbxFullscreen;
    GuiCheckBox* cbxVSync;

    SDL_Texture* atlasGUI;
    SDL_Texture* backgroundTexture;
    SDL_Texture* barTexture;
    SDL_Rect backgroundRect;
    SDL_Rect barRect;

    SceneManager* sceneManager;
    Window* window;
    AudioManager* audio;
    Render* render;
    App* app;
    GuiManager* guiManager;

    MenuSelection menuCurrentSelection;
    SettingsSelection settingsCurrentSelection;

    Font* font;

    bool fullscreen = false;
    bool vsync = false;
};

#endif // __SCENETITLE_H__