#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include <array>

#include "ct6502.h"

class Bus {

public:
    Bus();
    ~Bus();

public: // Devices connected to bus
    ct6502 cpu;
    std::array<uint16_t, 64 * 1024> ram;

public:
    void write(uint16_t addr, uint16_t data);
    uint16_t read(uint16_t addr) const;

};




#endif //BUS_H