#include <Arduino.h>
#include <SD_MMC.h>

#include "dap_config.h"

#include <sdmmc_storage.h>
#include <i2s_audio.h>

SDMMC_Storage* sd;
I2S_Audio* dac;

//Audio audio;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    sd = new SDMMC_Storage(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0, SD_MMC_D1, SD_MMC_D2, SD_MMC_D3);
    if (!sd->setup()) {
        Serial.println("FAiled somewhere in SD Card setup");
        while(1);
    }

    dac = new I2S_Audio(I2S_DAC_BCLK, I2S_DAC_LRC, I2S_DAC_DIN, SD_MMC);
    if (!dac->setup()) {
        Serial.println("Failed somewhere in Audio setup");
        while(1);
    }

    /*if (!SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0, SD_MMC_D1, SD_MMC_D2, SD_MMC_D3)) {
        Serial.println("1");
        while(1);
    }
    if (!SD_MMC.begin("/sdcard", false, false, 4000)) {
        Serial.println("2");
        while(1);
    }
    if (!audio.setPinout(I2S_DAC_BCLK, I2S_DAC_LRC, I2S_DAC_DIN)) {
        Serial.println("3");
        while(1);
    }
    audio.setVolume(5);
    audio.connecttoFS(SD_MMC, "/Music/test.mp3");*/

    Serial.println("Setup successful");

    //pinMode(7, OUTPUT);
}

void loop() {
    dac->loop();
    //audio.loop();
}