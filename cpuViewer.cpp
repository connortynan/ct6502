#include <string>
#include <iostream>
#include <sstream>
#include <conio.h>
#include <windows.h>
#include <fstream>

#include "Bus.h"

#define DEFAULT_TEXT "\033[39m\033[49m"
#define RED   "\033[38;2;255;0;0m"
#define GREEN "\033[38;2;0;255;0m"

class demo_6502 {

public:
    Bus bus;

    static std::string hex(uint32_t n, uint8_t d) {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4)
            s[i] = "0123456789ABCDEF"[n & 0xF];
        return s;
    }

    void PrintRam(uint16_t nAddr, int nRows, int nColumns) const {
        std::cout << DEFAULT_TEXT;
        for (int row = 0; row < nRows; row++)
        {
            std::string sOffset = "$" + hex(nAddr, 4) + ":";
            for (int col = 0; col < nColumns; col++)
            {
                sOffset += " " + hex(bus.read(nAddr), 2);
                nAddr += 1;
            }
            std::cout << sOffset << std::endl;
        }
    }

    void PrintCpu() const {
        std::cout << DEFAULT_TEXT << "Status: ";
        std::cout << (( bus.cpu.status & ct6502::NEGATIVE  ) ? GREEN : RED) << "N" ;
        std::cout << (( bus.cpu.status & ct6502::OVERFLOW  ) ? GREEN : RED) << "V" ;
        std::cout << (( bus.cpu.status & ct6502::CONSTANT  ) ? GREEN : RED) << "-" ;
        std::cout << (( bus.cpu.status & ct6502::BREAK     ) ? GREEN : RED) << "B" ;
        std::cout << (( bus.cpu.status & ct6502::DECIMAL   ) ? GREEN : RED) << "D" ;
        std::cout << (( bus.cpu.status & ct6502::INTERRUPT ) ? GREEN : RED) << "I" ;
        std::cout << (( bus.cpu.status & ct6502::ZERO      ) ? GREEN : RED) << "Z" ;
        std::cout << (( bus.cpu.status & ct6502::CARRY     ) ? GREEN : RED) << "C" ;
        std::cout << DEFAULT_TEXT << std::endl;

        std::cout << "PC: $" << hex(bus.cpu.pc, 4) << std::endl;
        std::cout << "A: $" << hex(bus.cpu.a, 2) << "  [" << std::to_string(bus.cpu.a) << "]" << std::endl;
        std::cout << "X: $" << hex(bus.cpu.x, 2) << "  [" << std::to_string(bus.cpu.x) << "]" << std::endl;
        std::cout << "Y: $" << hex(bus.cpu.y, 2) << "  [" << std::to_string(bus.cpu.y) << "]" << std::endl;
        std::cout << "Stack P: $" << hex(bus.cpu.stk_ptr, 4) << std::endl;
    }

    bool init() {
        // Convert hex string into bytes for RAM
        uint16_t nOffset = 0x8000;
        std::fstream file("../tests/6502_functional_test.he");
        std::string line;

        if (!file.is_open()) {
            std::cerr << "Error opening file." << std::endl;
            return false;
        }

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            while (!ss.eof()) {
                std::string b;
                ss >> b;
                bus.ram[nOffset++] = (uint8_t) std::stoul(b, nullptr, 16);
            }
        }

        // Set Reset Vector
        bus.ram[0xFFFC] = 0x00;
        bus.ram[0xFFFD] = 0x80;

        // Reset
        bus.cpu.reset();

        return true;
    }

    void loop() {
        long long cycle_count = 0;

        while (!bus.cpu.illegalOpcode) {
            // Clear the terminal screen
            std::cout << "\033c";
            std::cout << DEFAULT_TEXT << "SPACE = Step Instruction    R = RESET    I = IRQ    N = NMI" << std::endl;
            std::cout << std::endl;
            PrintRam(0x0000, 16, 16);
            std::cout << std::endl;
            PrintRam(0x8000, 16, 16);
            std::cout << std::endl;
            PrintCpu();

            if (cycle_count >= 0) {
                int c = _getch();

                if (c == ' ') { // SPACE
                    do {
                        bus.cpu.clock();
                        cycle_count++;
                    }
                    while (bus.cpu.cycles > 0);
                }
                if (c == 'r')
                    bus.cpu.reset();
                if (c == 'i')
                    bus.cpu.irq();
                if (c == 'n')
                    bus.cpu.nmi();
            } else {
                do {
                    bus.cpu.clock();
                    cycle_count++;
                }
                while (bus.cpu.cycles > 0);
                Sleep(100);
            }
        }
    }

};