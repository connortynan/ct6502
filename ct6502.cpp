#include "ct6502.h"
#include "Bus.h"

#undef OVERFLOW

#define STACK_ADDR 0x0100

ct6502::ct6502() {

    INSTRUCTION instr;
    // Fill lookup array with illegal operations
    instr.code = &ct6502::Op_ILLEGAL;
    instr.addr = &ct6502::Addr_IMP;
    instr.cycles = 0;
    for (auto & i : InstrTable) {
        i = instr;
    }

    // insert opcodes
    using c = ct6502;

    instr.code = &c::Op_ADC; instr.addr = &c::Addr_IMM; instr.cycles = 2; InstrTable[0x69] = instr;
    instr.code = &c::Op_ADC; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0x6D] = instr;
    instr.code = &c::Op_ADC; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0x65] = instr;
    instr.code = &c::Op_ADC; instr.addr = &c::Addr_INX; instr.cycles = 6; InstrTable[0x61] = instr;
    instr.code = &c::Op_ADC; instr.addr = &c::Addr_INY; instr.cycles = 6; InstrTable[0x71] = instr;
    instr.code = &c::Op_ADC; instr.addr = &c::Addr_ZPX; instr.cycles = 4; InstrTable[0x75] = instr;
    instr.code = &c::Op_ADC; instr.addr = &c::Addr_ABX; instr.cycles = 4; InstrTable[0x7D] = instr;
    instr.code = &c::Op_ADC; instr.addr = &c::Addr_ABY; instr.cycles = 4; InstrTable[0x79] = instr;

    instr.code = &c::Op_AND; instr.addr = &c::Addr_IMM; instr.cycles = 2; InstrTable[0x29] = instr;
    instr.code = &c::Op_AND; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0x2D] = instr;
    instr.code = &c::Op_AND; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0x25] = instr;
    instr.code = &c::Op_AND; instr.addr = &c::Addr_INX; instr.cycles = 6; InstrTable[0x21] = instr;
    instr.code = &c::Op_AND; instr.addr = &c::Addr_INY; instr.cycles = 5; InstrTable[0x31] = instr;
    instr.code = &c::Op_AND; instr.addr = &c::Addr_ZPX; instr.cycles = 4; InstrTable[0x35] = instr;
    instr.code = &c::Op_AND; instr.addr = &c::Addr_ABX; instr.cycles = 4; InstrTable[0x3D] = instr;
    instr.code = &c::Op_AND; instr.addr = &c::Addr_ABY; instr.cycles = 4; InstrTable[0x39] = instr;

    instr.code = &c::Op_ASL; instr.addr = &c::Addr_ABS; instr.cycles = 6; InstrTable[0x0E] = instr;
    instr.code = &c::Op_ASL; instr.addr = &c::Addr_ZER; instr.cycles = 5; InstrTable[0x06] = instr;
    instr.code = &c::Op_ASL; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0x0A] = instr;
    instr.code = &c::Op_ASL; instr.addr = &c::Addr_ZPX; instr.cycles = 6; InstrTable[0x16] = instr;
    instr.code = &c::Op_ASL; instr.addr = &c::Addr_ABX; instr.cycles = 7; InstrTable[0x1E] = instr;

    instr.code = &c::Op_BCC; instr.addr = &c::Addr_REL; instr.cycles = 2; InstrTable[0x90] = instr;

    instr.code = &c::Op_BCS; instr.addr = &c::Addr_REL; instr.cycles = 2; InstrTable[0xB0] = instr;

    instr.code = &c::Op_BEQ; instr.addr = &c::Addr_REL; instr.cycles = 2; InstrTable[0xF0] = instr;

    instr.code = &c::Op_BIT; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0x2C] = instr;
    instr.code = &c::Op_BIT; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0x24] = instr;

    instr.code = &c::Op_BMI; instr.addr = &c::Addr_REL; instr.cycles = 2; InstrTable[0x30] = instr;

    instr.code = &c::Op_BNE; instr.addr = &c::Addr_REL; instr.cycles = 2; InstrTable[0xD0] = instr;

    instr.code = &c::Op_BPL; instr.addr = &c::Addr_REL; instr.cycles = 2; InstrTable[0x10] = instr;

    instr.code = &c::Op_BRK; instr.addr = &c::Addr_IMP; instr.cycles = 7; InstrTable[0x00] = instr;

    instr.code = &c::Op_BVC; instr.addr = &c::Addr_REL; instr.cycles = 2; InstrTable[0x50] = instr;

    instr.code = &c::Op_BVS; instr.addr = &c::Addr_REL; instr.cycles = 2; InstrTable[0x70] = instr;

    instr.code = &c::Op_CLC; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0x18] = instr;

    instr.code = &c::Op_CLD; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0xD8] = instr;

    instr.code = &c::Op_CLI; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0x58] = instr;

    instr.code = &c::Op_CLV; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0xB8] = instr;

    instr.code = &c::Op_CMP; instr.addr = &c::Addr_IMM; instr.cycles = 2; InstrTable[0xC9] = instr;
    instr.code = &c::Op_CMP; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0xCD] = instr;
    instr.code = &c::Op_CMP; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0xC5] = instr;
    instr.code = &c::Op_CMP; instr.addr = &c::Addr_INX; instr.cycles = 6; InstrTable[0xC1] = instr;
    instr.code = &c::Op_CMP; instr.addr = &c::Addr_INY; instr.cycles = 3; InstrTable[0xD1] = instr;
    instr.code = &c::Op_CMP; instr.addr = &c::Addr_ZPX; instr.cycles = 4; InstrTable[0xD5] = instr;
    instr.code = &c::Op_CMP; instr.addr = &c::Addr_ABX; instr.cycles = 4; InstrTable[0xDD] = instr;
    instr.code = &c::Op_CMP; instr.addr = &c::Addr_ABY; instr.cycles = 4; InstrTable[0xD9] = instr;

    instr.code = &c::Op_CPX; instr.addr = &c::Addr_IMM; instr.cycles = 2; InstrTable[0xE0] = instr;
    instr.code = &c::Op_CPX; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0xEC] = instr;
    instr.code = &c::Op_CPX; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0xE4] = instr;

    instr.code = &c::Op_CPY; instr.addr = &c::Addr_IMM; instr.cycles = 2; InstrTable[0xC0] = instr;
    instr.code = &c::Op_CPY; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0xCC] = instr;
    instr.code = &c::Op_CPY; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0xC4] = instr;

    instr.code = &c::Op_DEC; instr.addr = &c::Addr_ABS; instr.cycles = 6; InstrTable[0xCE] = instr;
    instr.code = &c::Op_DEC; instr.addr = &c::Addr_ZER; instr.cycles = 5; InstrTable[0xC6] = instr;
    instr.code = &c::Op_DEC; instr.addr = &c::Addr_ZPX; instr.cycles = 6; InstrTable[0xD6] = instr;
    instr.code = &c::Op_DEC; instr.addr = &c::Addr_ABX; instr.cycles = 7; InstrTable[0xDE] = instr;

    instr.code = &c::Op_DEX; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0xCA] = instr;

    instr.code = &c::Op_DEY; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0x88] = instr;

    instr.code = &c::Op_EOR; instr.addr = &c::Addr_IMM; instr.cycles = 2; InstrTable[0x49] = instr;
    instr.code = &c::Op_EOR; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0x4D] = instr;
    instr.code = &c::Op_EOR; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0x45] = instr;
    instr.code = &c::Op_EOR; instr.addr = &c::Addr_INX; instr.cycles = 6; InstrTable[0x41] = instr;
    instr.code = &c::Op_EOR; instr.addr = &c::Addr_INY; instr.cycles = 5; InstrTable[0x51] = instr;
    instr.code = &c::Op_EOR; instr.addr = &c::Addr_ZPX; instr.cycles = 4; InstrTable[0x55] = instr;
    instr.code = &c::Op_EOR; instr.addr = &c::Addr_ABX; instr.cycles = 4; InstrTable[0x5D] = instr;
    instr.code = &c::Op_EOR; instr.addr = &c::Addr_ABY; instr.cycles = 4; InstrTable[0x59] = instr;

    instr.code = &c::Op_INC; instr.addr = &c::Addr_ABS; instr.cycles = 6; InstrTable[0xEE] = instr;
    instr.code = &c::Op_INC; instr.addr = &c::Addr_ZER; instr.cycles = 5; InstrTable[0xE6] = instr;
    instr.code = &c::Op_INC; instr.addr = &c::Addr_ZPX; instr.cycles = 6; InstrTable[0xF6] = instr;
    instr.code = &c::Op_INC; instr.addr = &c::Addr_ABX; instr.cycles = 7; InstrTable[0xFE] = instr;

    instr.code = &c::Op_INX; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0xE8] = instr;

    instr.code = &c::Op_INY; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0xC8] = instr;

    instr.code = &c::Op_JMP; instr.addr = &c::Addr_ABS; instr.cycles = 3; InstrTable[0x4C] = instr;
    instr.code = &c::Op_JMP; instr.addr = &c::Addr_IND; instr.cycles = 5; InstrTable[0x6C] = instr;

    instr.code = &c::Op_JSR; instr.addr = &c::Addr_ABS; instr.cycles = 6; InstrTable[0x20] = instr;

    instr.code = &c::Op_LDA; instr.addr = &c::Addr_IMM; instr.cycles = 2; InstrTable[0xA9] = instr;
    instr.code = &c::Op_LDA; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0xAD] = instr;
    instr.code = &c::Op_LDA; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0xA5] = instr;
    instr.code = &c::Op_LDA; instr.addr = &c::Addr_INX; instr.cycles = 6; InstrTable[0xA1] = instr;
    instr.code = &c::Op_LDA; instr.addr = &c::Addr_INY; instr.cycles = 5; InstrTable[0xB1] = instr;
    instr.code = &c::Op_LDA; instr.addr = &c::Addr_ZPX; instr.cycles = 4; InstrTable[0xB5] = instr;
    instr.code = &c::Op_LDA; instr.addr = &c::Addr_ABX; instr.cycles = 4; InstrTable[0xBD] = instr;
    instr.code = &c::Op_LDA; instr.addr = &c::Addr_ABY; instr.cycles = 4; InstrTable[0xB9] = instr;

    instr.code = &c::Op_LDX; instr.addr = &c::Addr_IMM; instr.cycles = 2; InstrTable[0xA2] = instr;
    instr.code = &c::Op_LDX; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0xAE] = instr;
    instr.code = &c::Op_LDX; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0xA6] = instr;
    instr.code = &c::Op_LDX; instr.addr = &c::Addr_ABY; instr.cycles = 4; InstrTable[0xBE] = instr;
    instr.code = &c::Op_LDX; instr.addr = &c::Addr_ZPY; instr.cycles = 4; InstrTable[0xB6] = instr;

    instr.code = &c::Op_LDY; instr.addr = &c::Addr_IMM; instr.cycles = 2; InstrTable[0xA0] = instr;
    instr.code = &c::Op_LDY; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0xAC] = instr;
    instr.code = &c::Op_LDY; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0xA4] = instr;
    instr.code = &c::Op_LDY; instr.addr = &c::Addr_ZPX; instr.cycles = 4; InstrTable[0xB4] = instr;
    instr.code = &c::Op_LDY; instr.addr = &c::Addr_ABX; instr.cycles = 4; InstrTable[0xBC] = instr;

    instr.code = &c::Op_LSR; instr.addr = &c::Addr_ABS; instr.cycles = 6; InstrTable[0x4E] = instr;
    instr.code = &c::Op_LSR; instr.addr = &c::Addr_ZER; instr.cycles = 5; InstrTable[0x46] = instr;
    instr.code = &c::Op_LSR; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0x4A] = instr;
    instr.code = &c::Op_LSR; instr.addr = &c::Addr_ZPX; instr.cycles = 6; InstrTable[0x56] = instr;
    instr.code = &c::Op_LSR; instr.addr = &c::Addr_ABX; instr.cycles = 7; InstrTable[0x5E] = instr;

    instr.code = &c::Op_NOP; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0xEA] = instr;

    instr.code = &c::Op_ORA; instr.addr = &c::Addr_IMM; instr.cycles = 2; InstrTable[0x09] = instr;
    instr.code = &c::Op_ORA; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0x0D] = instr;
    instr.code = &c::Op_ORA; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0x05] = instr;
    instr.code = &c::Op_ORA; instr.addr = &c::Addr_INX; instr.cycles = 6; InstrTable[0x01] = instr;
    instr.code = &c::Op_ORA; instr.addr = &c::Addr_INY; instr.cycles = 5; InstrTable[0x11] = instr;
    instr.code = &c::Op_ORA; instr.addr = &c::Addr_ZPX; instr.cycles = 4; InstrTable[0x15] = instr;
    instr.code = &c::Op_ORA; instr.addr = &c::Addr_ABX; instr.cycles = 4; InstrTable[0x1D] = instr;
    instr.code = &c::Op_ORA; instr.addr = &c::Addr_ABY; instr.cycles = 4; InstrTable[0x19] = instr;

    instr.code = &c::Op_PHA; instr.addr = &c::Addr_IMP; instr.cycles = 3; InstrTable[0x48] = instr;

    instr.code = &c::Op_PHP; instr.addr = &c::Addr_IMP; instr.cycles = 3; InstrTable[0x08] = instr;

    instr.code = &c::Op_PLA; instr.addr = &c::Addr_IMP; instr.cycles = 4; InstrTable[0x68] = instr;

    instr.code = &c::Op_PLP; instr.addr = &c::Addr_IMP; instr.cycles = 4; InstrTable[0x28] = instr;

    instr.code = &c::Op_ROL; instr.addr = &c::Addr_ABS; instr.cycles = 6; InstrTable[0x2E] = instr;
    instr.code = &c::Op_ROL; instr.addr = &c::Addr_ZER; instr.cycles = 5; InstrTable[0x26] = instr;
    instr.code = &c::Op_ROL; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0x2A] = instr;
    instr.code = &c::Op_ROL; instr.addr = &c::Addr_ZPX; instr.cycles = 6; InstrTable[0x36] = instr;
    instr.code = &c::Op_ROL; instr.addr = &c::Addr_ABX; instr.cycles = 7; InstrTable[0x3E] = instr;

    instr.code = &c::Op_ROR; instr.addr = &c::Addr_ABS; instr.cycles = 6; InstrTable[0x6E] = instr;
    instr.code = &c::Op_ROR; instr.addr = &c::Addr_ZER; instr.cycles = 5; InstrTable[0x66] = instr;
    instr.code = &c::Op_ROR; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0x6A] = instr;
    instr.code = &c::Op_ROR; instr.addr = &c::Addr_ZPX; instr.cycles = 6; InstrTable[0x76] = instr;
    instr.code = &c::Op_ROR; instr.addr = &c::Addr_ABX; instr.cycles = 7; InstrTable[0x7E] = instr;

    instr.code = &c::Op_RTI; instr.addr = &c::Addr_IMP; instr.cycles = 6; InstrTable[0x40] = instr;

    instr.code = &c::Op_RTS; instr.addr = &c::Addr_IMP; instr.cycles = 6; InstrTable[0x60] = instr;

    instr.code = &c::Op_SBC; instr.addr = &c::Addr_IMM; instr.cycles = 2; InstrTable[0xE9] = instr;
    instr.code = &c::Op_SBC; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0xED] = instr;
    instr.code = &c::Op_SBC; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0xE5] = instr;
    instr.code = &c::Op_SBC; instr.addr = &c::Addr_INX; instr.cycles = 6; InstrTable[0xE1] = instr;
    instr.code = &c::Op_SBC; instr.addr = &c::Addr_INY; instr.cycles = 5; InstrTable[0xF1] = instr;
    instr.code = &c::Op_SBC; instr.addr = &c::Addr_ZPX; instr.cycles = 4; InstrTable[0xF5] = instr;
    instr.code = &c::Op_SBC; instr.addr = &c::Addr_ABX; instr.cycles = 4; InstrTable[0xFD] = instr;
    instr.code = &c::Op_SBC; instr.addr = &c::Addr_ABY; instr.cycles = 4; InstrTable[0xF9] = instr;

    instr.code = &c::Op_SEC; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0x38] = instr;

    instr.code = &c::Op_SED; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0xF8] = instr;

    instr.code = &c::Op_SEI; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0x78] = instr;

    instr.code = &c::Op_STA; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0x8D] = instr;
    instr.code = &c::Op_STA; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0x85] = instr;
    instr.code = &c::Op_STA; instr.addr = &c::Addr_INX; instr.cycles = 6; InstrTable[0x81] = instr;
    instr.code = &c::Op_STA; instr.addr = &c::Addr_INY; instr.cycles = 6; InstrTable[0x91] = instr;
    instr.code = &c::Op_STA; instr.addr = &c::Addr_ZPX; instr.cycles = 4; InstrTable[0x95] = instr;
    instr.code = &c::Op_STA; instr.addr = &c::Addr_ABX; instr.cycles = 5; InstrTable[0x9D] = instr;
    instr.code = &c::Op_STA; instr.addr = &c::Addr_ABY; instr.cycles = 5; InstrTable[0x99] = instr;

    instr.code = &c::Op_STX; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0x8E] = instr;
    instr.code = &c::Op_STX; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0x86] = instr;
    instr.code = &c::Op_STX; instr.addr = &c::Addr_ZPY; instr.cycles = 4; InstrTable[0x96] = instr;

    instr.code = &c::Op_STY; instr.addr = &c::Addr_ABS; instr.cycles = 4; InstrTable[0x8C] = instr;
    instr.code = &c::Op_STY; instr.addr = &c::Addr_ZER; instr.cycles = 3; InstrTable[0x84] = instr;
    instr.code = &c::Op_STY; instr.addr = &c::Addr_ZPX; instr.cycles = 4; InstrTable[0x94] = instr;

    instr.code = &c::Op_TAX; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0xAA] = instr;

    instr.code = &c::Op_TAY; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0xA8] = instr;

    instr.code = &c::Op_TSX; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0xBA] = instr;

    instr.code = &c::Op_TXA; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0x8A] = instr;

    instr.code = &c::Op_TXS; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0x9A] = instr;

    instr.code = &c::Op_TYA; instr.addr = &c::Addr_IMP; instr.cycles = 2; InstrTable[0x98] = instr;
}

