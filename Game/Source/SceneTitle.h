#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"

#include "GuiButton.h"
#include "SceneManager.h"

class Font;

enum class SelectedOption
{
    NONE,
    CONTINUE,
    SETTINGS,
    CREDITS,
    EXIT
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

private:

    GuiButton* btnStart;
    GuiButton* btnContinue;
    GuiButton* btnSettings;
    GuiButton* btnCredits;
    GuiButton* btnExit;
    SDL_Texture* backgroundTexture;
    SDL_Rect backgroundRect;
    SDL_Rect creditsRect;

    SceneManager* sceneManager;
    SelectedOption currentSelection;

    Font* font;
};

#endif // __SCENETITLE_H__
