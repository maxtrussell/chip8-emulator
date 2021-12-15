#include "tests.hpp"

#include <iostream>

#define ASSERT_TRUE(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; }

Tests::Tests(bool debug) {
    vm = Chip8(debug);
    vm.init();
};

void Tests::reset() {
    vm.init();
}

void Tests::run_tests() {
    test_00EE();
    reset();

    test_1NNN();
    reset();

    test_2NNN();
    reset();

    test_3XNN();
    reset();

    test_4XNN();
    reset();

    test_5XY0();
    reset();

    test_6XNN();
    reset();

    test_7XNN();
    reset();

    test_8XY0();
    reset();

    test_8XY1();
    reset();

    test_8XY2();
    reset();

    test_8XY3();
    reset();

    test_8XY4();
    reset();

    test_8XY5();
    reset();

    test_8XY6();
    reset();

    test_8XY7();
    reset();

    test_8XYE();
    reset();

    test_9XY0();
    reset();

    test_ANNN();
    reset();

    test_BNNN();
    reset();

    test_EX9E();
    reset();

    test_EXA1();
    reset();

    test_FX07();
    reset();

    test_FX0A();
    reset();

    test_FX15();
    reset();

    test_FX18();
    reset();

    test_FX1E();
    reset();

    test_FX29();
    reset();

    test_FX33();
    reset();

    test_FX55();
    reset();

    test_FX65();
    reset();
}

bool Tests::test_00E0() {
    // Setup
    for (int i = 0; i < 2048; i++) {
        vm.gfx[i] = 1;
    }
    unsigned char opcode[] = {0x00, 0xE0};
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();
    
    // Assertions
    ASSERT_TRUE(vm.drawFlag == true);
    for (int i = 0; i < 2048; i++) {
        ASSERT_TRUE(vm.gfx[i] == 0);
    }
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}

bool Tests::test_00EE() {
    // Setup
    unsigned char opcode[] = {0x00, 0xEE};
    vm.load(opcode, 2);
    vm.stack[vm.sp++] = 0xFFF0;

    // Run
    vm.emulate_cycle();
    
    // Assertions
    ASSERT_TRUE(vm.sp == 0);
    ASSERT_TRUE(vm.pc == 0xFFF2);
    return true;
}

bool Tests::test_1NNN() {
    // Setup
    unsigned char opcode[] = {0x1F, 0xFF};
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();
    
    // Assertions
    ASSERT_TRUE(vm.pc == 0x0FFF);
    return true;
}

bool Tests::test_2NNN() {
    // Setup
    unsigned char opcode[] = {0x2F, 0xFF};
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();
    
    // Assertions
    ASSERT_TRUE(vm.pc == 0x0FFF);
    ASSERT_TRUE(vm.stack[0] == 0x0200);
    return true;
}

bool Tests::test_3XNN() {
    // Setup; VX == NN
    unsigned char opcode[] = {0x31, 0xFF};
    vm.V[0x1] = 0xFF;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();
    
    // Assertions
    ASSERT_TRUE(vm.pc == (0x200 + 4));

    // ---------------
    // Setup; VX != NN
    reset();
    vm.V[0x1] = 0x01;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();
    
    // Assertions
    ASSERT_TRUE(vm.pc == (0x200 + 2));
    return true;
}

bool Tests::test_4XNN() {
    // Setup; VX == NN
    unsigned char opcode[] = {0x41, 0xFF};
    vm.V[0x1] = 0xFF;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();
    
    // Assertions
    ASSERT_TRUE(vm.pc == (0x200 + 2));

    // ---------------
    // Setup; VX != NN
    reset();
    vm.V[0x1] = 0x01;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();
    
    // Assertions
    ASSERT_TRUE(vm.pc == (0x200 + 4));
    return true;
}

bool Tests::test_5XY0() {
    // Setup; VX == VY
    unsigned char opcode[] = {0x51, 0x20};
    vm.V[0x1] = 0xFF;
    vm.V[0x2] = 0xFF;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();
    
    // Assertions
    ASSERT_TRUE(vm.pc == (0x200 + 4));

    // ---------------
    // Setup; VX != NN
    reset();
    vm.V[0x1] = 0x01;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();
    
    // Assertions
    ASSERT_TRUE(vm.pc == (0x200 + 2));
    return true;
}

bool Tests::test_6XNN() {
    // Setup
    unsigned char opcode[] = {0x61, 0xFF};
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0xFF);
    ASSERT_TRUE(vm.pc == 0x200 + 2)
    return true;
} 

