#include "GuiButton.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(Input* input, float dt)
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

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiButton::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: /*render->DrawRectangle(bounds, { 100, 100, 100, 255 });*/
        render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &whiteButton);
        break;
    case GuiControlState::NORMAL: /*render->DrawRectangle(bounds, { 0, 139, 139, 127 });*/
        render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &greyButton);
        break;
    case GuiControlState::FOCUSED: /*render->DrawRectangle(bounds, { 224, 255, 255, 127 });*/
        render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &yellowButton);
        break;
    case GuiControlState::PRESSED: /*render->DrawRectangle(bounds, { 0, 255, 255, 255 });*/
        render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &brownButton);
        break;
    case GuiControlState::SELECTED: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
        break;
    default:
        break;
    }

    return false;
}