#ifndef DISPLAY_H
#define DISPLAY_H

class Display {
    public:
        virtual bool setup() { return true; };

        virtual bool loop() { return true; };
};

#endif