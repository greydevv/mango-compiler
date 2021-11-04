#ifndef IO_H
#define IO_H

#include <fstream>
#include <string>
#include "ast/ModuleAST.h"

std::string readFile(std::ifstream& in);
std::string stringify(ModuleAST* ast);

#endif
