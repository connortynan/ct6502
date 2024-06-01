#include "EmuTest.cpp"

int main() {
    EmuTest6502 demo;

    if (!demo.init())
        return 1;
    demo.loop();

    return 0;
}
