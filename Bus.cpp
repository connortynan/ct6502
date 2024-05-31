#include "Bus.h"

Bus::Bus() {
    cpu.connectBus(this);

    // Clear RAM contents
    for (auto &i : ram) {
        i = 0x00;
    }
};

Bus::~Bus()  {}

void Bus::write(uint16_t addr, uint16_t data) {
    if (addr >= 0x0000 && addr <= 0xFFFF)
        ram[addr] = data;
}

uint16_t Bus::read(uint16_t addr) const {
    if (addr >= 0x0000 && addr <= 0xFFFF)
        return ram[addr];
    return 0x00;
}