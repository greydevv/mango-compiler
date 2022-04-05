#ifndef COMPILE_H
#define COMPILE_H

#include <string>
#include "io.h"

std::shared_ptr<ModuleAST> compile(const std::string& fname, std::ostringstream& outs);
std::unique_ptr<ModuleAST> getAstFromFile(const std::string& fname);

#endif
