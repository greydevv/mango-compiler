#include <fstream>
#include <sstream>
#include <string>
#include "io.h"
#include "ast/ModuleAST.h"

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
    std::string tab = "  ";
    // indent both line and underline by two spaces
    s << tab << line << '\n';
    // fill beginning of string with spaces until (xPos-1) as xPos is the
    // position token begins
    s << tab << std::string(xPos-1, ' ') << '^' << std::string(len-1, '~');
    return s.str();
}

std::string underlineError(const std::string& fname, SourceLocation loc)
{
    // only use this method if the file is known to exist
    std::ifstream fs(fname);
    std::string src = readFile(fs);
    return "THIS IS A LINE";
}

CompileArgs parseArgs(std::vector<std::string> argv)
{
    CompileArgs args;
    for (auto arge : argv)
    {
        if (arge == "-debug")
            args.debug = true;
        else if (arge == "-emit")
            args.emit = true;
        else if (arge[0] != '-')
            args.paths.push_back(FilePath(arge));
    }
    return args;
}
