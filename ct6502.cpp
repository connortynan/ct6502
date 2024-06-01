#include "ct6502.h"
#include "Bus.h"

#define STACK_ADDR 0x0100

ct6502::ct6502() {

    INSTRUCTION in;
    // Fill lookup array with illegal operations
    in.code = &ct6502::Op_ILLEGAL;
    in.addr = &ct6502::Addr_IMP;
    in.cycles = 0;
    for (auto & i : InstrTable) {
        i = in;
    }

    // insert opcodes
    {
    // following this ordering: https://www.masswerk.at/6502/6502_instruction_set.html#ADC
    using c = ct6502;

    in.code = &c::Op_ADC; in.addr = &c::Addr_IMM; in.cycles = 2; InstrTable[0x69] = in;
    in.code = &c::Op_ADC; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0x65] = in;
    in.code = &c::Op_ADC; in.addr = &c::Addr_ZPX; in.cycles = 4; InstrTable[0x75] = in;
    in.code = &c::Op_ADC; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0x6D] = in;
    in.code = &c::Op_ADC; in.addr = &c::Addr_ABX; in.cycles = 4; InstrTable[0x7D] = in;
    in.code = &c::Op_ADC; in.addr = &c::Addr_ABY; in.cycles = 4; InstrTable[0x79] = in;
    in.code = &c::Op_ADC; in.addr = &c::Addr_INX; in.cycles = 6; InstrTable[0x61] = in;
    in.code = &c::Op_ADC; in.addr = &c::Addr_INY; in.cycles = 6; InstrTable[0x71] = in;

    in.code = &c::Op_AND; in.addr = &c::Addr_IMM; in.cycles = 2; InstrTable[0x29] = in;
    in.code = &c::Op_AND; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0x25] = in;
    in.code = &c::Op_AND; in.addr = &c::Addr_ZPX; in.cycles = 4; InstrTable[0x35] = in;
    in.code = &c::Op_AND; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0x2D] = in;
    in.code = &c::Op_AND; in.addr = &c::Addr_ABX; in.cycles = 4; InstrTable[0x3D] = in;
    in.code = &c::Op_AND; in.addr = &c::Addr_ABY; in.cycles = 4; InstrTable[0x39] = in;
    in.code = &c::Op_AND; in.addr = &c::Addr_INX; in.cycles = 6; InstrTable[0x21] = in;
    in.code = &c::Op_AND; in.addr = &c::Addr_INY; in.cycles = 5; InstrTable[0x31] = in;

    in.code = &c::Op_ASL; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0x0A] = in;
    in.code = &c::Op_ASL; in.addr = &c::Addr_ZER; in.cycles = 5; InstrTable[0x06] = in;
    in.code = &c::Op_ASL; in.addr = &c::Addr_ZPX; in.cycles = 6; InstrTable[0x16] = in;
    in.code = &c::Op_ASL; in.addr = &c::Addr_ABS; in.cycles = 6; InstrTable[0x0E] = in;
    in.code = &c::Op_ASL; in.addr = &c::Addr_ABX; in.cycles = 7; InstrTable[0x1E] = in;

    in.code = &c::Op_BCC; in.addr = &c::Addr_REL; in.cycles = 2; InstrTable[0x90] = in;

    in.code = &c::Op_BCS; in.addr = &c::Addr_REL; in.cycles = 2; InstrTable[0xB0] = in;

    in.code = &c::Op_BEQ; in.addr = &c::Addr_REL; in.cycles = 2; InstrTable[0xF0] = in;

    in.code = &c::Op_BIT; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0x24] = in;
    in.code = &c::Op_BIT; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0x2C] = in;

    in.code = &c::Op_BMI; in.addr = &c::Addr_REL; in.cycles = 2; InstrTable[0x30] = in;

    in.code = &c::Op_BNE; in.addr = &c::Addr_REL; in.cycles = 2; InstrTable[0xD0] = in;

    in.code = &c::Op_BPL; in.addr = &c::Addr_REL; in.cycles = 2; InstrTable[0x10] = in;

    in.code = &c::Op_BRK; in.addr = &c::Addr_IMP; in.cycles = 7; InstrTable[0x00] = in;

    in.code = &c::Op_BVC; in.addr = &c::Addr_REL; in.cycles = 2; InstrTable[0x50] = in;

    in.code = &c::Op_BVS; in.addr = &c::Addr_REL; in.cycles = 2; InstrTable[0x70] = in;

    in.code = &c::Op_CLC; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0x18] = in;

    in.code = &c::Op_CLD; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0xD8] = in;

    in.code = &c::Op_CLI; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0x58] = in;

    in.code = &c::Op_CLV; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0xB8] = in;

    in.code = &c::Op_CMP; in.addr = &c::Addr_IMM; in.cycles = 2; InstrTable[0xC9] = in;
    in.code = &c::Op_CMP; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0xC5] = in;
    in.code = &c::Op_CMP; in.addr = &c::Addr_ZPX; in.cycles = 4; InstrTable[0xD5] = in;
    in.code = &c::Op_CMP; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0xCD] = in;
    in.code = &c::Op_CMP; in.addr = &c::Addr_ABX; in.cycles = 4; InstrTable[0xDD] = in;
    in.code = &c::Op_CMP; in.addr = &c::Addr_ABY; in.cycles = 4; InstrTable[0xD9] = in;
    in.code = &c::Op_CMP; in.addr = &c::Addr_INX; in.cycles = 6; InstrTable[0xC1] = in;
    in.code = &c::Op_CMP; in.addr = &c::Addr_INY; in.cycles = 5; InstrTable[0xD1] = in;

    in.code = &c::Op_CPX; in.addr = &c::Addr_IMM; in.cycles = 2; InstrTable[0xE0] = in;
    in.code = &c::Op_CPX; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0xE4] = in;
    in.code = &c::Op_CPX; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0xEC] = in;

    in.code = &c::Op_CPY; in.addr = &c::Addr_IMM; in.cycles = 2; InstrTable[0xC0] = in;
    in.code = &c::Op_CPY; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0xC4] = in;
    in.code = &c::Op_CPY; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0xCC] = in;

    in.code = &c::Op_DEC; in.addr = &c::Addr_ZER; in.cycles = 5; InstrTable[0xC6] = in;
    in.code = &c::Op_DEC; in.addr = &c::Addr_ZPX; in.cycles = 6; InstrTable[0xD6] = in;
    in.code = &c::Op_DEC; in.addr = &c::Addr_ABS; in.cycles = 6; InstrTable[0xCE] = in;
    in.code = &c::Op_DEC; in.addr = &c::Addr_ABX; in.cycles = 7; InstrTable[0xDE] = in;

    in.code = &c::Op_DEX; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0xCA] = in;

    in.code = &c::Op_DEY; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0x88] = in;

    in.code = &c::Op_EOR; in.addr = &c::Addr_IMM; in.cycles = 2; InstrTable[0x49] = in;
    in.code = &c::Op_EOR; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0x45] = in;
    in.code = &c::Op_EOR; in.addr = &c::Addr_ZPX; in.cycles = 4; InstrTable[0x55] = in;
    in.code = &c::Op_EOR; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0x4D] = in;
    in.code = &c::Op_EOR; in.addr = &c::Addr_ABX; in.cycles = 4; InstrTable[0x5D] = in;
    in.code = &c::Op_EOR; in.addr = &c::Addr_ABY; in.cycles = 4; InstrTable[0x59] = in;
    in.code = &c::Op_EOR; in.addr = &c::Addr_INX; in.cycles = 6; InstrTable[0x41] = in;
    in.code = &c::Op_EOR; in.addr = &c::Addr_INY; in.cycles = 5; InstrTable[0x51] = in;

    in.code = &c::Op_INC; in.addr = &c::Addr_ZER; in.cycles = 5; InstrTable[0xE6] = in;
    in.code = &c::Op_INC; in.addr = &c::Addr_ZPX; in.cycles = 6; InstrTable[0xF6] = in;
    in.code = &c::Op_INC; in.addr = &c::Addr_ABS; in.cycles = 6; InstrTable[0xEE] = in;
    in.code = &c::Op_INC; in.addr = &c::Addr_ABX; in.cycles = 7; InstrTable[0xFE] = in;

    in.code = &c::Op_INX; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0xE8] = in;

    in.code = &c::Op_INY; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0xC8] = in;

    in.code = &c::Op_JMP; in.addr = &c::Addr_ABS; in.cycles = 3; InstrTable[0x4C] = in;
    in.code = &c::Op_JMP; in.addr = &c::Addr_IND; in.cycles = 5; InstrTable[0x6C] = in;

    in.code = &c::Op_JSR; in.addr = &c::Addr_ABS; in.cycles = 6; InstrTable[0x20] = in;

    in.code = &c::Op_LDA; in.addr = &c::Addr_IMM; in.cycles = 2; InstrTable[0xA9] = in;
    in.code = &c::Op_LDA; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0xA5] = in;
    in.code = &c::Op_LDA; in.addr = &c::Addr_ZPX; in.cycles = 4; InstrTable[0xB5] = in;
    in.code = &c::Op_LDA; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0xAD] = in;
    in.code = &c::Op_LDA; in.addr = &c::Addr_ABX; in.cycles = 4; InstrTable[0xBD] = in;
    in.code = &c::Op_LDA; in.addr = &c::Addr_ABY; in.cycles = 4; InstrTable[0xB9] = in;
    in.code = &c::Op_LDA; in.addr = &c::Addr_INX; in.cycles = 6; InstrTable[0xA1] = in;
    in.code = &c::Op_LDA; in.addr = &c::Addr_INY; in.cycles = 5; InstrTable[0xB1] = in;

    in.code = &c::Op_LDX; in.addr = &c::Addr_IMM; in.cycles = 2; InstrTable[0xA2] = in;
    in.code = &c::Op_LDX; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0xA6] = in;
    in.code = &c::Op_LDX; in.addr = &c::Addr_ZPY; in.cycles = 4; InstrTable[0xB6] = in;
    in.code = &c::Op_LDX; in.addr = &c::Addr_ABY; in.cycles = 4; InstrTable[0xBE] = in;
    in.code = &c::Op_LDX; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0xAE] = in;

    in.code = &c::Op_LDY; in.addr = &c::Addr_IMM; in.cycles = 2; InstrTable[0xA0] = in;
    in.code = &c::Op_LDY; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0xA4] = in;
    in.code = &c::Op_LDY; in.addr = &c::Addr_ZPX; in.cycles = 4; InstrTable[0xB4] = in;
    in.code = &c::Op_LDY; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0xAC] = in;
    in.code = &c::Op_LDY; in.addr = &c::Addr_ABX; in.cycles = 4; InstrTable[0xBC] = in;

    in.code = &c::Op_LSR; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0x4A] = in;
    in.code = &c::Op_LSR; in.addr = &c::Addr_ZER; in.cycles = 5; InstrTable[0x46] = in;
    in.code = &c::Op_LSR; in.addr = &c::Addr_ZPX; in.cycles = 6; InstrTable[0x56] = in;
    in.code = &c::Op_LSR; in.addr = &c::Addr_ABS; in.cycles = 6; InstrTable[0x4E] = in;
    in.code = &c::Op_LSR; in.addr = &c::Addr_ABX; in.cycles = 7; InstrTable[0x5E] = in;

    in.code = &c::Op_NOP; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0xEA] = in;

    in.code = &c::Op_ORA; in.addr = &c::Addr_IMM; in.cycles = 2; InstrTable[0x09] = in;
    in.code = &c::Op_ORA; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0x05] = in;
    in.code = &c::Op_ORA; in.addr = &c::Addr_ZPX; in.cycles = 4; InstrTable[0x15] = in;
    in.code = &c::Op_ORA; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0x0D] = in;
    in.code = &c::Op_ORA; in.addr = &c::Addr_ABX; in.cycles = 4; InstrTable[0x1D] = in;
    in.code = &c::Op_ORA; in.addr = &c::Addr_ABY; in.cycles = 4; InstrTable[0x19] = in;
    in.code = &c::Op_ORA; in.addr = &c::Addr_INX; in.cycles = 6; InstrTable[0x01] = in;
    in.code = &c::Op_ORA; in.addr = &c::Addr_INY; in.cycles = 5; InstrTable[0x11] = in;

    in.code = &c::Op_PHA; in.addr = &c::Addr_IMP; in.cycles = 3; InstrTable[0x48] = in;

    in.code = &c::Op_PHP; in.addr = &c::Addr_IMP; in.cycles = 3; InstrTable[0x08] = in;

    in.code = &c::Op_PLA; in.addr = &c::Addr_IMP; in.cycles = 4; InstrTable[0x68] = in;

    in.code = &c::Op_PLP; in.addr = &c::Addr_IMP; in.cycles = 4; InstrTable[0x28] = in;

    in.code = &c::Op_ROL; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0x2A] = in;
    in.code = &c::Op_ROL; in.addr = &c::Addr_ZER; in.cycles = 5; InstrTable[0x26] = in;
    in.code = &c::Op_ROL; in.addr = &c::Addr_ZPX; in.cycles = 6; InstrTable[0x36] = in;
    in.code = &c::Op_ROL; in.addr = &c::Addr_ABS; in.cycles = 6; InstrTable[0x2E] = in;
    in.code = &c::Op_ROL; in.addr = &c::Addr_ABX; in.cycles = 7; InstrTable[0x3E] = in;

    in.code = &c::Op_ROR; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0x6A] = in;
    in.code = &c::Op_ROR; in.addr = &c::Addr_ZER; in.cycles = 5; InstrTable[0x66] = in;
    in.code = &c::Op_ROR; in.addr = &c::Addr_ZPX; in.cycles = 6; InstrTable[0x76] = in;
    in.code = &c::Op_ROR; in.addr = &c::Addr_ABS; in.cycles = 6; InstrTable[0x6E] = in;
    in.code = &c::Op_ROR; in.addr = &c::Addr_ABX; in.cycles = 7; InstrTable[0x7E] = in;

    in.code = &c::Op_RTI; in.addr = &c::Addr_IMP; in.cycles = 6; InstrTable[0x40] = in;

    in.code = &c::Op_RTS; in.addr = &c::Addr_IMP; in.cycles = 6; InstrTable[0x60] = in;

    in.code = &c::Op_SBC; in.addr = &c::Addr_IMM; in.cycles = 2; InstrTable[0xE9] = in;
    in.code = &c::Op_SBC; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0xE5] = in;
    in.code = &c::Op_SBC; in.addr = &c::Addr_ZPX; in.cycles = 4; InstrTable[0xF5] = in;
    in.code = &c::Op_SBC; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0xED] = in;
    in.code = &c::Op_SBC; in.addr = &c::Addr_ABX; in.cycles = 4; InstrTable[0xFD] = in;
    in.code = &c::Op_SBC; in.addr = &c::Addr_ABY; in.cycles = 4; InstrTable[0xF9] = in;
    in.code = &c::Op_SBC; in.addr = &c::Addr_INX; in.cycles = 6; InstrTable[0xE1] = in;
    in.code = &c::Op_SBC; in.addr = &c::Addr_INY; in.cycles = 5; InstrTable[0xF1] = in;

    in.code = &c::Op_SEC; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0x38] = in;

    in.code = &c::Op_SED; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0xF8] = in;

    in.code = &c::Op_SEI; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0x78] = in;

    in.code = &c::Op_STA; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0x85] = in;
    in.code = &c::Op_STA; in.addr = &c::Addr_ZPX; in.cycles = 4; InstrTable[0x95] = in;
    in.code = &c::Op_STA; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0x8D] = in;
    in.code = &c::Op_STA; in.addr = &c::Addr_ABX; in.cycles = 5; InstrTable[0x9D] = in;
    in.code = &c::Op_STA; in.addr = &c::Addr_ABY; in.cycles = 5; InstrTable[0x99] = in;
    in.code = &c::Op_STA; in.addr = &c::Addr_INX; in.cycles = 6; InstrTable[0x81] = in;
    in.code = &c::Op_STA; in.addr = &c::Addr_INY; in.cycles = 6; InstrTable[0x91] = in;

    in.code = &c::Op_STX; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0x86] = in;
    in.code = &c::Op_STX; in.addr = &c::Addr_ZPY; in.cycles = 4; InstrTable[0x96] = in;
    in.code = &c::Op_STX; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0x8E] = in;

    in.code = &c::Op_STY; in.addr = &c::Addr_ZER; in.cycles = 3; InstrTable[0x84] = in;
    in.code = &c::Op_STY; in.addr = &c::Addr_ZPX; in.cycles = 4; InstrTable[0x94] = in;
    in.code = &c::Op_STY; in.addr = &c::Addr_ABS; in.cycles = 4; InstrTable[0x8C] = in;

    in.code = &c::Op_TAX; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0xAA] = in;

    in.code = &c::Op_TAY; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0xA8] = in;

    in.code = &c::Op_TSX; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0xBA] = in;

    in.code = &c::Op_TXA; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0x8A] = in;

    in.code = &c::Op_TXS; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0x9A] = in;

    in.code = &c::Op_TYA; in.addr = &c::Addr_IMP; in.cycles = 2; InstrTable[0x98] = in;
    }
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

