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

    SceneTitle(Window* win, SceneManager* sceneManager, AudioManager* audio, Render* render, App* app);
    virtual ~SceneTitle();

    bool Load(Textures* tex);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio);

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);
    
private:

    // Main menu buttons
    GuiButton* btnPlay = nullptr;
    GuiButton* btnContinue = nullptr;
    GuiButton* btnSettings = nullptr;
    GuiButton* btnCredits = nullptr;
    GuiButton* btnExit = nullptr;

    // Settings' sliders and checkboxes
    GuiSlider* sldrMusicVolume = nullptr;
    GuiSlider* sldrFxVolume = nullptr;
    GuiCheckBox* cbxFullscreen = nullptr;
    GuiCheckBox* cbxVSync = nullptr;

    SDL_Texture* atlasGUITexture = nullptr;
    SDL_Texture* backgroundTexture = nullptr;
    SDL_Texture* barTexture = nullptr;
    SDL_Rect backgroundRect = { 0,0,0,0 };
    SDL_Rect barRect = { 0,0,0,0 };

    SDL_Rect mouseCursorRect[2] = { 0,0,0,0 };

    iPoint mousePos = { 0,0 };
    bool clicking = false;
    int hoverFx = -1;
    int clickFx = -1;

    SceneManager* sceneManager = nullptr;
    Window* window = nullptr;
    AudioManager* audio = nullptr;
    Render* render = nullptr;
    App* app = nullptr;
    GuiManager* guiManager = nullptr;

    MenuSelection menuCurrentSelection = MenuSelection::NONE;
    SettingsSelection settingsCurrentSelection = SettingsSelection::NONE;

    Font* font = nullptr;

    bool fullscreen = false;
    bool vsync = false;
};

#endif // __SCENETITLE_H__