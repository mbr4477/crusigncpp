#ifndef _COLOR_H_
#define _COLOR_H_
#include "FastLED.h"
#include <string>

CRGB str2color(std::string colorName)
{
    if (colorName == "red")
    {
        return CRGB(255, 0, 0);
    }
    else if (colorName == "blue")
    {
        return CRGB(0, 0, 255);
    }
    else if (colorName == "green")
    {
        return CRGB(0, 255, 0);
    }
    else if (colorName == "yellow")
    {
        return CRGB(255, 255, 0);
    }
    else if (colorName == "magenta")
    {
        return CRGB(255, 0, 255);
    }
    else if (colorName == "cyan")
    {
        return CRGB(0, 255, 255);
    }
    else if (colorName == "white")
    {
        return CRGB(255, 255, 255);
    }
    else if (colorName == "black")
    {
        return CRGB(0, 0, 0);
    }
    return CRGB(0, 0, 0);
}
#endif