Here's a `.md` file that defines the syntax of your stack-based IR. This Markdown document provides a detailed description of each instruction and the structure of functions.

```markdown
# Stack-Based Intermediate Representation (IR) Syntax

## Overview

This document describes the syntax of the stack-based Intermediate Representation (IR) for our virtual machine. The IR consists of a series of instructions executed in a stack-based manner, where operations primarily involve pushing to and popping from the stack.

## Instructions

### Arithmetic Instructions

- **ADD**: Pops the top two values from the stack, adds them, and pushes the result.
- **SUB**: Pops the top two values from the stack, subtracts the second from the first, and pushes the result.
- **MUL**: Pops the top two values from the stack, multiplies them, and pushes the result.
- **DIV**: Pops the top two values from the stack, divides the second by the first, and pushes the result.

### Comparison Instructions

- **EQ**: Pops the top two values from the stack, compares them for equality, and pushes the result (1 if equal, 0 otherwise).
- **NE**: Pops the top two values from the stack, compares them for inequality, and pushes the result (1 if not equal, 0 otherwise).
- **LT**: Pops the top two values from the stack, checks if the second is less than the first, and pushes the result.
- **GT**: Pops the top two values from the stack, checks if the second is greater than the first, and pushes the result.
- **LE**: Pops the top two values from the stack, checks if the second is less than or equal to the first, and pushes the result.
- **GE**: Pops the top two values from the stack, checks if the second is greater than or equal to the first, and pushes the result.

### Logical Instructions

- **AND**: Pops the top two values from the stack, performs a logical AND, and pushes the result.
- **OR**: Pops the top two values from the stack, performs a logical OR, and pushes the result.
- **NOT**: Pops the top value from the stack, performs a logical NOT, and pushes the result.

### Memory Instructions

- **LOAD**: Pops an address from the stack, loads the value at that address, and pushes it onto the stack.
- **STORE**: Pops a value and an address from the stack, stores the value at the address.

### Control Flow Instructions

- **JMP <label>**: Unconditional jump to the specified label.
- **JZ <label>**: Pops the top value from the stack and jumps to the specified label if the value is zero.
- **JNZ <label>**: Pops the top value from the stack and jumps to the specified label if the value is not zero.
- **CALL <func>**: Calls the specified function.
- **RET**: Returns from the current function.

### Stack Instructions

- **PUSH <value>**: Pushes the specified value onto the stack.
- **POP**: Pops the top value from the stack.

### Miscellaneous Instructions

- **NOP**: No operation.
- **HLT**: Halts the execution of the program.

## Function Syntax

Functions are defined with a label and consist of a series of instructions. Each function begins with a label and ends with the `RET` instruction.

### Example Function Definition

```
FUNC main:
    PUSH 10
    PUSH 20
    ADD
    STORE result
    RET

FUNC add:
    ; Assumes the values to add are on top of the stack
    ADD
    RET
```

In this example, the `main` function pushes two values onto the stack, adds them, stores the result, and returns. The `add` function adds the two top values on the stack and returns the result.

## Labels

Labels are used to define jump targets and function names. They must be unique within the program.

### Example with Labels

```plaintext
FUNC main:
    PUSH 0
    PUSH 10
loop_start:
    POP
    JZ end_loop
    PUSH 1
    SUB
    JMP loop_start
end_loop:
    RET
```

This example demonstrates a simple loop that decrements a counter from 10 to 0.

## Summary

This document outlines the syntax and semantics of our stack-based IR, including arithmetic, comparison, logical, memory, control flow, stack, and miscellaneous instructions. Functions and labels are also defined to structure the program.

By following this syntax, you can create programs that leverage the stack-based architecture of our virtual machine.