void ct6502::PushToStack(uint8_t v) {
    write(STACK_ADDR + stk_ptr, v);
    if (stk_ptr == 0x00) stk_ptr = 0xFF;
    else stk_ptr--;
}

uint8_t ct6502::PopFromStack() {
    if (stk_ptr == 0xFF) stk_ptr = 0x00;
    else stk_ptr++;
    return read(STACK_ADDR + stk_ptr);
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
        PushToStack((pc >> 8) & 0x00FF);
        PushToStack(pc & 0x00FF);

        SetFlag(BREAK, false);
//        SetFlag(CONSTANT, true);
        SetFlag(INTERRUPT, true);
        PushToStack(status);

        addr_abs = 0xFFFE;
        uint16_t lo = read(addr_abs + 0);
        uint16_t hi = read(addr_abs + 1);
        pc = (hi  << 8) | lo;

        cycles = 7;
    }
}

void ct6502::nmi() {
    PushToStack((pc >> 8) & 0x00FF);
    PushToStack(pc & 0x00FF);

    SetFlag(BREAK, false);
//    SetFlag(CONSTANT, true);
    SetFlag(INTERRUPT, true);
    PushToStack(status);

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
    fetch();
    tmp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(CARRY);
    SetFlag(ZERO, (tmp & 0x00FF) == 0);

    if (GetFlag(DECIMAL)) {
        if (((a & 0x000F) + (fetched & 0x000F) + (uint16_t)GetFlag(CARRY)) > 9) tmp += 6;
        SetFlag(NEGATIVE, tmp & 0x80);
        SetFlag(OVERFLOW, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)tmp) & 0x0080));
        if (tmp > 0x99) {
            tmp += 96;
        }
        SetFlag(CARRY, tmp > 0x99);
    }
    else {
        SetFlag(CARRY, tmp > 0xFF);
        SetFlag(NEGATIVE, tmp & 0x80);
        SetFlag(OVERFLOW, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)tmp)) & 0x0080);
    }
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
    PushToStack((pc >> 8) & 0x00FF);
    PushToStack(pc & 0x00FF);
    PushToStack(status | CONSTANT | BREAK);
    SetFlag(INTERRUPT, true);
    pc = ((uint16_t)read(0xFFFF) << 8) | ((uint16_t)read(0xFFFE));
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

    PushToStack((pc >> 8) & 0x00FF);
    PushToStack(pc & 0x00FF);

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
    PushToStack(a);
    return 0;
}

