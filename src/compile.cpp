#include <string>
#include <iostream>
#include <fstream>
#include "io.h"
#include "compile.h"
#include "Lexer.h"
#include "Parser.h"
#include "ast/ModuleAST.h"

int compile(const std::string& fname)
{
    std::ifstream fs(fname);
    if (!fs.is_open())
    {
        std::cout << "File not Found: " << fname << '\n';
        return 1;
    }
    Parser parser(readFile(fs));
    std::unique_ptr<ModuleAST> ast = parser.parse();

    std::cout << stringify(ast.get());

    return parser.getErrState();
}
