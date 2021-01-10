#include "GuiCheckBox.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = text;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(Input* input, float dt)
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
                checked = !checked;
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    if (input->GetKey(SDL_SCANCODE_F8) == KeyState::KEY_DOWN)
    {
        debug = !debug;
    }

    return false;
}

bool GuiCheckBox::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
        /*if (checked) render->DrawRectangle(bounds, { 100, 100, 100, 255 });
        else render->DrawRectangle(bounds, { 100, 100, 100, 255 });*/
        break;
    case GuiControlState::NORMAL: render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &greyCheckBox);
        if (checked)
        {
            render->scale = 2;
            render->DrawTextureWithoutCamera(texture, bounds.x + 7, bounds.y + 7, &whiteTick);
            render->scale = 1;
        }
        else
        {
            render->scale = 2;
            render->DrawTextureWithoutCamera(texture, bounds.x + 7, bounds.y + 7, &whiteCross);
            render->scale = 1;
        }
        if (debug)
            render->DrawRectangleWithoutCamera(bounds, { 0, 255, 0, 127 });
        break;
    case GuiControlState::FOCUSED: render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &yellowCheckBox);
        if (checked)
        {
            render->scale = 2;
            render->DrawTextureWithoutCamera(texture, bounds.x + 7, bounds.y + 7, &whiteTick);
            render->scale = 1;
        }
        else
        {
            render->scale = 2;
            render->DrawTextureWithoutCamera(texture, bounds.x + 7, bounds.y + 7, &whiteCross);
            render->scale = 1;
        }
        if (debug)
            render->DrawRectangle(bounds, { 255, 255, 0, 127 });
        break;
    case GuiControlState::PRESSED: render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &brownCheckBox);
        if (checked)
        {
            render->scale = 1.5;
            render->DrawTextureWithoutCamera(texture, bounds.x + 11, bounds.y + 11, &whiteTick);
            render->scale = 1;
        }
        else
        {
            render->scale = 1.5;
            render->DrawTextureWithoutCamera(texture, bounds.x + 11, bounds.y + 11, &whiteCross);
            render->scale = 1;
        }
        if (debug)
            render->DrawRectangleWithoutCamera(bounds, { 0, 255, 255, 127 });
        break;
    case GuiControlState::SELECTED: /*render->DrawRectangle(bounds, { 0, 255, 0, 255 });*/
        break;
    default:
        break;
    }

    return false;
}
