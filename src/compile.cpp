#include <string>
#include <iostream>
#include <fstream>
#include "checks.h"
#include "io.h"
#include "compile.h"
#include "Lexer.h"
#include "Parser.h"
#include "ast/ModuleAST.h"
#include "Exception.h"

int compile(const std::string& fname, bool debug, bool emit)
{
    if (fname.size() == 0)
    {
        std::cout << CommandError("no file name supplied").what() << '\n';
        return 1;
    }

    if (debug)
        // validate enums
        runChecks();

    std::ifstream fs(fname);
    if (!fs.is_open())
    {
        std::cout << "File not found: " << fname << '\n';
        return 1;
    }
    std::string src = readFile(fs);
    Parser parser(fname, src);
    try {
        std::unique_ptr<ModuleAST> ast = parser.parse();
        if (debug)
            std::cout << stringify(ast.get());
        CodegenVisitor cg(fname, std::move(ast));
        cg.codegen();
        if (emit)
            cg.emitObjectCode();
        if (debug)
            cg.print();
    } catch (const BaseException& e) {
        std::cout << e.what() << '\n';
        return 1;
    }

    return 0;
}
