#include <string>
#include <iostream>
#include <sstream>
#include <conio.h>
#include <fstream>

#include "SimpleRamBus.h"

#define DEFAULT_TEXT "\033[39m\033[49m\033[0m"
#define RED   "\033[38;2;255;0;0m"
#define GREEN "\033[38;2;0;255;0m"

#define UPDATE_DISPLAY_ON_AUTOSTEP false
#define AUTOSTEP_AMT 1000

class EmuTest6502 {

public:
    SimpleRamBus emu;

    enum PROGRAM_FORMAT {
        PLAIN_HEX,
        INTEL_HEX,
        BINARY,
    };

    bool init() {
        if (!LoadProgram("../tests/6502_functional_test.hex", 0x0000, INTEL_HEX))
            return false;

        // Set Reset Vector
        emu.ram[0xFFFC] = 0x00;
        emu.ram[0xFFFD] = 0x04;

        // Reset
        emu.cpu.reset();

        return true;
    }

    void loop() {
        long long cycle_count = 0;
        int auto_steps = 0;
        int c = '\0';

        while (!emu.cpu.illegalOpcode) {
            if (auto_steps-- > 0) { // Currently autostepping
                if (UPDATE_DISPLAY_ON_AUTOSTEP) display(c);
                std::cout << "\r" << "Autostepping: " << (AUTOSTEP_AMT - auto_steps) << "/" << AUTOSTEP_AMT;

                emu.cpu.clock();
                cycle_count++;

//                Sleep(10);
            }
            else {
                display(c);

                if (cycle_count >= 0) {
                    c = _getch();

                    if (c == ' ' || c == 'a') { // SPACE
                        if (c == 'a') auto_steps = AUTOSTEP_AMT;
                        emu.cpu.clock();
                        cycle_count++;
                    }
                    if (c == 'r')
                        emu.cpu.reset();
                    if (c == 'i')
                        emu.cpu.irq();
                    if (c == 'n')
                        emu.cpu.nmi();
                }
            }

            while (emu.cpu.cycles > 0) { // complete current instruction before either getting input or decrementing autostep
                emu.cpu.clock();
                cycle_count++;
            }
        }
    }

private:
    static std::string hex(uint32_t n, uint8_t d) {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4)
            s[i] = "0123456789ABCDEF"[n & 0xF];
        return s;
    }

    bool LoadProgram(const std::string& filename, uint16_t memOffset, PROGRAM_FORMAT format, const bool debug = false) {
        std::fstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error opening file." << std::endl;
            return false;
        }
        std::string line;

        switch (format) {

            case PLAIN_HEX: {
                while (std::getline(file, line)) {
                    std::stringstream ss(line);
                    while (!ss.eof()) {
                        std::string b;
                        ss >> b;
                        emu.ram[memOffset++] = (uint8_t) std::stoul(b, nullptr, 16);
                    }
                }
                return true;
            }

            case INTEL_HEX: {
                std::string b2, b4;
                b2.resize(2); b4.resize(4);
                // https://en.wikipedia.org/wiki/Intel_HEX
                bool eof = false;
                while (std::getline(file, line) && !eof) {
                    std::stringstream ss(line);
                    while (ss.get() != ':');
                    // Go until ':' appears on the line, ignore all data before

                    ss.read(&b2[0], 2);
                    auto byte_count = (uint8_t) std::stoul(b2, nullptr, 16);

                    ss.read(&b4[0], 4);
                    auto addr = (uint16_t) std::stoul(b4, nullptr, 16);

                    ss.read(&b2[0], 2);
                    auto record_type = (uint8_t) std::stoul(b2, nullptr, 16);

                    if (debug) {
                        std::cout << ss.str() << std::endl;
                        std::cout << "c: " << (uint16_t) byte_count
                                  << " a: " << addr
                                  << " t: " << (uint16_t) record_type << std::endl;
                    }

                    switch (record_type) {
                        case 0x00: {
                            uint16_t checksum = byte_count + ((addr >> 8) & 0xFF) + (addr & 0xFF)+ record_type;
                            for (uint32_t memAddr = memOffset + addr;
                                 memAddr < memOffset + addr + byte_count; memAddr++) {
                                ss.read(&b2[0], 2);
                                auto data = (uint8_t) std::stoul(b2, nullptr, 16);
                                if (debug) std::cout << b2 << ": " << (uint16_t) data << " @ 0x" << hex(memAddr, 4) << std::endl;
                                emu.ram[memAddr] = data;
                                checksum += data;
                            }
                            ss.read(&b2[0], 2);
                            checksum += (uint8_t) std::stoul(b2, nullptr, 16);
                            if (debug) std::cout << "checksum: " << hex(checksum, 4) << std::endl;
                            if ((checksum & 0xFF) != 0) {
                                if (debug) _getch();
                                return false;
                            }
                            break;
                        }
                        case 0x01: {
                            eof = true;
                            break;
                        }
                        default:
                            continue;
                    }
                }
                if (debug) _getch();
                return true;
            }

            case BINARY: // TODO
                return false;
        }
    }

    void PrintRam(uint16_t nAddr, int nRows, int nColumns) const {
        std::cout << DEFAULT_TEXT << "        ";
        for (int lolo = nAddr; lolo < nAddr + nColumns; lolo++) {
            std::cout << "0123456789ABCDEF"[lolo & 0xF] << "  ";
        }
        std::cout << std::endl;
        for (int row = 0; row < nRows; row++)
        {
            std::string sOffset = "$" + hex(nAddr, 4) + ":";
            for (int col = 0; col < nColumns; col++)
            {
                sOffset += " " + hex(emu.read(nAddr), 2);
                nAddr += 1;
            }
            std::cout << sOffset << std::endl;
        }
    }

    void PrintCpu() const {
        std::cout << DEFAULT_TEXT << "Status: ";
        std::cout << ((emu.cpu.status & ct6502::NEGATIVE  ) ? GREEN : RED) << "N" ;
        std::cout << ((emu.cpu.status & ct6502::OVERFLOW  ) ? GREEN : RED) << "V" ;
        std::cout << ((emu.cpu.status & ct6502::CONSTANT  ) ? GREEN : RED) << "-" ;
        std::cout << ((emu.cpu.status & ct6502::BREAK     ) ? GREEN : RED) << "B" ;
        std::cout << ((emu.cpu.status & ct6502::DECIMAL   ) ? GREEN : RED) << "D" ;
        std::cout << ((emu.cpu.status & ct6502::INTERRUPT ) ? GREEN : RED) << "I" ;
        std::cout << ((emu.cpu.status & ct6502::ZERO      ) ? GREEN : RED) << "Z" ;
        std::cout << ((emu.cpu.status & ct6502::CARRY     ) ? GREEN : RED) << "C" ;
        std::cout << DEFAULT_TEXT << std::endl;

        std::cout << "PC: $" << hex(emu.cpu.pc, 4) << std::endl;
        std::cout << "A: $" << hex(emu.cpu.a, 2) << "  [" << std::to_string(emu.cpu.a) << "]" << std::endl;
        std::cout << "X: $" << hex(emu.cpu.x, 2) << "  [" << std::to_string(emu.cpu.x) << "]" << std::endl;
        std::cout << "Y: $" << hex(emu.cpu.y, 2) << "  [" << std::to_string(emu.cpu.y) << "]" << std::endl;
        std::cout << "Stack P: $" << hex(emu.cpu.stk_ptr, 4) << std::endl;
    }

    static std::string underlineIf(bool b) {
        return (b ? "\033[4m" : DEFAULT_TEXT);
    }

    void display(const int c) const {
        // Clear the terminal screen
        std::cout << "\033c";

        std::cout << underlineIf(c==' ') << " SPACE=Step 1 Instruction "
                  << underlineIf(c=='r') << " R=RESET "
                  << underlineIf(c=='i') << " I=IRQ "
                  << underlineIf(c=='n') << " N=NMI "
                  << underlineIf(c=='a') << " A=Autostep " << AUTOSTEP_AMT << " Instructions "
                  << std::endl;

        std::cout << std::endl;
        PrintRam(0x0000, 16, 16);
        std::cout << std::endl;
        PrintRam(0x0400, 16, 16);
        std::cout << std::endl;
        PrintCpu();
    }

};