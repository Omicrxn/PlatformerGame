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
    btnStart = new GuiButton(1, { (int)win->GetWindowWidth() / 2 - 500 / 2, (int)win->GetWindowHeight() / 2 + 20, 500, 40 }, "START");
    btnStart->SetObserver(this);

    btnContinue = new GuiButton(2, { (int)win->GetWindowWidth() / 2 - 500 / 2, (int)win->GetWindowHeight() / 2 + 80, 500, 40 }, "CONTINUE");
    btnContinue->SetObserver(this);

    btnSettings = new GuiButton(3, { (int)win->GetWindowWidth() / 2 - 500 / 2, (int)win->GetWindowHeight() / 2 + 140, 500, 40 }, "SETTINGS");
    btnSettings->SetObserver(this);

    btnCredits = new GuiButton(4, { (int)win->GetWindowWidth() / 2 - 500 / 2, (int)win->GetWindowHeight() / 2 + 200, 500, 40 }, "CREDITS");
    btnCredits->SetObserver(this);

    btnExit = new GuiButton(5, { (int)win->GetWindowWidth() / 2 - 500 / 2, (int)win->GetWindowHeight() / 2 + 260, 500, 40 }, "EXIT");
    btnExit->SetObserver(this);

    //background rectangle definition
    backgroundRect = { 0,0,1280,720 };

    creditsRect = { 0,0,1280,720 };

    this->sceneManager = sceneManager;
    currentSelection = SelectedOption::NONE;
}

SceneTitle::~SceneTitle()
{
}

bool SceneTitle::Load(Textures* tex)
{
    backgroundTexture = tex->Load("Assets/Textures/Scenes/title_screen.png");

    font = new Font("Assets/Fonts/happy_school.xml", tex);

    return false;
}

bool SceneTitle::Update(Input* input, float dt)
{
    //if (input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);

    if (currentSelection == SelectedOption::NONE)
    {
        btnStart->Update(input, dt);
        btnContinue->Update(input, dt);
        btnSettings->Update(input, dt);
        btnCredits->Update(input, dt);
        btnExit->Update(input, dt);
    }
    else if (currentSelection == SelectedOption::SETTINGS)
    {

    }

    if (input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
        currentSelection = SelectedOption::NONE;

    return false;
}

bool SceneTitle::Draw(Render* render)
{
    render->DrawTexture(backgroundTexture, 0, 0, &backgroundRect);

    if (currentSelection == SelectedOption::NONE)
    {
        btnStart->Draw(render);
        btnContinue->Draw(render);
        btnSettings->Draw(render);
        btnCredits->Draw(render);
        btnExit->Draw(render);

        render->DrawText(font, "PLAY", 580, 375, 50, 5, { 255,255,255,255 });
        render->DrawText(font, "CONTINUE", 525, 435, 50, 5, { 255,255,255,255 });
        render->DrawText(font, "SETTINGS", 525, 495, 50, 5, { 255,255,255,255 });
        render->DrawText(font, "CREDITS", 550, 555, 50, 5, { 255,255,255,255 });
        render->DrawText(font, "EXIT", 590, 615, 50, 5, { 255,255,255,255 });
    }
    else if (currentSelection == SelectedOption::CONTINUE)
    {

    }
    else if (currentSelection == SelectedOption::SETTINGS)
    {
        render->DrawRectangle(backgroundRect, { 0, 0, 0, 227 });
    }
    else if (currentSelection == SelectedOption::CREDITS)
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
        if (control->id == 1) TransitionToScene(SceneType::GAMEPLAY);
        
        else if (control->id == 4) currentSelection = SelectedOption::CREDITS;
        else if (control->id == 5) sceneManager->menuExitCall = true; // TODO: Exit request
    }
    default: break;
    }

    return true;
}