#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include <map>
#include <queue>

class Input {
    public:
        virtual bool setup() { return true; };

        virtual bool loop() { return true; };
};

#define INPUT_TYPES(x) \
    x(BTN_UP) \
    x(BTN_DOWN) \
    x(BTN_LEFT) \
    x(BTN_RIGHT) \
    x(GESTURE_UP) \
    x(GESTURE_DOWN) \
    x(SELECT_PRESS) \
    x(SELECT_HOLD)

#define DEF_ENUM(x) x,
#define DEF_MAP(x) { Input_Type::x, []() { Serial.println(x); } },

typedef void (*Input_Trigger)();

enum Input_Type {
    INPUT_TYPES(DEF_ENUM)
};

extern std::map<Input_Type, Input_Trigger> input_mapping;
extern std::queue<Input_Type> inputs;

void handleInputs();

#endif