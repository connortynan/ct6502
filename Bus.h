#ifndef BUS_H
#define BUS_H

#include <cstdint>

class Bus {

public:
    Bus() = default;
    ~Bus() = default;


public:
    virtual void write(uint16_t addr, uint16_t data) = 0;
    [[nodiscard]] virtual uint16_t read(uint16_t addr) const = 0;

};

#endif //BUS_H