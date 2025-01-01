#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "../include/CodeGenerator.h"
#include "../include/Parser.h"
#include "../include/Tokenizer.h"

enum class ExitCode : std::uint8_t { SUCCESS = 0, TOKENIZER_ERROR = 1, PARSER_ERROR = 2, IR_ERROR = 3 };

const static std::string NAME = "PCore Compiler";
const static std::string VERSION = "1.4.0";
const static std::string AUTHOR = "liamd";

static auto compile(const std::string &filepath) -> ExitCode;
static void compileLLVMOutputAndRun();

int main(int argc, char *argv[]) {
    // todo: add command line arguments

    std::cout << NAME << " v" << VERSION << " by " << AUTHOR << '\n';

    const ExitCode exitCode = compile("../resources/test.pc");

    if (exitCode == ExitCode::SUCCESS) {
        compileLLVMOutputAndRun();
    }

    return static_cast<int>(exitCode);
}

static auto compile(const std::string &filepath) -> ExitCode {
    std::vector<Token> tokens;

    // Tokenize source code
    try {
        Tokenizer tokenizer(filepath);
        tokens = tokenizer.tokenize();

        // For debugging purposes
        for (const Token &token : tokens) {
            token.print();
        }

        std::cout << "//---------------------- Tokenization successful ----------------------//\n";
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return ExitCode::TOKENIZER_ERROR;
    }

    // Parse tokens
    std::unique_ptr<Program> program;
    try {
        Parser parser;
        program = parser.parse(tokens);

        program->print("");

        std::cout << "//---------------------- Parsing successful ----------------------//\n";
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return ExitCode::PARSER_ERROR;
    }

    // Generate intermediate representation
    try {
        CodeGenerator codeGenerator;
        codeGenerator.generateCode(program);

        std::cout << "//---------------------- IR generation successful ----------------------//\n";
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return ExitCode::IR_ERROR;
    }

    return ExitCode::SUCCESS;
}

static void compileLLVMOutputAndRun() {
    // Command to execute (compile LLVM IR using llc and clang, run the executable)
    const std::string command = "powershell.exe -Command "
                                "\""
                                "cd E:\\Utility\\code\\projects; "          // Change directory
                                "llc -filetype=obj output.ll -o output.o; " // Generate object file
                                "clang output.o -o output.exe; "            // Link object file
                                "./output.exe; "                            // Run executable
                                "echo $LASTEXITCODE"                        // Print exit code
                                "\"";

    if (const int result = system(command.c_str()); result == 0) {
        std::cout << "Commands executed successfully." << '\n';
    } else {
        std::cerr << "Error executing commands. Exit code: " << result << '\n';
    }
}
