#ifndef AUDIO_H
#define AUDIO_H

#include <Arduino.h>

class DAC_Audio {
    public:
        virtual bool setup() { return true; };

        virtual bool playFile(String path) { return true; };

        virtual bool loop() { return true; };
};

#endif