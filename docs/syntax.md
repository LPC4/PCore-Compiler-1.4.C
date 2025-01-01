# PCore Syntax Documentation

## 1. Program Structure
- A program starts with the keyword `program` followed by the program's name.
- Example:
  ```c++
  program main;
  
  main {
      print("Hello, World!");
  }
  ```

## 2. Comments
- Single-line comments begin with `//`.
  ```c++
  // This is a single-line comment
  int x = 42; // Inline comment
  ```

## 3. Variables
- Explicit typing is required.
- Implicit type conversions occur where possible.
- Example:
  ```c++
  int x = 42;
  float y = 3.14;
  char* name = "Liam";
  bit flag = 1;
  int x = y; // Implicit conversion, x = 3
  ```

## 4. Functions
- Functions can optionally use the `func` keyword.
- Function syntax:
  ```c++
  (params) -> return_type
  func_name {
      body
  }
  ```
- Example:
  ```c++
  (int x, int y) -> int 
  func add {
      return x + y;
  }
    
  // Implicit parameters and return type
  main {
      int result = add(2, 3);
      print(result);
  }
  ```

## 5. Control Flow
- `if-else`:
  ```c++
  if condition {
      // True block
  } else {
      // False block
  }
  ```
- `while` loop:
  ```c++
  while condition {
      // Loop body
  }
  ```

## 6. Input/Output
- Basic output using `print`.
- Example:
  ```c++
  print("Hello, World!");
  ```

## 7. Operators
- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Logical: `&&`, `||`, `!`
- Assignment: `=`, `+=`, `-=`, `*=`, `/=`

## 8. Arrays
- Declared using `[]`.
- Example:
  ```c++
  [int, 5] numbers = [1, 2, 3, 4, 5];
  ```

## 9. Modules and Imports
- Use `program` for naming the program.
- Use `import` to include other modules.
- Example:
  ```c++
  program main

  import utils

  main {
      print("Using utils module");
  }
  ```

## 10. Memory Management
- Manual memory management.
- Standard library provides `alloc` and `free` functions.
- Example:
  ```c++
  ptr x = alloc(sizeof(int) * 10); // Allocate memory for 10 integers
  *x = 42; // Assign value
  *(x + 1) = 43; // Assign value
  print(*x); // Access value
  free(x); // Free memory
  ```
- Invalid memory access will panic at runtime.
- See memory.md for more details.

## 11. Error Handling
- No explicit error handling constructs (e.g., try-catch).
- Errors are handled using return types.
- Example:
  ```c++
  (int x) -> int 
  divide_safe {
      if (x == 0) {
          return -1; // Error indicator
      }
      return 42 / x;
  }
  ```

## 12. Additional Constructs
- Future pipeline syntax (for chaining operations):
  ```c++
  pipeline {
      input -> transform -> validate -> output;
  }
  ```

