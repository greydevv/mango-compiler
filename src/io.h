#ifndef IO_H
#define IO_H

#include <fstream>
#include <string>
#include <vector>
#include "path.h"
#include "Token.h"
#include "ast/ModuleAST.h"

#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "fmt/color.h"

std::string readFile(std::ifstream& in);
std::string stringify(ModuleAST* ast);
std::string underlineError(const std::string& line, int x, int len);
std::string underlineError(const std::string& fname, SourceLocation loc);

struct CompileArgs
{
    std::vector<FilePath> paths;
    bool debug;
    bool emit;
};

CompileArgs parseArgs(std::vector<std::string> argv);

#endif
