#ifndef TOUCHWHEEL_INPUT_H
#define TOUCHWHEEL_INPUT_H

#include "input.h"

#include <vector>
#include <Arduino.h>

enum Touchwheel_State {
    NONE,
    UP,
    DOWN
};

class Circular_Queue {
    private:
        std::vector<float> queue;
        int head;
        int tail;

        int size;
        int capacity;

    public:
        Circular_Queue(int max_size);

        void enqueue(float val);
        float* dequeue();

        float avg();

        bool isFull() { return capacity == size; };
};

class Touchwheel_Input : public Input {
    private:
        std::vector<int> pins;
        std::vector<float> lows;
        std::vector<float> highs;

        Circular_Queue current;
        Circular_Queue prev;
        float radial_threshold;
        Touchwheel_State state = NONE;

        float wheel_pos();
        void calibrate();

    public:
        Touchwheel_Input(std::vector<int> passed_pins, int num_samples);

        bool setup();
        bool loop();
};

#endif