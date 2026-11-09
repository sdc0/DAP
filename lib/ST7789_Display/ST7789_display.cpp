#include <ST7789_display.h>

ST7789_Display::ST7789_Display(int din, int sck, int cs, int rst, int dc, int bl, int height, int width) {
    ST7789_Display::din = din;
    ST7789_Display::sck = sck;
    ST7789_Display::cs = cs;
    ST7789_Display::rst = rst;
    ST7789_Display::dc = dc;
    ST7789_Display::bl = bl;

    screen_height = height;
    screen_width = width;
}

bool ST7789_Display::setup() {
    bus = new Arduino_ESP32SPI(dc, cs, sck, din, -1, 1);
    display = new Arduino_ST7789(bus, rst, 1, true, screen_height, screen_width, 0, 0);
    canvas = new Arduino_Canvas(screen_width, screen_height, display);

    display->begin();

    canvas->begin();
    canvas->setTextWrap(false);
    canvas->flush();

    return true;
}

bool ST7789_Display::loop() {
    canvas->flush();
    return true;
}

Half_Screen_Renderer::Half_Screen_Renderer(int o_x, int o_y, int b, const GFXfont* f, Arduino_GFX* g) {
    offset_x = o_x;
    first_offset_y = o_y;
    bezel_height = b;

    font = f;
    canvas = g;
}

void Half_Screen_Renderer::renderMenu(Menu* m) {
    canvas->fillScreen(RGB565_WHITE);
    canvas->drawRect(0, 0, canvas->width() / 2 + 1, canvas->height(), RGB565_BLACK);
    canvas->fillRect(0, 0, canvas->width() / 2, bezel_height, RGB565_GREY);

    for (int i = 0; i < m->getOptionsLength(); i++) {
        renderOption(m->getOption(i)->text, i, (m->current_option == i), false);
    }
}

void Half_Screen_Renderer::renderOption(String text, int index, bool isSelected, bool wasSelected) {
    canvas->setFont(font);

    int16_t x1, x2;
    uint16_t w, h;
    canvas->getTextBounds("Qq", 0, 0, &x1, &x2, &w, &h);

    int posy = first_offset_y + 1.5 * h * index;

    if (wasSelected || isSelected) canvas->fillRect(0, posy, canvas->width() / 2, h * 1.5, (isSelected) ? RGB565_LIGHTBLUE : RGB565_WHITE);
    canvas->setCursor(offset_x, posy + h);
    canvas->setTextColor(isSelected ? RGB565_WHITE : RGB565_BLACK);
    canvas->print(text);
}