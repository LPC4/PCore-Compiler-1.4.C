#pragma once

#include <cstdint>

enum class Instruction : std::uint8_t {
    // Arithmetic instructions
    ADD,
    SUB,
    MUL,
    DIV,

    // Comparison instructions
    EQ,
    NE,
    LT,
    GT,
    LE,
    GE,

    // Logical instructions
    AND,
    OR,
    NOT,

    // Memory instructions
    LOAD,
    STORE,

    // Control flow instructions
    JMP,
    JZ,
    JNZ,
    CALL,
    RET,

    // Stack instructions
    PUSH,
    POP,

    // Miscellaneous instructions
    NOP,
    HLT,
};
