#include <string>

#include "../include/Parser.h"
#include "../include/Tokenizer.h"

enum ExitCode : std::uint8_t {
    SUCCESS = 0,
    TOKENIZER_ERROR = 1,
    PARSER_ERROR = 2
};

const static std::string NAME = "PCore Compiler";
const static std::string VERSION = "1.4.0";
const static std::string AUTHOR = "liamd";

static int compile(const std::string &filepath) {
    std::vector<Token> tokens;

    // Tokenize source code
    try {
        Tokenizer tokenizer(filepath);
        tokens = tokenizer.tokenize();
    } catch (const std::runtime_error &e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return TOKENIZER_ERROR;
    }

    // For debugging purposes
    for (const Token &token : tokens) {
        token.print();
    }

    // Parse tokens
    try {
        Parser parser;
        parser.parse(tokens);
    } catch (const std::runtime_error &e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return PARSER_ERROR;
    }

    return SUCCESS;
}

int main(int argc, char* argv[]) {
    // todo: add command line arguments

    printf("%s v%s by %s\n", NAME.c_str(), VERSION.c_str(), AUTHOR.c_str());

    return compile("../resources/test.pc");
}
