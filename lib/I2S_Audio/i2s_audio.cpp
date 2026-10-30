#include <i2s_audio.h>

#include <Audio.h>

Audio audio;

I2S_Audio::I2S_Audio(int i2s_bclk, int i2s_lrc, int i2s_din, fs::FS& mounted_filesystem) : filesystem(mounted_filesystem) {
    bclk = i2s_bclk;
    lrc = i2s_lrc;
    din = i2s_din;
}

bool I2S_Audio::setup() {
    if (!audio.setPinout(bclk, lrc, din)) {
        Serial.println("Failed to set I2S pins");
        return false;
    }

    audio.setVolume(5);

    return playFile("/Music/test.mp3");

    return true;
}

bool I2S_Audio::playFile(String path) {
    return audio.connecttoFS(filesystem, path.c_str());
}

bool I2S_Audio::loop() {
    audio.loop();

    return true;
}
