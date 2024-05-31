#include "cpuViewer.cpp"

int main() {
    demo_6502 demo;

    if (!demo.init())
        return 1;

    demo.loop();

    return 0;
}
