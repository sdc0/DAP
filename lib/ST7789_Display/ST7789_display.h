#ifndef ST7789_DISPALY_H
#define ST7789_DISPLAY_H

#include "display.h"

#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#include "menu.h"

typedef void (*Render_Menu_Func)(Menu* m);
typedef void (*Render_Option_Func)(String text, int index, bool isSelected, bool wasSelected);

class ST7789_Display : public Display {
    private:
        int din;
        int sck;
        int cs;
        int rst;
        int dc;
        int bl;

    public: 
        ST7789_Display(int din, int sck, int cs, int rst, int dc, int bl);

        bool setup() override;
        bool loop() override;
};

class Half_Screen_Renderer : public Renderer {
    private:
        int offset_x;
        int first_offset_y;
        const GFXfont* font;

    public:
        Half_Screen_Renderer(int o_x, int o_y, const GFXfont* f);

        void renderMenu(Menu* m) override;
        void renderOption(String text, bool isSelected, bool wasSelected);
};

#endif