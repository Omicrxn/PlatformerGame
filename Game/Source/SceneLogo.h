#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Scene.h"

#include "SDL/include/SDL.h"

class SceneLogo : public Scene
{
public:

    SceneLogo();
    virtual ~SceneLogo();

    bool Load(Textures* tex);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio);

private:

    SDL_Rect logo = { 0,0,0,0 };
    SDL_Texture* logoTexture = nullptr;

    int state = 0;
    float timeCounter = 0.0f;
    float logoAlpha = 0.0f;
};

#endif // __SCENELOGO_H__
