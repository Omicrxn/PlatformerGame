#include "SceneTitle.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

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
    creditsTexture = tex->Load("Assets/Textures/Scenes/credits_screen.png");

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

    if (input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
        currentSelection = SelectedOption::NONE;

    return false;
}

bool SceneTitle::Draw(Render* render)
{
    if (currentSelection == SelectedOption::NONE)
    {
        render->DrawTexture(backgroundTexture, 0, 0, &backgroundRect);

        btnStart->Draw(render);
        btnContinue->Draw(render);
        btnSettings->Draw(render);
        btnCredits->Draw(render);
        btnExit->Draw(render);
    }
    else if (currentSelection == SelectedOption::CONTINUE)
    else if (currentSelection == SelectedOption::CREDITS)
        render->DrawTexture(creditsTexture, 0, 0, &creditsRect);

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