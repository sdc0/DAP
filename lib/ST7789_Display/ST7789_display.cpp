#include <ST7789_display.h>

ST7789_Display::ST7789_Display(int din, int sck, int cs, int rst, int dc, int bl) {
    ST7789_Display::din = din;
    ST7789_Display::sck = sck;
    ST7789_Display::cs = cs;
    ST7789_Display::rst = rst;
    ST7789_Display::dc = dc;
    ST7789_Display::bl = bl;
}