bool Tests::test_7XNN() {
    // Setup; case 1 no carry
    unsigned char opcode[] = {0x71, 0xFE};
    vm.V[0x1] = 1;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0xFF);
    ASSERT_TRUE(vm.V[0xF] == 0);
    ASSERT_TRUE(vm.pc == 0x200 + 2)

    // Setup; case 2, carry don't modify VF
    reset();
    vm.V[0x1] = 2;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0x00);
    ASSERT_TRUE(vm.V[0xF] == 0);
    ASSERT_TRUE(vm.pc == 0x200 + 2)
    return true;
} 

bool Tests::test_8XY0() {
    // Setup; case 1 no carry
    unsigned char opcode[] = {0x81, 0x20};
    vm.V[0x2] = 0xFF;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0xFF);
    ASSERT_TRUE(vm.pc == 0x200 + 2)
    return true;
}

bool Tests::test_8XY1() {
    // Setup; case 1 no carry
    unsigned char opcode[] = {0x81, 0x21};
    vm.V[0x1] = 0x0F;
    vm.V[0x2] = 0xF0;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0xFF);
    ASSERT_TRUE(vm.pc == 0x200 + 2)
    return true;
}

bool Tests::test_8XY2() {
    // Setup; case 1 no carry
    unsigned char opcode[] = {0x81, 0x22};
    vm.V[0x1] = 0xFF;
    vm.V[0x2] = 0xF0;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0xF0);
    ASSERT_TRUE(vm.pc == 0x200 + 2)
    return true;
}

bool Tests::test_8XY3() {
    // Setup; case 1 no carry
    unsigned char opcode[] = {0x81, 0x23};
    vm.V[0x1] = 0xFF;
    vm.V[0x2] = 0xF0;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0x0F);
    ASSERT_TRUE(vm.pc == 0x200 + 2)
    return true;
}

bool Tests::test_8XY4() {
    // Setup; case 1 no carry
    unsigned char opcode[] = {0x81, 0x24};
    vm.V[0x1] = 0xF0;
    vm.V[0x2] = 0x0F;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0xFF);
    ASSERT_TRUE(vm.V[0xF] == 0);
    ASSERT_TRUE(vm.pc == 0x200 + 2);

    // Setup; case 2 carry
    reset();
    vm.V[0x1] = 0xF1;
    vm.V[0x2] = 0x0F;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0x00);
    ASSERT_TRUE(vm.V[0xF] == 1);
    ASSERT_TRUE(vm.pc == 0x200 + 2)
    return true;
}

bool Tests::test_8XY5() {
    // Setup; case 1 no borrow
    unsigned char opcode[] = {0x81, 0x25};
    vm.V[0x1] = 0xFF;
    vm.V[0x2] = 0x0F;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0xF0);
    ASSERT_TRUE(vm.V[0xF] == 1);
    ASSERT_TRUE(vm.pc == 0x200 + 2);

    // Setup; case 2 carry
    reset();
    vm.V[0x1] = 0xF0;
    vm.V[0x2] = 0xF1;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0xFF);
    ASSERT_TRUE(vm.V[0xF] == 0);
    ASSERT_TRUE(vm.pc == 0x200 + 2)
    return true;
}

bool Tests::test_8XY6() {
    // Setup
    unsigned char opcode[] = {0x81, 0x26};
    vm.V[0x1] = 0xFF;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == (0xFF >> 1));
    ASSERT_TRUE(vm.V[0xF] == 1);
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}

bool Tests::test_8XY7() {
    // Setup; case 1 no borrow
    unsigned char opcode[] = {0x81, 0x27};
    vm.V[0x1] = 0x0F;
    vm.V[0x2] = 0xFF;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0xF0);
    ASSERT_TRUE(vm.V[0xF] == 1);
    ASSERT_TRUE(vm.pc == 0x200 + 2);

    // Setup; case 2 borrow
    reset();
    vm.V[0x1] = 0xF1;
    vm.V[0x2] = 0xF0;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0xFF);
    ASSERT_TRUE(vm.V[0xF] == 0);
    ASSERT_TRUE(vm.pc == 0x200 + 2)
    return true;
}

bool Tests::test_8XYE() {
    // Setup
    unsigned char opcode[] = {0x81, 0x2E};
    vm.V[0x1] = 0xFF;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0xFE);
    ASSERT_TRUE(vm.V[0xF] == 1);
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}

