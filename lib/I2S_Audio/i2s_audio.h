#ifndef I2S_AUDIO_H
#define I2S_AUDIO_H

#include "audio.h"

#include <FS.h>

class I2S_Audio : public DAC_Audio {
    private:
        int bclk;
        int lrc;
        int din;

        fs::FS& filesystem;

    public:
        I2S_Audio(int i2s_bclk, int i2s_lrc, int i2s_din, fs::FS& mounted_filesystem);

        bool setup() override;
        bool playFile(String path) override;
        bool loop() override;
};

#endif