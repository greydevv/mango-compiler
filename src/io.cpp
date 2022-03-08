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
    ASTStringifier stringifier = ASTStringifier(false);
    return stringifier.toString(ast);
}

std::string underlineError(const std::string& line, int xPos, int len)
{
    std::ostringstream s;
    // indent both line and underline by two spaces
    s << "  " << line << '\n';
    // fill beginning of string with spaces until (xPos-1) as xPos is the
    // position token begins
    s << "  " << std::string(xPos-1, ' ') << std::string(len, '^');
    return s.str();
}
