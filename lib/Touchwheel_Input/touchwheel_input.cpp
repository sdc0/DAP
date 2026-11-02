#include <touchwheel_input.h>

#include <cmath>

Circular_Queue::Circular_Queue(int max_size) : queue(max_size) {
    head = 0;
    tail = 0;

    size = 0;
    capacity = max_size;
}

void Circular_Queue::enqueue(float val) {
    queue[tail] = val;
    if (size > 0 && head == tail) head = (head + 1) % capacity;
    else size++;
    tail = (tail + 1) % capacity;
}

float* Circular_Queue::dequeue() {
    if (size == 0) return NULL;

    float* temp = &queue[head];
    head = (head + 1) % capacity;
    size--;

    return temp;
}

float Circular_Queue::avg() {
    if (size == 0) return 0;

    float sin_sum = 0.0;
    float cos_sum = 0.0;

    for (int i = 0; i < size; i++) {
        sin_sum += std::sin(queue[(head + i) % capacity] * PI / 180);
        cos_sum += std::cos(queue[(head + i) % capacity] * PI / 180);
    }

    return fmod(360.0 + std::atan2(sin_sum / size, cos_sum / size) * 180 / PI, 360.0);
}

Touchwheel_Input::Touchwheel_Input(std::vector<int> passed_pins, int num_samples=10) : current(num_samples), prev(num_samples / 2), lows(passed_pins.size()), highs(passed_pins.size()) {
    radial_threshold = 30; // roughly 30 degrees
    pins = passed_pins;
    for (int i = 0; i < passed_pins.size(); i++) {
        lows[i] = 1000000;
        highs[i] = 0;
    }
}

bool Touchwheel_Input::setup() {
    //calibrate();

    return true;
}

bool Touchwheel_Input::loop() {

    float pos = wheel_pos(); // scale to degrees
    Serial.println();
    //Serial.println(pos);

    /*if (pos < 0) {
        while (prev.dequeue() != NULL);
        while (current.dequeue() != NULL);
    }else if (!prev.isFull()) {
        prev.enqueue(pos);
    }else if (!current.isFull()) {
        current.enqueue(pos);
    }else if (fmod(360.0 + prev.avg() - current.avg(), 360.0) > radial_threshold && fmod(360.0 + prev.avg() - current.avg(), 360.0) < 360 - 2 * radial_threshold) {
        Serial.printf("UP: %f -> %f (%f)\n", prev.avg(), current.avg(), fmod(360.0 + prev.avg() - current.avg(), 360.0));

        while (prev.dequeue() != NULL);
        while (current.dequeue() != NULL);
    }else if (fmod(360.0 + current.avg() - prev.avg(), 360.0) > radial_threshold && fmod(360.0 + current.avg() - prev.avg(), 360) < 360 - 2 * radial_threshold) {
        Serial.printf("DOWN: %f -> %f (%f)\n", prev.avg(), current.avg(), fmod(360.0 + current.avg() - prev.avg(), 360.0));

        while (prev.dequeue() != NULL);
        while (current.dequeue() != NULL);
    }else {
        current.enqueue(pos);
    }*/

    /*if (pos < 0) { // finger lifted, empty prev queue
        while (prev.dequeue() != NULL);
    }else if (!prev.isFull()) { // initially fill the moving average queue
        prev.enqueue(pos);
    }else if (state == NONE) { // no spike has been detected, check for first spike
        if (fmod(360.0 + prev.avg() - pos, 360.0) > radial_threshold && fmod(360.0 + prev.avg() - pos, 360.0) < 360 - radial_threshold) {
            current.enqueue(pos);
            state = UP;
        }else if (fmod(360.0 + pos - prev.avg(), 360.0) > radial_threshold && fmod(360.0 + pos - prev.avg(), 360.0) < 360 - radial_threshold) {
            current.enqueue(pos);
            state = DOWN;
        }else { // no spike
            prev.enqueue(pos);
        }
    }else if (state == UP && fmod(360.0 + prev.avg() - pos, 360.0) > radial_threshold && fmod(360.0 + prev.avg() - pos, 360.0) < 360 - radial_threshold) {
        if (current.isFull()) { // sufficient number of spikes, trigger UP event and fill prev queue
            Serial.println("UP");

            float* back = current.dequeue();
            while (back != NULL) {
                prev.enqueue(*back);
                back = current.dequeue();
            }
            state = NONE;
        }else { // insufficient number of spikes above threshold, store more
            current.enqueue(pos);
        }
    }else if (state == DOWN && fmod(360.0 + pos - prev.avg(), 360.0) > radial_threshold && fmod(360.0 + pos - prev.avg(), 360.0) < 360 - radial_threshold) {
        if (current.isFull()) { // sufficient number of spikes, trigger DOWN event and fill prev queue
            Serial.println("DOWN");

            float* back = current.dequeue();
            while (back != NULL) {
                prev.enqueue(*back);
                back = current.dequeue();
            }
            state = NONE;
        }else { // insufficient number of spikes above threshold, store more
            current.enqueue(pos);
        }
    }else { // a spike has occurred, but the current is not a spike, so reset spike progress and dump extras to prev queue
        float* back = current.dequeue();
        while (back != NULL) {
            prev.enqueue(*back);
            back = current.dequeue();
        }
        state = NONE;
    }*/

    return true;
}

