#ifndef TOUCHWHEEL_INPUT_H
#define TOUCHWHEEL_INPUT_H

#include "input.h"

#include <vector>
#include <Arduino.h>

#include "audio.h"

class Touchwheel_Input : public Input {
    private:
        std::vector<int> pins;

        float current;
        float prev;
        float radial_threshold;

        float wheel_pos();

    public:
        Touchwheel_Input(std::vector<int> passed_pins);

        bool loop() override;
};

#endif