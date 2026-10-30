#ifndef STORAGE_H
#define STORAGE_H

class Storage {
    public:
        virtual bool setup() { return true; };

        virtual bool loop() { return true; };
};

#endif