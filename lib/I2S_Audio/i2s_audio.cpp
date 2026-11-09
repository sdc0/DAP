#include <i2s_audio.h>

#include <AudioTools/AudioCodecs/CodecMP3Helix.h>
#include <Arduino.h>
#include <FS.h>

// TODO: fix decoder so that multiple formats can be read (ex. FLAC, WAV, ...)
I2S_Audio::I2S_Audio(int i2s_bclk, int i2s_lrc, int i2s_din, fs::FS* mounted_filesystem) : vol(dac), decoder(&vol, new MP3DecoderHelix()) {
    bclk = i2s_bclk;
    lrc = i2s_lrc;
    din = i2s_din;

    filesystem = mounted_filesystem;
}

bool I2S_Audio::setup() {
    auto config = dac.defaultConfig(TX_MODE);
    config.pin_bck = bclk;
    config.pin_ws = lrc;
    config.pin_data = din;
    dac.begin(config);

    auto vcfg = vol.defaultConfig();
    vcfg.copyFrom(config);
    vol.begin(vcfg);
    vol.setVolume(0.01);

    decoder.begin();

    return playFile("/Music/test.mp3");

    return true;
}

bool I2S_Audio::playFile(String path) {
    audioFile = filesystem->open(path);
    copier.begin(decoder, audioFile);

    return true;
}

bool I2S_Audio::loop() {
    return copier.copy();
}

void I2S_Audio::setVolume(float volume) {
    vol.setVolume(volume);
}