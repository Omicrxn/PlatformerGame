#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"

#include "GuiButton.h"

class SceneTitle : public Scene
{
public:

    SceneTitle(Window* win);
    virtual ~SceneTitle();

    bool Load(Textures* tex);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload();

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

private:

    GuiButton* btnStart;
    GuiButton* btnExit;
    SDL_Texture* backgroundTexture;
    SDL_Rect backgroundRect;
};

#endif // __SCENETITLE_H__
