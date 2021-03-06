#include "SceneTitle.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"
#include "SceneManager.h"
#include "Font.h"
#include "App.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

SceneTitle::SceneTitle(Window* win, SceneManager* sceneManager, AudioManager* audio, Render* render, App* app)
{
    this->app = app;
    this->sceneManager = sceneManager;
    this->window = win;
    this->audio = audio;
    this->render = render;

    // GUI: Initialize required controls for the screen
    btnPlay = new GuiButton(1, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 20, 190, 40 }, "PLAY");
    btnPlay->SetObserver(this);

    btnContinue = new GuiButton(2, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 80, 190, 40 }, "CONTINUE");
    btnContinue->SetObserver(this);

    btnSettings = new GuiButton(3, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 140, 190, 40 }, "SETTINGS");
    btnSettings->SetObserver(this);
    
    btnCredits = new GuiButton(4, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 200, 190, 40 }, "CREDITS");
    btnCredits->SetObserver(this);
    
    btnExit = new GuiButton(5, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 260, 190, 40 }, "EXIT");
    btnExit->SetObserver(this);
    
    // GUI: Initialize required controls for the settings
    sldrMusicVolume = new GuiSlider(6, { 700, (int)win->GetWindowHeight() / 2 + 23, 35, 35 }, "MUSICVOLUME");
    sldrMusicVolume->SetObserver(this);

    sldrFxVolume = new GuiSlider(7, { 700, (int)win->GetWindowHeight() / 2 + 83, 35, 35 }, "FXVOLUME");
    sldrFxVolume->SetObserver(this);

    cbxFullscreen = new GuiCheckBox(8, { (int)win->GetWindowWidth() / 2 - 45 / 2 - 200, (int)win->GetWindowHeight() / 2 + 140, 45, 49 }, "FULLSCREEN");
    cbxFullscreen->SetObserver(this);

    cbxVSync = new GuiCheckBox(9, { (int)win->GetWindowWidth() / 2 - 45 / 2 - 200, (int)win->GetWindowHeight() / 2 + 200, 45, 49 }, "VSYNC");
    cbxVSync->SetObserver(this);

    // Rectangles definition
    backgroundRect = { 0,0,1280,720 };
    barRect = { 0,0,300,35 };

    mouseCursorRect[0] = { 30,482,30,30 };
    mouseCursorRect[1] = { 60,482,30,30 };

    menuCurrentSelection = MenuSelection::NONE;
    settingsCurrentSelection = SettingsSelection::NONE;
}

SceneTitle::~SceneTitle()
{
}

bool SceneTitle::Load(Textures* tex)
{
    backgroundTexture = tex->Load("Assets/Textures/Scenes/title_screen.png");
    barTexture = tex->Load("Assets/Textures/UI/bar.png");
    atlasGUITexture = tex->Load("Assets/Textures/UI/uipack_rpg_sheet.png");

    hoverFx = audio->LoadFx("Assets/Audio/Fx/bong.ogg");
    clickFx = audio->LoadFx("Assets/Audio/Fx/click.ogg");

    btnPlay->SetTexture(atlasGUITexture);
    btnContinue->SetTexture(atlasGUITexture);
    btnSettings->SetTexture(atlasGUITexture);
    btnCredits->SetTexture(atlasGUITexture);
    btnExit->SetTexture(atlasGUITexture);
    sldrMusicVolume->SetTexture(atlasGUITexture);
    sldrFxVolume->SetTexture(atlasGUITexture);
    cbxFullscreen->SetTexture(atlasGUITexture);
    cbxVSync->SetTexture(atlasGUITexture);

    font = new Font("Assets/Fonts/happy_school.xml", tex);

    audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

    this->vsync = render->vsync;

    if(!app->isGameSaved())
        btnContinue->state = GuiControlState::DISABLED;

    return false;
}

