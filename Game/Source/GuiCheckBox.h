#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiCheckBox : public GuiControl
{
public:

    GuiCheckBox(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiCheckBox();

    bool Update(Input* input, float dt);
    bool Draw(Render* render);

private:

    // GuiCheckBox specific properties
    // Maybe some animation properties for state change?
    bool checked;

    SDL_Rect greyCheckBox = { 290,0,45,49 };
    SDL_Rect yellowCheckBox = { 293,294,45,49 };
    SDL_Rect brownCheckBox = { 293,343,45,49 };
    SDL_Rect whiteCheckBox = { 293,437,45,49 };

    SDL_Rect brownCross = { 370,0,15,15 };
    SDL_Rect brownTick = { 370,45,16,15 };
    SDL_Rect greyCross = { 370,15,15,15 };
    SDL_Rect greyTick = { 370,30,16,15 };
    SDL_Rect whiteCross = { 370,60,15,15 };
    SDL_Rect whiteTick = { 370,75,16,15 };
    SDL_Rect yellowCross = { 369,169,15,15 };
    SDL_Rect yellowTick = { 369,184,16,15 };
};

#endif // __GUICHECKBOX_H__
