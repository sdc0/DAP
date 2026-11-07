#ifndef DAP_CONFIG_H
#define DAP_CONFIG_H

// ST7789 display settings
#define SCREEN_DIN 5
#define SCREEN_SCLK 6
#define SCREEN_CS 4
#define SCREEN_DC 8
#define SCREEN_RST 9
#define SCREEN_BL 7

// DPad settings
#define DPAD_UP 15
#define DPAD_DOWN 16
#define DPAD_LEFT 17
#define DPAD_RIGHT 18

// Touchwheel settings
#define TOUCHWHEEL_A 1
#define TOUCHWHEEL_B 2
#define TOUCHWHEEL_C 3

// SD MMC settings
#define SD_MMC_CLK 14
#define SD_MMC_CMD 13
#define SD_MMC_D0 39
#define SD_MMC_D1 40
#define SD_MMC_D2 41
#define SD_MMC_D3 42

// SD SPI settings
#define SD_SPI_MISO 39
#define SD_SPI_CLK 40
#define SD_DPI_MOSI 41
#define SD_SPI_CS 42

// I2S Audio settings
#define I2S_DAC_LRC 10
#define I2S_DAC_DIN 11
#define I2S_DAC_BCLK 12

#define MAX(x, y) ((x > y) ? x : y)
#define MIN(x, y) ((x < y) ? x : y)

#endif

#ifndef DAP_GLOBAL_OBJECTS_H
#define DAP_GLOBAL_OBJECTS_H

#include <i2s_audio.h>
#include <sdmmc_storage.h>
#include <touchwheel_input.h>
#include <dpad_input.h>

extern I2S_Audio* i2s;
extern SDMMC_Storage* sdmmc;
extern Touchwheel_Input* tw;
extern DPad_Input* dpad;

#endif