ct6502::~ct6502() = default;

void ct6502::write(uint16_t addr, uint16_t data) {
    bus->write(addr, data);
}

uint16_t ct6502::read(uint16_t addr) const {
    return bus->read(addr);
}

void ct6502::clock() {
    if (cycles == 0) {
        opcode = read(pc++);

        cycles = InstrTable[opcode].cycles;

        uint8_t additional_cycle1 = (this->*InstrTable[opcode].addr)();

        uint8_t additional_cycle2 = (this->*InstrTable[opcode].code)();

        cycles += (additional_cycle1 & additional_cycle2);
    }

    cycles--;
}

void ct6502::reset() {
    a = 0;
    x = 0;
    y = 0;
    stk_ptr = 0xFD;
    status = 0x00 | CONSTANT;

    addr_abs = 0xFFFC;
    uint16_t lo = read(addr_abs + 0);
    uint16_t hi = read(addr_abs + 1);
    pc = (hi  << 8) | lo;

    addr_rel = 0x0000;
    addr_abs = 0x0000;
    fetched = 0x0000;

    cycles = 8;
}

void ct6502::irq() {
    if (GetFlag(INTERRUPT) == 0) {
        write(STACK_ADDR + stk_ptr, (pc >> 8) & 0x00FF);
        stk_ptr--;
        write(STACK_ADDR + stk_ptr, pc & 0x00FF);
        stk_ptr--;

        SetFlag(BREAK, false);
        SetFlag(CONSTANT, true);
        SetFlag(INTERRUPT, true);
        write(STACK_ADDR + stk_ptr, status);
        stk_ptr--;

        addr_abs = 0xFFFE;
        uint16_t lo = read(addr_abs + 0);
        uint16_t hi = read(addr_abs + 1);
        pc = (hi  << 8) | lo;

        cycles = 7;
    }
}

