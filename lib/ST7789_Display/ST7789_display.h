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
        int din, sck, cs, rst, dc, bl;

        Arduino_DataBus* bus;
        Arduino_GFX* display;

        int screen_height, screen_width;

    public: 
        Arduino_GFX* canvas;

        ST7789_Display(int din, int sck, int cs, int rst, int dc, int bl, int height, int width);

        bool setup() override;
        bool loop() override;
};

class Half_Screen_Renderer : public Renderer {
    private:
        int offset_x;
        int first_offset_y;
        int bezel_height;
        const GFXfont* font;

        Arduino_GFX* canvas;

    public:
        Half_Screen_Renderer(int o_x, int o_y, int b, const GFXfont* f, Arduino_GFX* g);

        void renderMenu(Menu* m) override;
        void renderOption(String text, int index, bool isSelected, bool wasSelected);
};

#endif