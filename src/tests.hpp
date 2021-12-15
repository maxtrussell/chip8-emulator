#pragma once

#include "chip8.hpp"

class Tests {
public:
    Chip8 vm;

    Tests(bool);
    void reset();
    void run_tests();
private:
    bool test_00E0();
    bool test_00EE();
    bool test_1NNN();
    bool test_2NNN();
    bool test_3XNN();
    bool test_4XNN();
    bool test_5XY0();
    bool test_6XNN();
    bool test_7XNN();
    bool test_8XY0();
    bool test_8XY1();
    bool test_8XY2();
    bool test_8XY3();
    bool test_8XY4();
    bool test_8XY5();
    bool test_8XY6();
    bool test_8XY7();
    bool test_8XYE();
    bool test_9XY0();
    bool test_ANNN();
    bool test_BNNN();
    bool test_CXNN();
    bool test_DXYN();
    bool test_EX9E();
    bool test_EXA1();
    bool test_FX07();
    bool test_FX0A();
    bool test_FX15();
    bool test_FX18();
    bool test_FX1E();
    bool test_FX29();
    bool test_FX33();
    bool test_FX55();
    bool test_FX65();
};
