#include <string>

#include "../include/CodeGenerator.h"
#include "../include/Parser.h"
#include "../include/Tokenizer.h"

enum ExitCode : std::uint8_t { SUCCESS = 0, TOKENIZER_ERROR = 1, PARSER_ERROR = 2, IR_ERROR = 3 };

const static std::string NAME = "PCore Compiler";
const static std::string VERSION = "1.4.0";
const static std::string AUTHOR = "liamd";

static int compile(const std::string &filepath) {
    std::vector<Token> tokens;

    // Tokenize source code
    try {
        Tokenizer tokenizer(filepath);
        tokens = tokenizer.tokenize();

        // For debugging purposes
        for (const Token &token : tokens) {
            token.print();
        }

        printf("//---------------------- Tokenization successful ----------------------//\n");
    } catch (const std::runtime_error &e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return TOKENIZER_ERROR;
    }

    // Parse tokens
    std::unique_ptr<Program> program;
    try {
        Parser parser;
        program = parser.parse(tokens);

        program->print("");

        printf("//---------------------- Parsing successful ----------------------//\n");

    } catch (const std::runtime_error &e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return PARSER_ERROR;
    }

    // Generate intermediate representation
    try {
        CodeGenerator codeGenerator;
        codeGenerator.generateCode(program);

        printf("//---------------------- IR generation successful ----------------------//\n");

    } catch (const std::runtime_error &e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return IR_ERROR;
    }

    // todo: generate intermediate representation in LLVM IR
    // todo: generate executable

    return SUCCESS;
}

void compileAndRun() {
    // Command to execute
    std::string command = "powershell.exe -Command \"cd E:\\Utility\\code\\projects; "
                          "llc -filetype=obj output.ll -o output.o; "
                          "clang output.o -o output.exe; "
                          "./output.exe; "
                          "echo $LASTEXITCODE\"";

    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "Commands executed successfully." << std::endl;
    } else {
        std::cerr << "Error executing commands. Exit code: " << result << std::endl;
    }
}

int main(int argc, char *argv[]) {
    // todo: add command line arguments

    printf("%s v%s by %s\n", NAME.c_str(), VERSION.c_str(), AUTHOR.c_str());

    const int exitCode = compile("../resources/test.pc");

    if (exitCode == SUCCESS) {
        compileAndRun();
    }

    return exitCode;
}

