#ifndef INPUT_H
#define INPUT_H

class Input {
    public:
        virtual bool setup() { return true; };

        virtual bool loop() { return true; };
};

#endif