#include <string>
#include <iostream>
#include <fstream>
#include "ast/ModuleAST.h"
#include "compile.h"
#include "io.h"
#include "Lexer.h"
#include "Parser.h"
#include "ast/ModuleAST.h"
#include "Exception.h"

std::shared_ptr<ModuleAST> compile(const std::string& fname, std::ostringstream& outs)
{
    std::ifstream fs(fname);
    if (!fs.is_open())
    {
        fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::orange_red), "Error");
        fmt::print(fmt::emphasis::bold, ": file not found: '{}'\n", fname);
        return nullptr;
    }
    std::string src = readFile(fs);
    Parser parser(fname, src);
    std::unique_ptr<ModuleAST> ast = parser.parse();
    std::shared_ptr<ModuleAST> sharedAst = std::move(ast);
    CodegenVisitor cg(fname, sharedAst);
    cg.codegen();
    cg.emitObjectCode();
    outs << cg.print();
    return sharedAst;
}
