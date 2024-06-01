cmake_minimum_required(VERSION 3.28)
project(ct-vic-20)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        main.cpp
        Bus.cpp
        ct6502.cpp
        EmuTest.cpp
)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})