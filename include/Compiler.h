#pragma once
#include <string>

#include "Tokenizer.h"

class Compiler {
public:
    Compiler(std::string name, std::string version, std::string author, const std::string &filepath);

private:
    std::string m_name;
    std::string m_version;
    std::string m_author;

    Tokenizer m_tokenizer;
};
