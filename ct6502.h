#ifndef CT6502_H
#define CT6502_H

#include <cstdint>
#include "Bus.h"

#undef OVERFLOW

class SimpleRamBus;

class ct6502 {

public:
    ct6502();
    ~ct6502();

public:
    enum FLAGS6502 {
        CARRY     = 0x01,
        ZERO      = 0x02,
        INTERRUPT = 0x04,
        DECIMAL   = 0x08, // unused for now
        BREAK     = 0x10,
        CONSTANT  = 0x20, // unused
        OVERFLOW  = 0x40,
        NEGATIVE  = 0x80,
    };

    uint8_t a = 0x00; // Accumulator register
    uint8_t x = 0x00; // X register
    uint8_t y = 0x00; // Y register
    uint8_t stk_ptr = 0x00; // Stack pointer
    uint16_t pc = 0x00; // Program counter
    uint8_t status = 0x00; // Status register

    void connectBus(Bus *b) { bus = b; }

    // Addressing Modes
    uint8_t Addr_IMP(); // IMPLIED
    uint8_t Addr_IMM(); // IMMEDIATE
    uint8_t Addr_ZER(); // ZERO PAGE
    uint8_t Addr_ZPX(); // INDEXED-X ZERO PAGE
    uint8_t Addr_ZPY(); // INDEXED-Y ZERO PAGE
    uint8_t Addr_REL(); // RELATIVE
    uint8_t Addr_ABS(); // ABSOLUTE
    uint8_t Addr_ABX(); // INDEXED-X ABSOLUTE
    uint8_t Addr_ABY(); // INDEXED-Y ABSOLUTE
    uint8_t Addr_IND(); // INDIRECT
    uint8_t Addr_INX(); // INDEXED-X INDIRECT
    uint8_t Addr_INY(); // INDEXED-Y INDIRECT

    // Opcodes (in alphabetical order)
    uint8_t Op_ADC();	uint8_t Op_AND();	uint8_t Op_ASL();	uint8_t Op_BCC();
    uint8_t Op_BCS();	uint8_t Op_BEQ();	uint8_t Op_BIT();	uint8_t Op_BMI();
    uint8_t Op_BNE();	uint8_t Op_BPL();	uint8_t Op_BRK();	uint8_t Op_BVC();
    uint8_t Op_BVS();	uint8_t Op_CLC();	uint8_t Op_CLD();	uint8_t Op_CLI();
    uint8_t Op_CLV();	uint8_t Op_CMP();	uint8_t Op_CPX();	uint8_t Op_CPY();
    uint8_t Op_DEC();	uint8_t Op_DEX();	uint8_t Op_DEY();	uint8_t Op_EOR();
    uint8_t Op_INC();	uint8_t Op_INX();	uint8_t Op_INY();	uint8_t Op_JMP();
    uint8_t Op_JSR();	uint8_t Op_LDA();	uint8_t Op_LDX();	uint8_t Op_LDY();
    uint8_t Op_LSR();	uint8_t Op_NOP();	uint8_t Op_ORA();	uint8_t Op_PHA();
    uint8_t Op_PHP();	uint8_t Op_PLA();	uint8_t Op_PLP();	uint8_t Op_ROL();
    uint8_t Op_ROR();	uint8_t Op_RTI();	uint8_t Op_RTS();	uint8_t Op_SBC();
    uint8_t Op_SEC();	uint8_t Op_SED();	uint8_t Op_SEI();	uint8_t Op_STA();
    uint8_t Op_STX();	uint8_t Op_STY();	uint8_t Op_TAX();	uint8_t Op_TAY();
    uint8_t Op_TSX();	uint8_t Op_TXA();	uint8_t Op_TXS();	uint8_t Op_TYA();

    // Illegal Opcode
    uint8_t Op_ILLEGAL();

    void clock();
    void reset();
    void irq();
    void nmi();

    uint8_t fetch();
    uint8_t fetched = 0x00;
    uint16_t tmp = 0x00;
    uint16_t addr_abs = 0x0000;
    uint16_t addr_rel = 0x00;
    uint8_t opcode = 0x00;
    uint8_t cycles = 0x00;
    bool illegalOpcode = false;

private:
    Bus *bus = nullptr;

private:
    void write(uint16_t addr, uint16_t data);
    [[nodiscard]] uint16_t read(uint16_t addr) const;

    void PushToStack(uint8_t v);
    [[nodiscard]] uint8_t PopFromStack();

    [[nodiscard]] uint8_t GetFlag(ct6502::FLAGS6502 f) const;
    void SetFlag(ct6502::FLAGS6502 f, bool v);

    typedef uint8_t (ct6502::*CodeExec)();
    typedef uint8_t (ct6502::*AddrExec)();
    struct INSTRUCTION {
        CodeExec code;
        AddrExec addr;
        uint8_t cycles = 0;
    };

    INSTRUCTION InstrTable[256];
};

#endif //CT6502_H
