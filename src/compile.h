#ifndef COMPILE_H
#define COMPILE_H

#include <string>
#include "ContextManager.h"
#include "io.h"

std::shared_ptr<ModuleAST> compile(const std::string& fname, ContextManager& ctx, std::ostringstream& outs);
std::unique_ptr<ModuleAST> getAstFromFile(const std::string& fname);

#endif
