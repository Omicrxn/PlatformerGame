#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiSlider : public GuiControl
{
public:

    GuiSlider(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiSlider();

    bool Update(Input* input, float dt);
    bool Draw(Render* render);

    int GetValue();

private:

    // GuiSlider specific properties
    // Maybe some animation properties for state change?
    SDL_Rect slider = { 0,0,0,0 };
    int value = 0;

    int minValue = 0;
    int maxValue = 0;
    int barWidth = 0;
    int unit = 0;

    int leftLimit = 0;
    int rightLimit = 0;

    SDL_Rect whiteSlider = { 335,0,35,35 };
    SDL_Rect greySlider = { 335,38,35,35 };
    SDL_Rect yellowSlider = { 335,76,35,35 };
    SDL_Rect brownSlider = { 335,114,35,35 };

    bool debug = false;
};

#endif // __GUISLIDER_H__