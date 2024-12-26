# PCore Compiler

The **PCore Compiler** is an ambitious project to create a robust compiler for the **PCore programming language**. The language is designed with a custom syntax that focuses on explicit typing, manual memory management, and streamlined constructs for modern software development. This repository provides the core tools needed to tokenize, parse, and eventually generate LLVM IR for efficient execution.

## Features

### Current Functionality
- **Tokenizer**: Breaks down PCore source code into meaningful tokens.
- **Parser**: Converts tokens into an Abstract Syntax Tree (AST).
- **Syntax Validation**: PCore-specific syntax is fully documented in the `documentation` folder.

### Planned Features
- **AST Analysis**: Enhance the structure and semantic validity of the generated AST.
- **LLVM Code Generation**: Transform the AST into optimized LLVM Intermediate Representation (IR).

## Project Goals
The PCore Compiler aims to:
1. Provide a seamless development experience for PCore programmers.
2. Leverage LLVM for efficient, portable machine code generation.
3. Serve as a foundation for experimenting with language design and compiler technologies.

## Getting Started

### Prerequisites
- **C++ Compiler**: Requires C++17 or later.
- **LLVM**: For eventual IR generation (currently not implemented).
- **CMake**: For building the project.

### Building the Project
1. Clone the repository:
   ```bash
   git clone https://github.com/LPC4/PCore-Compiler-1.4.C.git
   cd PCore-Compiler-1.4.C
   ```
2. Create a build directory and run CMake:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
3. Run the compiler (currently limited to tokenizing and parsing):
   ```bash
   ./PCoreCompiler <source-file>
   ```

### Documentation
Detailed documentation on PCore’s syntax, design goals, and examples can be found in the `documentation` folder.

## Contributing
Contributions are welcome! If you’d like to help improve the project, please:
1. Fork the repository.
2. Create a feature branch.
3. Submit a pull request with your changes.

## Roadmap
- Complete the parser with advanced syntax handling.
- Implement semantic analysis for AST validation.
- Generate LLVM IR and optimize the code generation pipeline.
- Add test cases for the tokenizer, parser, and LLVM backend.

## License
This project is licensed under the [MIT License](LICENSE).

## Contact
For questions or discussions, please reach out via GitHub Issues. Thank you for your interest in the PCore Compiler!

---


