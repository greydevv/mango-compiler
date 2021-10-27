#ifndef IO_H
#define IO_H

#include "ast/ModuleAST.h"

std::string readFile(std::ifstream& in);
std::string stringify(ModuleAST* ast);

#endif