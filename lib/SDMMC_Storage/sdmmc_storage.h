#ifndef SDMMC_STORAGE_H
#define SDMMC_STORAGE_H

#include "storage.h"

#define MOUNTPOINT "/sdcard"

class SDMMC_Storage : public Storage {
    private:
        int cmd;
        int clk;
        int d0;
        int d1;
        int d2;
        int d3;

        bool onebit;

    public:
        SDMMC_Storage(int sd_clk, int sd_cmd, int sd_d0);
        SDMMC_Storage(int sd_clk, int sd_cmd, int sd_d0, int sd_d1, int sd_d2, int sd_d3);

        bool setup() override;
        bool loop() override;
};

#endif