void Touchwheel_Input::calibrate() {
    Serial.println("Don't touch for a little");
    long now = millis();
    while (millis() - now < 5000) {
        for (int i = 0; i < 3; i++) {
            float val = touchRead(pins[i]);
            if (val < lows[i]) lows[i] = val;
        }
    }

    Serial.println("Now make a full circle");
    now = millis();
    while (millis() - now < 5000) {
        for (int i = 0; i < 3; i++) {
            float val = touchRead(pins[i]);
            if (val > highs[i]) highs[i] = val;
        }
    }
}

float Touchwheel_Input::wheel_pos() {
    /*float a = ((float)touchRead(pins[0]) - lows[0]) / (highs[0] - lows[0]);
    float b = ((float)touchRead(pins[1]) - lows[1]) / (highs[1] - lows[1]);
    float c = ((float)touchRead(pins[2]) - lows[2]) / (highs[2] - lows[2]);

    if (a < 0.1) a = 0;
    if (b < 0.1) b = 0;
    if (c < 0.1) c = 0;

    if (a + b + c < 0.1) return -1.0f;

    float x = a * sin(radians(300)) + b * sin(radians(60)) + c * sin(radians(180));
    float y = a * cos(radians(300)) + b * cos(radians(60)) + c * cos(radians(180));

    float degrees = atan2(x, y) * 180 / PI;
    if (degrees < 0) degrees += 360;

    Serial.printf("A: %.2f, B: %.2f, C: %.2f, degrees: %f\n", a, b, c, degrees);

    return degrees;*/

    // dynamic calibration code for normalizing on (assumption of) sinusoidal proximities
    float a = touchRead(pins[0]);
    float b = touchRead(pins[1]);
    float c = touchRead(pins[2]);

    float min = (a < b) ? ((a < c) ? a : ((b < c) ? b : c)) : ((b < c) ? b : c);
    a -= min;
    b -= min;
    c -= min;

    if (a < min * 0.3 && b < min * 0.3 && c < min * 0.3) return -1.0; // not being touched

    float sum = a + b + c;
    a /= sum;
    b /= sum;
    c /= sum;

    float x = a * cos(radians(300)) + b * cos(radians(60)) + c * cos(radians(180));
    float y = a * sin(radians(300)) + b * sin(radians(60)) + c * sin(radians(180));

    float degrees = atan2(y, x) * 180 / PI;
    if (degrees < 0) degrees += 360;

    Serial.printf("A: %.2f, B: %.2f, C: %.2f, degrees: %f", a, b, c, degrees);

    return degrees;
}