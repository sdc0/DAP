#include <dpad_input.h>

DPad_Input::DPad_Input(int u, int d, int l, int r) {
    up = u;
    down = d;
    left = l;
    right = r;

    last_trigger = millis() - INPUT_TIMEOUT;
}

bool DPad_Input::setup() {
    pinMode(up, INPUT);
    pinMode(down, INPUT);
    pinMode(left, INPUT);
    pinMode(right, INPUT);

    return true;
}

bool DPad_Input::loop() {
    if (millis() - last_trigger > INPUT_TIMEOUT) {
        if (digitalRead(up)) {
            inputs.push(Input_Type::BTN_UP);
            last_trigger = millis();
        }

        if (digitalRead(down)) {
            inputs.push(Input_Type::BTN_DOWN);
            last_trigger = millis();
        }

        if (digitalRead(left)) {
            inputs.push(Input_Type::BTN_LEFT);
            last_trigger = millis();
        }

        if (digitalRead(right)) {
            inputs.push(Input_Type::BTN_RIGHT);
            last_trigger = millis();
        }
    }

    return true;
}