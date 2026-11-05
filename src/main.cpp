#include <Arduino.h>
#include <SD_MMC.h>

#include "dap_config.h"

#include <sdmmc_storage.h>
#include <i2s_audio.h>
#include <touchwheel_input.h>

SDMMC_Storage* sd;
I2S_Audio* dac;
Touchwheel_Input* tw;

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

    tw = new Touchwheel_Input(std::vector<int>{TOUCHWHEEL_A, TOUCHWHEEL_B, TOUCHWHEEL_C});
    if (!tw->setup()) {
        Serial.println("Failed somewhere in touchwheel setup");
        while(1);
    }

    Serial.println("Setup successful");
}

void loop() {
    dac->loop();
    tw->loop();

    handleInputs();
}