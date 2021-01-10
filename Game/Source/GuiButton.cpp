#include "GuiButton.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;

    isFocusing = false;
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(Input* input, float dt, AudioManager* audio, int hoverFx, int clickFx)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (!isFocusing)
            {
                isFocusing = true;
                audio->PlayFx(hoverFx);
            }

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
            {
                state = GuiControlState::PRESSED;
                audio->PlayFx(clickFx);
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                NotifyObserver();
            }
        }
        else
        {
            state = GuiControlState::NORMAL;
            isFocusing = false;
        }
    }

    if (input->GetKey(SDL_SCANCODE_F8) == KeyState::KEY_DOWN)
    {
        debug = !debug;
    }

    return false;
}

bool GuiButton::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &whiteButton);
        if (debug)
            render->DrawRectangleWithoutCamera(bounds, { 100, 100, 100, 127 });
        break;
    case GuiControlState::NORMAL: render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &greyButton);
        if (debug)
            render->DrawRectangleWithoutCamera(bounds, { 0, 255, 0, 127 });
        break;
    case GuiControlState::FOCUSED: render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &yellowButton);
        if (debug)
            render->DrawRectangleWithoutCamera(bounds, { 255, 255, 0, 127 });
        break;
    case GuiControlState::PRESSED: render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &brownButton);
        if (debug)
            render->DrawRectangle(bounds, { 0, 255, 255, 127 });
        break;
    case GuiControlState::SELECTED: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
        break;
    default:
        break;
    }

    return false;
}