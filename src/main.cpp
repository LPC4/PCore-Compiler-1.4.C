#include "../include/Compiler.h"

int main(char *argv) {
    // todo: add command line arguments

    Compiler compiler = Compiler("PCore Compiler", "1.4.0", "liamd", "../resources/test.pc");

    return EXIT_SUCCESS;
}
