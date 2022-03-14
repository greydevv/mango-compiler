#ifndef IO_H
#define IO_H

#include <fstream>
#include <string>
#include "ast/ModuleAST.h"

#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "fmt/color.h"

std::string readFile(std::ifstream& in);
std::string stringify(ModuleAST* ast);

std::string underlineError(const std::string& line, int x, int len);

#endif
