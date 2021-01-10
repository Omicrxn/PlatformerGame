#include "GuiSlider.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;

    minValue = 0;
    maxValue = 128;
    barWidth = 300;

    leftLimit = 550;
    rightLimit = 850;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        int motionX, motionY;
        input->GetMouseMotion(motionX, motionY);

        unit = barWidth / maxValue;
        value = (mouseX - leftLimit) / unit;
        value = round(value);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            // TODO.
            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                NotifyObserver();
            }
        }
        else if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KeyState::KEY_UP)
        {
            state = GuiControlState::NORMAL;
        }

        if (state == GuiControlState::PRESSED && motionX != 0)
        {
            bounds.x = mouseX - (bounds.w / 2);
            NotifyObserver();
        }

        if (bounds.x < leftLimit)
        {
            bounds.x = leftLimit;
        }
        if (bounds.x >= rightLimit)
        {
            bounds.x = rightLimit;
        }

        if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
            state = GuiControlState::NORMAL;
    }

    if (input->GetKey(SDL_SCANCODE_F8) == KeyState::KEY_DOWN)
    {
        debug = !debug;
    }

    return false;
}

bool GuiSlider::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: /*render->DrawRectangle(bounds, { 100, 100, 100, 255 });*/
        break;
    case GuiControlState::NORMAL: render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &greySlider);
        if (debug)
            render->DrawRectangle(bounds, { 0, 255, 0, 127 });
        break;
    case GuiControlState::FOCUSED: render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &yellowSlider);
        if (debug)
            render->DrawRectangle(bounds, { 255, 255, 0, 127 });
        break;
    case GuiControlState::PRESSED: render->DrawTextureWithoutCamera(texture, bounds.x, bounds.y, &brownSlider);
        if (debug)
            render->DrawRectangle(bounds, { 0, 255, 255, 127 });
        break;
    case GuiControlState::SELECTED: /*render->DrawRectangle(bounds, { 0, 255, 0, 255 });*/
        break;
    default:
        break;
    }

    return false;
}

int GuiSlider::GetValue()
{
    return value;
}