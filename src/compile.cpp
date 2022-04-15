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
#include "visitors/ASTValidator.h"
#include "visitors/ASTCodegenner.h"

std::shared_ptr<ModuleAST> compile(const std::string& fname, std::ostringstream& outs)
{
    std::shared_ptr<ModuleAST> ast = getAstFromFile(fname);
    if (!ast)
        throw CompilationError(fmt::format("file not found: {}", fname));
    if (ast->children.size() > 0)
    {
        ASTValidator vd(fname, ast);
        vd.validate();
        ASTCodegenner cg(fname, ast);
        cg.codegen();
        cg.emitObjectCode();
        outs << cg.print();
    }
    return ast;
}