bool SceneTitle::Update(Input* input, float dt)
{
    if (menuCurrentSelection == MenuSelection::NONE)
    {
        btnPlay->Update(input, dt, audio, hoverFx, clickFx);
        btnContinue->Update(input, dt, audio, hoverFx, clickFx);
        btnSettings->Update(input, dt, audio, hoverFx, clickFx);
        btnCredits->Update(input, dt, audio, hoverFx, clickFx);
        btnExit->Update(input, dt, audio, hoverFx, clickFx);
    }
    else if (menuCurrentSelection == MenuSelection::PLAY)
    {
        TransitionToScene(SceneType::GAMEPLAY);
    }
    else if (menuCurrentSelection == MenuSelection::CONTINUE)
    {
        if (app->isGameSaved())
        {
            sceneManager->continueOption = true;
            app->LoadGameRequest();
            TransitionToScene(SceneType::GAMEPLAY);
        }
    }
    else if (menuCurrentSelection == MenuSelection::SETTINGS)
    {
        sldrMusicVolume->Update(input, dt);
        sldrFxVolume->Update(input, dt);
        cbxFullscreen->Update(input, dt);
        cbxVSync->Update(input, dt);

        if (settingsCurrentSelection == SettingsSelection::MUSICVOLUME)
        {
            Mix_VolumeMusic(sldrMusicVolume->GetValue());
        }
        else if (settingsCurrentSelection == SettingsSelection::FXVOLUME)
        {
            Mix_Volume(-1, sldrFxVolume->GetValue());
        }
        else if (settingsCurrentSelection == SettingsSelection::FULLSCREEN)
        {
            if (fullscreen == false)
            {
                SDL_SetWindowFullscreen(window->window, 1);
                fullscreen = true;
            }
            else
            {
                SDL_SetWindowFullscreen(window->window, 0);
                fullscreen = false;
            }
            settingsCurrentSelection = SettingsSelection::NONE;
        }
        else if (settingsCurrentSelection == SettingsSelection::VSYNC)
        {
            if (vsync == false)
            {
                SDL_GL_SetSwapInterval(1);
                vsync = true;
            }
            else
            {
                SDL_GL_SetSwapInterval(0);
                vsync = false;
            }
        }

        settingsCurrentSelection = SettingsSelection::NONE;
    }
    else if (menuCurrentSelection == MenuSelection::EXIT)
    {
        sceneManager->menuExitCall = true;
    }

    if (input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
        menuCurrentSelection = MenuSelection::NONE;

    input->GetMousePosition(mousePos.x, mousePos.y);

    if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
        clicking = true;
    else
        clicking = false;

    return false;
}

bool SceneTitle::Draw(Render* render)
{
    render->camera = { 0,0 };
    render->DrawTexture(backgroundTexture, 0, 0, &backgroundRect);

    if (menuCurrentSelection == MenuSelection::NONE)
    {
        btnPlay->Draw(render);
        btnContinue->Draw(render);
        btnSettings->Draw(render);
        btnCredits->Draw(render);
        btnExit->Draw(render);

        int offset = 3;
        render->DrawText(font, "PARACELSUS", 370 + offset, 250 + offset, 100, 13, { 105,105,105,255 });
        render->DrawText(font, "PARACELSUS", 370, 250, 100, 13, { 255,255,255,255 });

        render->DrawText(font, "PLAY", 596 + offset, 383 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "PLAY", 596, 383, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "CONTINUE", 557 + offset, 443 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "CONTINUE", 557, 443, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "SETTINGS", 557 + offset, 503 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "SETTINGS", 557, 503, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "CREDITS", 573 + offset, 563 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "CREDITS", 573, 563, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "EXIT", 605 + offset, 623 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "EXIT", 605, 623, 40, 5, { 255,255,255,255 });
    }
    else if (menuCurrentSelection == MenuSelection::SETTINGS)
    {
        render->DrawTexture(barTexture, 567, 383, &barRect);
        render->DrawTexture(barTexture, 567, 443, &barRect);

        sldrMusicVolume->Draw(render);
        sldrFxVolume->Draw(render);
        cbxFullscreen->Draw(render);
        cbxVSync->Draw(render);

        int offset = 3;
        render->DrawText(font, "SETTINGS", 425 + offset, 250 + offset, 100, 13, { 105,105,105,255 });
        render->DrawText(font, "SETTINGS", 425, 250, 100, 13, { 255,255,255,255 });

        render->DrawText(font, "MUSIC", 425 + offset, 383 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "MUSIC", 425, 383, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "FX", 425 + offset, 443 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "FX", 425, 443, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "FULLSCREEN", 475 + offset, 503 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "FULLSCREEN", 475, 503, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "VSYNC", 475 + offset, 563 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "VSYNC", 475, 563, 40, 5, { 255,255,255,255 });

        render->DrawText(font, "PRESS 'B' TO RETURN" , 475 + offset, 623 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "PRESS 'B' TO RETURN", 475, 623, 40, 5, { 255,255,255,255 });
    }
    else if (menuCurrentSelection == MenuSelection::CREDITS)
    {
        int offset = 3;
        render->DrawText(font, "AUTHORS:", 570 + offset, 190 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "AUTHORS:", 570, 190, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "ALEJANDRO AVILA", 490 + offset, 230 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "ALEJANDRO AVILA", 490, 230, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "BOSCO BARBER", 520 + offset, 270 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "BOSCO BARBER", 520, 270, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "YERAY TARIFA", 520 + offset, 310 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "YERAY TARIFA", 520, 310, 40, 5, { 255,255,255,255 });

        offset = 2;
        render->DrawText(font, "This project is licensed under an unmodified MIT license, which is an", 150 + offset, 400 + offset, 30, 3, { 105,105,105,255 });
        render->DrawText(font, "This project is licensed under an unmodified MIT license, which is an", 150, 400, 30, 3, { 255,255,255,255 });
        render->DrawText(font, "OSI-certified license that allows static linking with closed source software.", 150 + offset, 430 + offset, 30, 3, { 105,105,105,255 });
        render->DrawText(font, "OSI-certified license that allows static linking with closed source software.", 150, 430, 30, 3, { 255,255,255,255 });
        render->DrawText(font, "The assets' work of this project is licensed under the", 150 + offset, 490 + offset, 30, 3, { 105,105,105,255 });
        render->DrawText(font, "The assets' work of this project is licensed under the", 150, 490, 30, 3, { 255,255,255,255 });
        render->DrawText(font, "Creative Commons Attribution 4.0 International License.", 150 + offset, 520 + offset, 30, 3, { 105,105,105,255 });
        render->DrawText(font, "Creative Commons Attribution 4.0 International License.", 150, 520, 30, 3, { 255,255,255,255 });

        offset = 3;
        render->DrawText(font, "PRESS 'B' TO RETURN", 475 + offset, 623 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "PRESS 'B' TO RETURN", 475, 623, 40, 5, { 255,255,255,255 });
    }

    if (clicking)
        render->DrawTextureWithoutCamera(atlasGUITexture, mousePos.x, mousePos.y, &mouseCursorRect[0]);
    else
        render->DrawTextureWithoutCamera(atlasGUITexture, mousePos.x, mousePos.y, &mouseCursorRect[1]);

    return false;
}

bool SceneTitle::Unload(Textures* tex, AudioManager* audio)
{
    tex->UnLoad(backgroundTexture);
    tex->UnLoad(barTexture);
    tex->UnLoad(atlasGUITexture);
    backgroundTexture = nullptr;
    barTexture = nullptr;
    atlasGUITexture = nullptr;

    delete btnPlay;
    delete btnContinue;
    delete btnSettings;
    delete btnCredits;
    delete btnExit;
    delete sldrMusicVolume;
    delete sldrFxVolume;
    delete cbxFullscreen;
    delete cbxVSync;

    this->app = nullptr;
    this->sceneManager = nullptr;
    this->window = nullptr;
    this->audio = nullptr;
    this->render = nullptr;

    delete font;

    return true;
}

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneTitle::OnGuiMouseClickEvent(GuiControl* control)
{
    switch (control->type)
    {
    case GuiControlType::BUTTON:
    {
        if (control->id == 1) menuCurrentSelection = MenuSelection::PLAY;
        else if (control->id == 2) menuCurrentSelection = MenuSelection::CONTINUE;
        else if (control->id == 3) menuCurrentSelection = MenuSelection::SETTINGS;
        else if (control->id == 4) menuCurrentSelection = MenuSelection::CREDITS;
        else if (control->id == 5) menuCurrentSelection = MenuSelection::EXIT; // TODO: Exit request
    }
    case GuiControlType::SLIDER:
    {
        if (control->id == 6) settingsCurrentSelection = SettingsSelection::MUSICVOLUME;
        else if (control->id == 7) settingsCurrentSelection = SettingsSelection::FXVOLUME;
    }
    case GuiControlType::CHECKBOX:
    {
        if (control->id == 8) settingsCurrentSelection = SettingsSelection::FULLSCREEN;
        else if (control->id == 9) settingsCurrentSelection = SettingsSelection::VSYNC;
    }
    default: break;
    }

    return true;
}