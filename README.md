# ct6502: MOS6502 emulator in C++

Nothing new for a 6502 emulator, I just wanted to learn about it.
I followed along with javidx9's tutorial video and [project](https://github.com/OneLoneCoder/olcNES), as well as [gianlucag's implementation](https://github.com/gianlucag/mos6502/tree/master).

(Currently) only implements legal opcodes. All illegal opcodes terminate the program but can be changed to a NOP relatively easily.
Also has a simple testing program to step through instructions and view memory, and the 'base' branch only contains the ct6502 class files plus a abstract Bus class.

Was tested against [Klaus2m5's 6502 functional test program](https://github.com/Klaus2m5/6502_65C02_functional_tests)

# To use:
Implement a class that derived from the Bus class (or create a new one from scratch, either works), and call the clock function of the ct6502 accordingly.
(test_program shows a simple implementation of it in SimpleRamBus.h)