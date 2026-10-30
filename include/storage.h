#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <FS.h>

class Storage {
    public:
        virtual bool setup() { return true; };

        virtual bool loop() { return true; };

        virtual fs::FS* getFS() { return NULL; };
};

#endif