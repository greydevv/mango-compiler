#ifndef IO_H
#define IO_H

#include <fstream>
#include <string>
#include <vector>
#include "ast/ModuleAST.h"

#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "fmt/color.h"

std::string readFile(std::ifstream& in);
std::string stringify(ModuleAST* ast);
std::string underlineError(const std::string& line, int x, int len);

struct CompileArgs
{
    std::vector<std::string> fnames;
    bool debug;
    bool emit;
};

CompileArgs parseArgs(std::vector<std::string> argv);

#endif
