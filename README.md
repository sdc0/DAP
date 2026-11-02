**Note: download zip of ESP32-audioI2S Library version 4.0.1, unzip it into the lib/ directory, and inside it, change lines 905 & 906 in src/Audio.h to:**

```
virtual const char*           getStreamTitle() {return NULL; };
virtual const char*           whoIsIt() {return NULL; };
```

**Otherwise they will cause a linking error since they aren't defined anywhere (for some reason)**



**IMPORTANT NOTE: disregard everything above this for now (leaving just in case I need to use the library later), switched to Phil Schatzmann's arduino-audio-tools git repo cloned into lib/, requiring no changes (other than getting rid of ESP32-audioI2S because of a conflicting RingBuffer definition I think)**