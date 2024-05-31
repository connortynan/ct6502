#ifndef BUS_H
#define BUS_H

#include <cstdint>

class Bus {

public:
    Bus();
    ~Bus();

public: // Devices connected to bus
    ct6502

public:
    void write(uint16_t addr, uint16_t data);
    uint16_t read(uint16_t addr);

};




#endif //BUS_H