uint8_t ct6502::Op_PHP() {
    PushToStack(status | BREAK | CONSTANT);
    return 0;
}

uint8_t ct6502::Op_PLA() {
    a = PopFromStack();
    SetFlag(ZERO, a == 0x00);
    SetFlag(NEGATIVE, a & 0x80);
    return 0;
}

uint8_t ct6502::Op_PLP() {
    status = PopFromStack() | BREAK | CONSTANT;
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
    SetFlag(CARRY, fetched & 0x0001);
    SetFlag(ZERO, (tmp & 0x00FF) == 0);
    SetFlag(NEGATIVE, tmp & 0x0080);
    if (InstrTable[opcode].addr == &ct6502::Addr_IMP)
        a = tmp & 0x00FF;
    else
        write(addr_abs, tmp & 0x00FF);
    return 0;
}

uint8_t ct6502::Op_RTI() {
    status = PopFromStack() | CONSTANT | BREAK;

    pc = (uint16_t)PopFromStack();
    pc |= (uint16_t)PopFromStack() << 8;
    return 0;
}

uint8_t ct6502::Op_RTS() {
    pc = (uint16_t)PopFromStack();
    pc |= (uint16_t)PopFromStack() << 8;

    pc++;
    return 0;
}

uint8_t ct6502::Op_SBC() {
    fetch();
    uint16_t val = ((uint16_t)fetched) ^ 0x00FF;
    tmp = (uint16_t)a + val + (uint16_t)GetFlag(CARRY);
    SetFlag(ZERO, ((tmp & 0x00FF) == 0));
    SetFlag(NEGATIVE, tmp & 0x0080);
    SetFlag(OVERFLOW, (tmp ^ (uint16_t)a) & (tmp ^ val) & 0x0080);
    if (GetFlag(DECIMAL)) {
        if ( (((uint16_t)a & 0x0F) - (uint16_t)(GetFlag(CARRY) ? 0 : 1)) < (fetched & 0x0F))
            tmp -= 6;
        if (tmp > 0x99) {
            tmp -= 0x60;
        }
    }

    SetFlag(CARRY, tmp & 0xFF00);
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
