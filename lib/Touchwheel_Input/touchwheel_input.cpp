#include <touchwheel_input.h>

Touchwheel_Input::Touchwheel_Input(std::vector<int> passed_pins) {
    radial_threshold = 30;
    pins = passed_pins;
    prev = -1;
}

bool Touchwheel_Input::loop() {

    float pos = wheel_pos();
    //Serial.println(pos);

    if (pos < 0) {
        prev = -1;
    }else if (prev < 0) {
        prev = pos;
    }else {
        float delta = atan2(sin(radians(pos - prev)), cos(radians(pos - prev))) * 180 / PI;
        //Serial.printf("%.2f => %.2f (%.2f)\n", prev, pos, delta);

        if (delta > radial_threshold) {
            inputs.push(Input_Type::GESTURE_DOWN);

            prev = pos;
        }else if (delta < -radial_threshold) {
            inputs.push(Input_Type::GESTURE_UP);

            prev = pos;
        }
    }

    return true;
}

float Touchwheel_Input::wheel_pos() {
    // TODO: implement dynamic pin reading for >3 pin touchwheels
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

    //Serial.printf("A: %.2f, B: %.2f, C: %.2f, degrees: %f", a, b, c, degrees);

    return degrees;
}