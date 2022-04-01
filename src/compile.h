#ifndef COMPILE_H
#define COMPILE_H

#include <string>
#include "io.h"

std::shared_ptr<ModuleAST> compile(const std::string& fname, std::ostringstream& outs);

#endif
