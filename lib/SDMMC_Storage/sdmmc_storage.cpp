#include <sdmmc_storage.h>

#include <Arduino.h>
#include <FS.h>
#include <SD_MMC.h>

SDMMC_Storage::SDMMC_Storage(int sd_clk, int sd_cmd, int sd_d0) {
    cmd = sd_cmd;
    clk = sd_clk;
    d0 = sd_d0;

    onebit = true;
    d1 = -1;
    d2 = -1;
    d3 = -1;
}

SDMMC_Storage::SDMMC_Storage(int sd_clk, int sd_cmd, int sd_d0, int sd_d1, int sd_d2, int sd_d3) {
    cmd = sd_cmd;
    clk = sd_clk;
    d0 = sd_d0;
    d1 = sd_d1;
    d2 = sd_d2;
    d3 = sd_d3;

    onebit = false;
}

bool SDMMC_Storage::setup() {
    if (!SD_MMC.setPins(clk, cmd, d0, d1, d2, d3)) {
        Serial.println("Failed to set pins correctly");
        return false;
    }

    if (!SD_MMC.begin(MOUNTPOINT, onebit, false, 4000)) {
        Serial.println("Failed to mount SD Card");
        return false;
    }

    return true;
}

bool SDMMC_Storage::loop() {
    return true;
}

fs::FS* SDMMC_Storage::getFS() {
    return &SD_MMC;
}