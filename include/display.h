#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "menu.h"

class Display {
    public:
        virtual bool setup() { return true; };

        virtual bool loop() { return true; };
};

class Renderer {
    public:
        virtual void renderMenu(Menu*) { };
        virtual void renderOption(String text, int index, bool isSelected, bool wasSelected) { };
};

#endif