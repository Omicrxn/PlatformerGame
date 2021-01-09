#include "SceneTitle.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"
#include "Font.h"

#include "EntityManager.h"

#include "SDL/include/SDL.h"

SceneTitle::SceneTitle(Window* win, SceneManager* sceneManager)
{
    // GUI: Initialize required controls for the screen
    /*btnStart = new GuiButton(1, { (int)win->GetWindowWidth()/2 - 500/2, (int)win->GetWindowHeight() / 2+20, 500, 80 }, "START");
    btnStart->SetObserver(this);

    btnExit = new GuiButton(2, { (int)win->GetWindowWidth() / 2 - 500/2, (int)win->GetWindowHeight() / 2+120, 500, 80 }, "EXIT");
    btnExit->SetObserver(this);*/
    //background rectangle definition
    /*backgroundRect = { 0,0,1280,720 };*/

    // GUI: Initialize required controls for the screen
    btnStart = new GuiButton(1, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 20, 190, 40 }, "START");
    btnStart->SetObserver(this);

    btnContinue = new GuiButton(2, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 80, 190, 40 }, "CONTINUE");
    btnContinue->SetObserver(this);

    btnSettings = new GuiButton(3, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 140, 190, 40 }, "SETTINGS");
    btnSettings->SetObserver(this);

    btnCredits = new GuiButton(4, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 200, 190, 40 }, "CREDITS");
    btnCredits->SetObserver(this);

    btnExit = new GuiButton(5, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 260, 190, 40 }, "EXIT");
    btnExit->SetObserver(this);

    // GUI: Initialize required controls for the settings
    sldrMusicVolume = new GuiSlider(1, { (int)win->GetWindowWidth() / 2 - 500 / 2, (int)win->GetWindowHeight() / 2 + 20, 500, 40 }, "MUSICVOLUME");
    sldrMusicVolume->SetObserver(this);

    sldrFxVolume = new GuiSlider(2, { (int)win->GetWindowWidth() / 2 - 500 / 2, (int)win->GetWindowHeight() / 2 + 80, 500, 40 }, "FXVOLUME");
    sldrFxVolume->SetObserver(this);

    cbxFullscreenMode = new GuiCheckBox(1, { (int)win->GetWindowWidth() / 2 - 45 / 2 - 200, (int)win->GetWindowHeight() / 2 + 140, 45, 49 }, "FULLSCREEN");
    cbxFullscreenMode->SetObserver(this);

    cbxVSync = new GuiCheckBox(2, { (int)win->GetWindowWidth() / 2 - 45 / 2 - 200, (int)win->GetWindowHeight() / 2 + 200, 45, 49 }, "VSYNC");
    cbxVSync->SetObserver(this);

    //Rectangles definition
    backgroundRect = { 0,0,1280,720 };
    creditsRect = { 0,0,1280,720 };

    whiteButton = { 0,0,190,49 };
    brownButton = { 0,49,190,49 };
    greyButton = { 0,188,190,49 };
    yellowButton = { 0,282,190,49 };

    greyCheckBox = { 290,0,45,49 };
    yellowCheckBox = { 293,294,45,49 };
    brownCheckBox = { 293,343,45,49 };
    whiteCheckBox = { 293,437,45,49 };

    this->sceneManager = sceneManager;
    this->window = win;

    menuCurrentSelection = MenuSelection::NONE;
    settingsCurrentSelection = SettingsSelection::NONE;
}

SceneTitle::~SceneTitle()
{
}

bool SceneTitle::Load(Textures* tex)
{
    backgroundTexture = tex->Load("Assets/Textures/Scenes/title_screen.png");
    atlasGUI = tex->Load("Assets/Textures/UI/uipack_rpg_sheet.png");
    font = new Font("Assets/Fonts/happy_school.xml", tex);

    return false;
}

