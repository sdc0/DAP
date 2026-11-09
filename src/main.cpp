#include <Arduino.h>
#include <SD_MMC.h>

#include "dap_config.h"

#include <sdmmc_storage.h>
#include <i2s_audio.h>
#include <touchwheel_input.h>
#include <dpad_input.h>
#include <ST7789_display.h>

#include "MyriadProRegular9pt7b.h"
#include "MyriadProBold9pt7b.h"

void secondCoreLoop(void*);

SDMMC_Storage* sd;
I2S_Audio* dac;
ST7789_Display* screen;
Half_Screen_Renderer* half;
Touchwheel_Input* tw;
DPad_Input* dpad;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    pinMode(LED_BUILTIN, OUTPUT);

    sd = new SDMMC_Storage(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0, SD_MMC_D1, SD_MMC_D2, SD_MMC_D3);
    if (!sd->setup()) {
        Serial.println("Failed somewhere in SD Card setup");
        while(1);
    }

    dac = new I2S_Audio(I2S_DAC_BCLK, I2S_DAC_LRC, I2S_DAC_DIN, sd->getFS());
    if (!dac->setup()) {
        Serial.println("Failed somewhere in Audio setup");
        while(1);
    }

    screen = new ST7789_Display(SCREEN_DIN, SCREEN_SCLK, SCREEN_CS, SCREEN_RST, SCREEN_DC, SCREEN_BL, 240, 320);
    if (!screen->setup()) {
        Serial.println("Failed somewhere in display setup");
        while(1);
    }

    half = new Half_Screen_Renderer(5, 15, 15, &MyriadProBold9pt7b, screen->canvas);
    root_menu = setupMainMenu();
    current_menu = root_menu;

    half->renderMenu(current_menu);

    tw = new Touchwheel_Input(std::vector<int>{TOUCHWHEEL_A, TOUCHWHEEL_B, TOUCHWHEEL_C});
    if (!tw->setup()) {
        Serial.println("Failed somewhere in touchwheel setup");
        while(1);
    }

    dpad = new DPad_Input(DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT);
    if (!dpad->setup()) {
        Serial.println("Failed somewhere in dpad setup");
        while(1);
    }

    // setup input mappings
    input_mapping[BTN_UP] = []() {
        current_menu->moveSelectedUp();
        half->renderMenu(current_menu);
    };
    input_mapping[BTN_DOWN] = []() {
        current_menu->moveSelectedDown();
        half->renderMenu(current_menu);
    };

    input_mapping[BTN_RIGHT] = []() {
        // enter submenu OR call function
        Menu_Option* o = current_menu->getOption(current_menu->current_option);

        if (o->type == STATIC_NESTING) {
            o->child->prev = current_menu;
            current_menu = o->child;
        }else if (o->type == DYNAMIC_NESTING) {
            o->child = o->gen(o, current_menu);
            o->child->prev = current_menu;
            current_menu = o->child;
        }else {
            o->func(o);
        }

        half->renderMenu(current_menu);
    };

    input_mapping[BTN_LEFT] = []() {
        if (current_menu->prev == NULL) return;

        current_menu = current_menu->prev;
        Menu_Option* o = current_menu->getOption(current_menu->current_option);
        if (o->type == DYNAMIC_NESTING) {
            delete o->child;
            o->child = NULL;
        }

        half->renderMenu(current_menu);
    };

    // setup multithreading for more stable audio
    Serial.println(xTaskCreatePinnedToCore(
        secondCoreLoop,
        "audio_thread",
        64 * 1024,
        NULL,
        24,
        NULL,
        0
    ));

    Serial.println("Setup successful");
}

void loop() {
    tw->loop();
    dpad->loop();

    handleInputs();

    screen->loop();
}

void secondCoreLoop(void* pvParameters) {
    while(1) {
        dac->loop();
    }
}