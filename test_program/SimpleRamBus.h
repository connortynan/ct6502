#ifndef CT_VIC_20_SIMPLERAMBUS_H
#define CT_VIC_20_SIMPLERAMBUS_H

#include <array>
#include "../Bus.h"
#include "../ct6502.h"

class SimpleRamBus : public Bus {

public:
    SimpleRamBus() {
        cpu.connectBus(this);

        // Clear RAM contents
        for (auto &i : ram) {
            i = 0x00;
        }
    }
    ~SimpleRamBus()  = default;

public: // Devices connected to bus
    ct6502 cpu;
    std::array<uint16_t, 64 * 1024> ram {};


public:
    void write(uint16_t addr, uint16_t data) override {
        if (addr >= 0x0000 && addr <= 0xFFFF)
            ram[addr] = data;
    }

    [[nodiscard]] uint16_t read(uint16_t addr) const override {
        if (addr >= 0x0000 && addr <= 0xFFFF)
            return ram[addr];
        return 0x00;
    }

};

#endif //CT_VIC_20_SIMPLERAMBUS_H