bool Tests::test_9XY0() {
    // Setup, case 1 VX == VY
    unsigned char opcode[] = {0x91, 0x20};
    vm.V[0x1] = 0xFF;
    vm.V[0x2] = 0xFF;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.pc == 0x200 + 2);

    // Setup, case 2 VX != VY
    reset();
    vm.load(opcode, 2);
    vm.V[0x1] = 0xFF;
    vm.V[0x2] = 0x00;

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.pc == 0x200 + 4);
    return true;
}

bool Tests::test_ANNN() {
    // Setup
    unsigned char opcode[] = {0xAF, 0xFF};
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.I == 0xFFF);
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}

bool Tests::test_BNNN() {
    // Setup
    unsigned char opcode[] = {0xBF, 0xFE};
    vm.V[0] = 0x01;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.pc == 0xFFF);
    return true;
}

bool Tests::test_EX9E() {
    // Setup, case 1 key is pressed
    unsigned char opcode[] = {0xE1, 0x9E};
    vm.V[1] = 1;
    vm.set_key(1, true);
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.pc == 0x200 + 4);

    // Setup, case 1 key is not pressed
    reset();
    vm.V[1] = 1;
    vm.set_key(1, false);
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}

bool Tests::test_EXA1() {
    // Setup, case 1 key is pressed
    unsigned char opcode[] = {0xE1, 0xA1};
    vm.V[1] = 1;
    vm.set_key(1, true);
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.pc == 0x200 + 2);

    // Setup, case 1 key is not pressed
    reset();
    vm.V[1] = 1;
    vm.set_key(1, false);
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.pc == 0x200 + 4);
    return true;
}

bool Tests::test_FX07() {
    // Setup
    unsigned char opcode[] = {0xF1, 0x07};
    vm.delay_timer = 10;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 10);
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}

bool Tests::test_FX0A() {
    // Setup
    unsigned char opcode[] = {0xF1, 0x0A};
    vm.load(opcode, 2);

    // Run
    for (int i = 0; i < 3; i++) {
        if (i == 2) {
            vm.set_key(1, true);
        }
        vm.emulate_cycle();
    }

    // Assertions
    ASSERT_TRUE(vm.V[1] == 1);
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}

bool Tests::test_FX15() {
    // Setup
    unsigned char opcode[] = {0xF1, 0x15};
    vm.V[1] = 0xFF;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0xFF);
    ASSERT_TRUE(vm.delay_timer == 0xFE);
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}

bool Tests::test_FX18() {
    // Setup
    unsigned char opcode[] = {0xF1, 0x18};
    vm.V[1] = 0xFF;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 0xFF);
    ASSERT_TRUE(vm.sound_timer == 0xFE);
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}

bool Tests::test_FX1E() {
    // Setup
    unsigned char opcode[] = {0xF1, 0x1E};
    vm.I = 0xFE;
    vm.V[1] = 0x1;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 1);
    ASSERT_TRUE(vm.V[0xF] == 0);
    ASSERT_TRUE(vm.I == 0xFF);
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}

bool Tests::test_FX29() {
    // Setup
    unsigned char opcode[] = {0xF1, 0x29};
    vm.V[1] = 1;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 1);
    ASSERT_TRUE(vm.I == 5);
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}

bool Tests::test_FX33() {
    // Setup
    unsigned char opcode[] = {0xF1, 0x33};
    vm.V[1] = 150;
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 150);
    ASSERT_TRUE(vm.memory[vm.I] == 1);
    ASSERT_TRUE(vm.memory[vm.I+1] == 5);
    ASSERT_TRUE(vm.memory[vm.I+2] == 0);
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}

bool Tests::test_FX55() {
    // Setup
    unsigned char opcode[] = {0xF8, 0x55};
    vm.I = 5;
    for (int i = 0; i <= 8; i++) {
        vm.V[i] = i;
    }
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.V[1] == 1);
    ASSERT_TRUE(vm.I == 5);
    for (int i = 0; i <= 8; i++) {
        ASSERT_TRUE(vm.memory[vm.I+i] == i);
    }
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}

bool Tests::test_FX65() {
    // Setup
    unsigned char opcode[] = {0xF8, 0x65};
    vm.I = 5;
    for (int i = 0; i <= 8; i++) {
        vm.memory[vm.I + i] = i;
    }
    vm.load(opcode, 2);

    // Run
    vm.emulate_cycle();

    // Assertions
    ASSERT_TRUE(vm.I == 5);
    for (int i = 0; i <= 8; i++) {
        ASSERT_TRUE(vm.V[i] == i);
    }
    ASSERT_TRUE(vm.pc == 0x200 + 2);
    return true;
}
