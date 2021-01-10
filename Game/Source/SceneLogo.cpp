#include "SceneLogo.h"

#include "Render.h"
#include "Window.h"
#include "Textures.h"

#include "EntityManager.h"

#define LOGO_FADE_SPEED 255

SceneLogo::SceneLogo()
{
    
    logo = { 0, 0, 608, 280 };
    
    state = 0;
    timeCounter = 0.0f;
    logoAlpha = 0.0f;
}

SceneLogo::~SceneLogo()
{
}

bool SceneLogo::Load(Textures* tex)
{
    logoTexture = tex->Load("Assets/Textures/Scenes/logo.png");
    return false;
}

bool SceneLogo::Update(Input* inputHandler, float dt)
{
    if (state == 0)
    {
        state = 1;
    }
    else if (state == 1)
    {
        logoAlpha += (LOGO_FADE_SPEED * dt);

        if (logoAlpha > 255)
        {
            logoAlpha = 255;
            state = 2;
        }
    }
    else if (state == 2)
    {
        // Waiting for 3 seconds
        timeCounter += dt;
        if (timeCounter >= 3.0f) state = 3;
    }
    else if (state == 3)
    {
        logoAlpha -= (LOGO_FADE_SPEED * dt);

        if (logoAlpha < 0.0f)
        {
            logoAlpha = 0.0f;
            TransitionToScene(SceneType::TITLE);
        }
    }

    return false;
}

bool SceneLogo::Draw(Render* render)
{
    //background rectangle
    render->DrawRectangle({ 0, 0, 1280, 720 }, { 42, 43, 46, 255 });
    uint winWidth, winHeight;
    render->win->GetWindowSize(winWidth, winHeight);
    //logo
    SDL_SetTextureAlphaMod(logoTexture, logoAlpha);//set texture alpha with the updated logoAlpha to accomplish fade in / fade out
    render->DrawTexture(logoTexture, winWidth/2-logo.w/2,winHeight/2-logo.h/2, &logo);
    return false;
}

bool SceneLogo::Unload(Textures* tex, AudioManager* audio)
{   
    tex->UnLoad(logoTexture);
    logoTexture = nullptr;
    return true;
}