void ct6502::nmi() {
    write(STACK_ADDR + stk_ptr, (pc >> 8) & 0x00FF);
    stk_ptr--;
    write(STACK_ADDR + stk_ptr, pc & 0x00FF);
    stk_ptr--;

    SetFlag(BREAK, false);
    SetFlag(CONSTANT, true);
    SetFlag(INTERRUPT, true);
    write(STACK_ADDR + stk_ptr, status);
    stk_ptr--;

    addr_abs = 0xFFFA;
    uint16_t lo = read(addr_abs + 0);
    uint16_t hi = read(addr_abs + 1);
    pc = (hi  << 8) | lo;

    cycles = 8;
}

uint8_t ct6502::GetFlag(ct6502::FLAGS6502 f) const {
    return ((status & f) > 0) ? 1 : 0;
}

void ct6502::SetFlag(ct6502::FLAGS6502 f, bool v) {
    if (v)
        status |= f;
    else
        status &= ~f;
}

uint8_t ct6502::fetch() {
    if (InstrTable[opcode].addr != &ct6502::Addr_IMP)
        fetched = read(addr_abs);
    return fetched;
}

// Addressing Modes

uint8_t ct6502::Addr_IMP() {
    fetched = a;
    return 0;
}

uint8_t ct6502::Addr_IMM() {
    addr_abs = pc++;
    return 0;
}

