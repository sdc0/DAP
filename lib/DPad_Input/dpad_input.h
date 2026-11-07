#ifndef DPAD_INPUT_H
#define DPAD_INPUT_H

#include "input.h"

#define INPUT_TIMEOUT 200

class DPad_Input : public Input {
    private:
        int up, down, left, right;
        long last_trigger;

    public:
        DPad_Input(int u, int d, int l, int r);

        bool setup() override;
        bool loop() override;
};

#endif