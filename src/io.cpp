#include <fstream>
#include <sstream>
#include <string>
#include "ast/ModuleAST.h"
#include "io.h"

std::string readFile(std::ifstream& in)
{
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

std::string stringify(ModuleAST* ast) 
{
    ASTStringifier stringifier = ASTStringifier(true);
    return stringifier.toString(ast);
}
