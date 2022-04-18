#ifndef COMPILE_H
#define COMPILE_H

#include <string>
#include "ContextManager.h"
#include "io.h"

std::shared_ptr<ModuleAST> compile(const FilePath& fp, ContextManager& ctx, std::ostringstream& outs);

#endif