uint8_t ct6502::Addr_ZER() {
    addr_abs = read(pc++);
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t ct6502::Addr_ZPX() {
    addr_abs = (read(pc++) + x);
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t ct6502::Addr_ZPY() {
    addr_abs = (read(pc++) + y);
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t ct6502::Addr_REL() {
    addr_rel = read(pc++);
    if (addr_rel & 0x80)
        addr_rel |= 0xFF00;
    return 0;
}

uint8_t ct6502::Addr_ABS() {
    uint16_t lo = read(pc++);
    uint16_t hi = read(pc++);
    addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t ct6502::Addr_ABX() {
    uint16_t lo = read(pc++);
    uint16_t hi = read(pc++);
    addr_abs = (hi << 8) | lo;
    addr_abs += x;
    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t ct6502::Addr_ABY() {
    uint16_t lo = read(pc++);
    uint16_t hi = read(pc++);
    addr_abs = (hi << 8) | lo;
    addr_abs += y;
    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t ct6502::Addr_IND() {
    uint16_t ptr_lo = read(pc++);
    uint16_t ptr_hi= read(pc++);
    uint16_t ptr = (ptr_hi << 8) | ptr_lo;
    addr_abs = (read(ptr+1) << 8) | read(ptr + 0);
    return 0;
}

uint8_t ct6502::Addr_INX() {
    uint16_t t = read(pc++);
    uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
    uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);
    addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t ct6502::Addr_INY() {
    uint16_t t = read(pc++);
    uint16_t lo = read(t & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);
    addr_abs = (hi << 8) | lo;
    addr_abs += y;
    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

// Operation Instructions

uint8_t ct6502::Op_ILLEGAL() {
    illegalOpcode = true;
    return 0;
}


uint8_t ct6502::Op_ADC() {
    // https://youtu.be/8XmxKPJDGU0?si=kscBnFep_h_t5Dla&t=2856
    fetch();
    tmp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(CARRY);
    SetFlag(CARRY, tmp > 255);
    SetFlag(ZERO, (tmp & 0x00FF) == 0);
    SetFlag(NEGATIVE, tmp & 0x80);
    SetFlag(OVERFLOW, (~((uint16_t)a ^ (uint16_t)fetched & ((uint16_t)a ^ (uint16_t)tmp)) & 0x0080));
    a = tmp & 0x00FF;
    return 1;
}

uint8_t ct6502::Op_AND() {
    fetch();
    a = a & fetched;
    SetFlag(ZERO, a == 0x00);
    SetFlag(NEGATIVE, a & 0x80);
    return 1;
}

uint8_t ct6502::Op_ASL() {
    fetch();
    tmp = (uint16_t)fetched << 1;
    SetFlag(CARRY, (tmp & 0xFF00) > 0);
    SetFlag(ZERO, (tmp & 0x00FF) == 0x00);
    SetFlag(NEGATIVE, tmp & 0x80);
    if (InstrTable[opcode].addr == &ct6502::Addr_IMP)
        a = tmp & 0x00FF;
    else
        write(addr_abs, tmp & 0x00FF);

    return 0;
}

uint8_t ct6502::Op_BCC() {
    if (GetFlag(CARRY) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t ct6502::Op_BCS() {
    if (GetFlag(CARRY) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t ct6502::Op_BEQ() {
    if (GetFlag(ZERO) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t ct6502::Op_BIT() {
    fetch();
    tmp = a & fetched;
    SetFlag(ZERO, (tmp & 0x00FF) == 0x00);
    SetFlag(NEGATIVE, fetched & (1 << 7));
    SetFlag(OVERFLOW, fetched & (1 << 6));
    return 0;
}

uint8_t ct6502::Op_BMI() {
    if (GetFlag(NEGATIVE) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t ct6502::Op_BNE() {
    if (GetFlag(ZERO) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t ct6502::Op_BPL() {
    if (GetFlag(NEGATIVE) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t ct6502::Op_BRK() {
    pc++;
    SetFlag(INTERRUPT, true);
    write(STACK_ADDR + stk_ptr, (pc >> 8) & 0x00FF);
    stk_ptr--;
    write(STACK_ADDR + stk_ptr, pc & 0x00FF);
    stk_ptr--;

    SetFlag(BREAK, true);
    write(STACK_ADDR + stk_ptr, status);
    stk_ptr--;
    SetFlag(BREAK, false);

    pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
    return 0;
}

uint8_t ct6502::Op_BVC() {
    if (GetFlag(OVERFLOW) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t ct6502::Op_BVS() {
    if (GetFlag(OVERFLOW) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t ct6502::Op_CLC() {
    SetFlag(CARRY, false);
    return 0;
}

uint8_t ct6502::Op_CLD() {
    SetFlag(DECIMAL, false);
    return 0;
}

uint8_t ct6502::Op_CLI() {
    SetFlag(INTERRUPT, false);
    return 0;
}

uint8_t ct6502::Op_CLV() {
    SetFlag(OVERFLOW, false);
    return 0;
}

uint8_t ct6502::Op_CMP() {
    fetch();
    tmp = (uint16_t)a - (uint16_t)fetched;
    SetFlag(CARRY, a >= fetched);
    SetFlag(ZERO, (tmp & 0x00FF) == 0);
    SetFlag(NEGATIVE, tmp & 0x0080);
    return 1;
}

uint8_t ct6502::Op_CPX() {
    fetch();
    tmp = (uint16_t)x - (uint16_t)fetched;
    SetFlag(CARRY, x >= fetched);
    SetFlag(ZERO, (tmp & 0x00FF) == 0);
    SetFlag(NEGATIVE, tmp & 0x0080);
    return 0;
}

uint8_t ct6502::Op_CPY() {
    fetch();
    tmp = (uint16_t)y - (uint16_t)fetched;
    SetFlag(CARRY, y >= fetched);
    SetFlag(ZERO, (tmp & 0x00FF) == 0);
    SetFlag(NEGATIVE, tmp & 0x0080);
    return 0;
}

uint8_t ct6502::Op_DEC() {
    fetch();
    tmp = fetched-1;
    write(addr_abs, tmp & 0x00FF);
    SetFlag(ZERO, (tmp & 0x00FF) == 0);
    SetFlag(NEGATIVE, tmp & 0x0080);
    return 0;
}

uint8_t ct6502::Op_DEX() {
    x--;
    SetFlag(ZERO, x == 0);
    SetFlag(NEGATIVE, x & 0x80);
    return 0;
}

uint8_t ct6502::Op_DEY() {
    y--;
    SetFlag(ZERO, y == 0);
    SetFlag(NEGATIVE, y & 0x80);
    return 0;
}

uint8_t ct6502::Op_EOR() {
    fetch();
    a = a ^ fetched;
    SetFlag(ZERO, a == 0);
    SetFlag(NEGATIVE, a & 0x80);
    return 1;
}

uint8_t ct6502::Op_INC() {
    fetch();
    tmp = fetched+1;
    write(addr_abs, tmp & 0x00FF);
    SetFlag(ZERO, (tmp & 0x00FF) == 0);
    SetFlag(NEGATIVE, tmp & 0x0080);
    return 0;
}

uint8_t ct6502::Op_INX() {
    x++;
    SetFlag(ZERO, x == 0);
    SetFlag(NEGATIVE, x & 0x80);
    return 0;
}

uint8_t ct6502::Op_INY() {
    y++;
    SetFlag(ZERO, y == 0);
    SetFlag(NEGATIVE, y & 0x80);
    return 0;
}

uint8_t ct6502::Op_JMP() {
    pc = addr_abs;
    return 0;
}

uint8_t ct6502::Op_JSR() {
    pc--;

    write(STACK_ADDR + stk_ptr, (pc >> 8) & 0x00FF);
    stk_ptr--;
    write(STACK_ADDR + stk_ptr, pc & 0x00FF);
    stk_ptr--;

    pc = addr_abs;
    return 0;
}

uint8_t ct6502::Op_LDA() {
    fetch();
    a = fetched;
    SetFlag(ZERO, a == 0);
    SetFlag(NEGATIVE, a & 0x80);
    return 1;
}

uint8_t ct6502::Op_LDX() {
    fetch();
    x = fetched;
    SetFlag(ZERO, x == 0);
    SetFlag(NEGATIVE, x & 0x80);
    return 1;
}

uint8_t ct6502::Op_LDY() {
    fetch();
    y = fetched;
    SetFlag(ZERO, y == 0);
    SetFlag(NEGATIVE, y & 0x80);
    return 1;
}

uint8_t ct6502::Op_LSR() {
    fetch();
    SetFlag(CARRY, fetched & 0x0001);
    tmp = fetched >> 1;
    SetFlag(ZERO, (tmp & 0x00FF) == 0);
    SetFlag(NEGATIVE, tmp & 0x0080);
    if (InstrTable[opcode].addr == &ct6502::Addr_IMP)
        a = tmp & 0x00FF;
    else
        write(addr_abs, tmp & 0x00FF);
    return 0;
}

uint8_t ct6502::Op_NOP() {
    return 0;
}

uint8_t ct6502::Op_ORA() {
    fetch();
    a = a | fetched;
    SetFlag(ZERO, a == 0);
    SetFlag(NEGATIVE, a & 0x80);
    return 1;
}

uint8_t ct6502::Op_PHA() {
    write(STACK_ADDR + stk_ptr, a);
    stk_ptr--;
    return 0;
}

uint8_t ct6502::Op_PHP() {
    write(STACK_ADDR + stk_ptr, status | BREAK | CONSTANT);
    SetFlag(BREAK, false);
    SetFlag(CONSTANT, false);
    stk_ptr--;
    return 0;
}

uint8_t ct6502::Op_PLA() {
    stk_ptr++;
    a = read(STACK_ADDR + stk_ptr);
    SetFlag(ZERO, a == 0x00);
    SetFlag(NEGATIVE, a & 0x80);
    return 0;
}

uint8_t ct6502::Op_PLP() {
    stk_ptr++;
    status = read(STACK_ADDR + stk_ptr);
    SetFlag(CONSTANT, true);
    return 0;
}

uint8_t ct6502::Op_ROL() {
    fetch();
    tmp = (uint16_t)(fetched<<1) | GetFlag(CARRY);
    SetFlag(CARRY, tmp & 0xFF00);
    SetFlag(ZERO, (tmp & 0x00FF) == 0);
    SetFlag(NEGATIVE, tmp & 0x0080);
    if (InstrTable[opcode].addr == &ct6502::Addr_IMP)
        a = tmp & 0x00FF;
    else
        write(addr_abs, tmp & 0x00FF);
    return 0;
}

uint8_t ct6502::Op_ROR() {
    fetch();
    tmp = (uint16_t)(GetFlag(CARRY) << 7) | (fetched>>1);
    SetFlag(CARRY, tmp & 0x0001);
    SetFlag(ZERO, (tmp & 0x00FF) == 0);
    SetFlag(NEGATIVE, tmp & 0x0080);
    if (InstrTable[opcode].addr == &ct6502::Addr_IMP)
        a = tmp & 0x00FF;
    else
        write(addr_abs, tmp & 0x00FF);
    return 0;
}

uint8_t ct6502::Op_RTI() {
    stk_ptr++;
    status = read(STACK_ADDR + stk_ptr);
    stk_ptr &= ~BREAK;
    stk_ptr &= ~CONSTANT;

    stk_ptr++;
    pc = (uint16_t)read(STACK_ADDR + stk_ptr);
    stk_ptr++;
    pc |= (uint16_t)read(STACK_ADDR + stk_ptr) << 8;
    return 0;
}

uint8_t ct6502::Op_RTS() {
    stk_ptr++;
    pc = (uint16_t)read(STACK_ADDR + stk_ptr);
    stk_ptr++;
    pc |= (uint16_t)read(STACK_ADDR + stk_ptr) << 8;

    pc++;
    return 0;
}

uint8_t ct6502::Op_SBC() {
    fetch();
    uint16_t val = ((uint16_t)fetched) ^ 0x00FF;
    tmp = (uint16_t)a + (uint16_t)val + (uint16_t)GetFlag(CARRY);
    SetFlag(CARRY, tmp > 255);
    SetFlag(ZERO, (tmp & 0x00FF) == 0);
    SetFlag(NEGATIVE, tmp & 0x80);
    SetFlag(OVERFLOW, (tmp ^ (uint16_t)a) & (tmp ^ val) & 0x0080);
    a = tmp & 0x00FF;
    return 1;
}

uint8_t ct6502::Op_SEC() {
    SetFlag(CARRY, true);
    return 0;
}

uint8_t ct6502::Op_SED() {
    SetFlag(DECIMAL, true);
    return 0;
}

uint8_t ct6502::Op_SEI() {
    SetFlag(INTERRUPT, true);
    return 0;
}

uint8_t ct6502::Op_STA() {
    write(addr_abs, a);
    return 0;
}

uint8_t ct6502::Op_STX() {
    write(addr_abs, x);
    return 0;
}

uint8_t ct6502::Op_STY() {
    write(addr_abs, y);
    return 0;
}

uint8_t ct6502::Op_TAX() {
    x = a;
    SetFlag(ZERO, x == 0);
    SetFlag(NEGATIVE, x & 0x80);
    return 0;
}

uint8_t ct6502::Op_TAY() {
    y = a;
    SetFlag(ZERO, y == 0);
    SetFlag(NEGATIVE, y & 0x80);
    return 0;
}

uint8_t ct6502::Op_TSX() {
    x = stk_ptr;
    SetFlag(ZERO, x == 0);
    SetFlag(NEGATIVE, x & 0x80);
    return 0;
}

uint8_t ct6502::Op_TXA() {
    a = x;
    SetFlag(ZERO, a == 0);
    SetFlag(NEGATIVE, a & 0x80);
    return 0;
}

uint8_t ct6502::Op_TXS() {
    stk_ptr = x;
    return 0;
}

uint8_t ct6502::Op_TYA() {
    a = y;
    SetFlag(ZERO, a == 0);
    SetFlag(NEGATIVE, a & 0x80);
    return 0;
}