bool SceneTitle::Update(Input* input, float dt)
{
    if (menuCurrentSelection == MenuSelection::NONE)
    {
        btnStart->Update(input, dt);
        btnContinue->Update(input, dt);
        btnSettings->Update(input, dt);
        btnCredits->Update(input, dt);
        btnExit->Update(input, dt);
    }
    else if (menuCurrentSelection == MenuSelection::PLAY)
    {
        TransitionToScene(SceneType::GAMEPLAY);
    }
    else if (menuCurrentSelection == MenuSelection::CONTINUE)
    {

    }
    else if (menuCurrentSelection == MenuSelection::SETTINGS)
    {
        sldrMusicVolume->Update(input, dt);
        sldrFxVolume->Update(input, dt);
        cbxFullscreenMode->Update(input, dt);
        cbxVSync->Update(input, dt);

        if (settingsCurrentSelection == SettingsSelection::FULLSCREEN)
        {
            if (fullscreen == false)
            {
                SDL_SetWindowFullscreen(window->window, SDL_WINDOW_FULLSCREEN);
                fullscreen = true;
            }
            else
            {
                SDL_SetWindowFullscreen(window->window, 0);
                fullscreen = false;
            }
        }
        else if (settingsCurrentSelection == SettingsSelection::VSYNC)
        {

        }

        settingsCurrentSelection = SettingsSelection::NONE;
    }
    else if (menuCurrentSelection == MenuSelection::EXIT)
    {
        sceneManager->menuExitCall = true;
    }

    if (input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
        menuCurrentSelection = MenuSelection::NONE;

    return false;
}

bool SceneTitle::Draw(Render* render)
{
    render->DrawTexture(backgroundTexture, 0, 0, &backgroundRect);

    if (menuCurrentSelection == MenuSelection::NONE)
    {
        btnStart->Draw(render, this);
        btnContinue->Draw(render, this);
        btnSettings->Draw(render, this);
        btnCredits->Draw(render, this);
        btnExit->Draw(render, this);

        int offset = 3;
        render->DrawText(font, "PLAY", 596 + offset, 383 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "CONTINUE", 557 + offset, 443 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "SETTINGS", 557 + offset, 503 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "CREDITS", 573 + offset, 563 + offset, 40, 5, { 105,105,105,255 });
        render->DrawText(font, "EXIT", 605 + offset, 623 + offset, 40, 5, { 105,105,105,255 });

        render->DrawText(font, "PLAY", 596, 383, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "CONTINUE", 557, 443, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "SETTINGS", 557, 503, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "CREDITS", 573, 563, 40, 5, { 255,255,255,255 });
        render->DrawText(font, "EXIT", 605, 623, 40, 5, { 255,255,255,255 });
    }
    else if (menuCurrentSelection == MenuSelection::SETTINGS)
    {
        render->DrawRectangle(backgroundRect, { 0, 0, 0, 227 });

        sldrMusicVolume->Draw(render);
        sldrFxVolume->Draw(render);
        cbxFullscreenMode->Draw(render, this);
        cbxVSync->Draw(render, this);

        render->DrawText(font, "Fullscreen", 450, 495, 50, 5, { 255,255,255,255 });
    }
    else if (menuCurrentSelection == MenuSelection::CREDITS)
    {
        render->DrawRectangle(backgroundRect, { 0, 0, 0, 227 });

        render->DrawText(font, "Authors:", 540, 100, 50, 5, { 255,255,255,255 });
        render->DrawText(font, "Alejandro Avila", 460, 160, 50, 5, { 255,255,255,255 });
        render->DrawText(font, "Bosco Barber", 490, 220, 50, 5, { 255,255,255,255 });
        render->DrawText(font, "Yeray Tarifa", 490, 280, 50, 5, { 255,255,255,255 });
        render->DrawText(font, "This project is licensed under an unmodified MIT license, which is an", 215, 400, 25, 3, { 255,255,255,255 });
        render->DrawText(font, "OSI-certified license that allows static linking with closed source software.", 215, 430, 25, 3, { 255,255,255,255 });
        render->DrawText(font, "The assets' work of this project is licensed under the", 215, 490, 25, 3, { 255,255,255,255 });
        render->DrawText(font, "Creative Commons Attribution 4.0 International License.", 215, 510, 25, 3, { 255,255,255,255 });
    }

    return false;
}

bool SceneTitle::Unload()
{
    return false;
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
        if (control->id == 1) settingsCurrentSelection = SettingsSelection::MUSICVOLUME;
        else if (control->id == 2) settingsCurrentSelection = SettingsSelection::FXVOLUME;
    }
    case GuiControlType::CHECKBOX:
    {
        if (control->id == 1) settingsCurrentSelection = SettingsSelection::FULLSCREEN;
        else if (control->id == 2) settingsCurrentSelection = SettingsSelection::VSYNC;
    }
    default: break;
    }

    return true;
}