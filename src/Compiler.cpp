#include "../include/Compiler.h"

Compiler::Compiler(std::string name, std::string version, std::string author, const std::string &filepath) :
    m_name(std::move(name)), m_version(std::move(version)), m_author(std::move(author)), m_tokenizer(filepath) {
    printf("%s %sby %s\n", m_name.c_str(), version.c_str(), m_author.c_str());

    std::vector<Token> tokens = m_tokenizer.tokenize();

    for (const Token &token : tokens) {
        printf("Token: %s\n", token.getValue().c_str());
    }
}
