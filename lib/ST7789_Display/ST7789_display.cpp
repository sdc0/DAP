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

Half_Screen_Renderer::Half_Screen_Renderer(int o_x, int b, const GFXfont* f, Arduino_GFX* g) {
    offset_x = o_x;
    bezel_height = b;

    font = f;
    canvas = g;

    int16_t x1, x2;
    uint16_t w;
    canvas->setFont(f);
    canvas->getTextBounds("Qq", 0, 0, &x1, &x2, &w, &text_height);

    max_rows = (canvas->height() - b) / (text_height * 1.5);
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

    int posy = bezel_height + 1.5 * text_height * index;

    if (wasSelected || isSelected) canvas->fillRect(0, posy, canvas->width() / 2, text_height * 1.5, (isSelected) ? RGB565_LIGHTBLUE : RGB565_WHITE);
    canvas->setCursor(offset_x, posy + text_height);
    canvas->setTextColor(isSelected ? RGB565_WHITE : RGB565_BLACK);
    canvas->print(text);
}