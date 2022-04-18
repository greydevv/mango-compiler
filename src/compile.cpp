#include <string>
#include <iostream>
#include <fstream>
#include "ContextManager.h"
#include "ast/ModuleAST.h"
#include "compile.h"
#include "io.h"
#include "Lexer.h"
#include "Parser.h"
#include "ast/ModuleAST.h"
#include "Exception.h"
#include "visitors/ASTValidator.h"
#include "visitors/ASTCodegenner.h"

std::shared_ptr<ModuleAST> compile(const FilePath& fp, ContextManager& ctx, std::ostringstream& outs)
{
    // TODO: KEEP A REFERENCE TO THE OPEN FILE IN THE ContextManager AND WHEN
    // AN ERROR ARISES CHECK TO SEE IF THAT FILE IS ALREADY OPENED
    std::shared_ptr<ModuleAST> ast = getAstFromFile(fp, ctx);
    if (!ast)
        throw CompilationError(fmt::format("file not found: {}", fp.relpath));
    if (ast->children.size() > 0)
    {
        ASTValidator vd(ast, ctx);
        vd.validate();
        ASTCodegenner cg(ast, ctx);
        cg.codegen();
        cg.emitObjectCode();
        outs << cg.print();
    }
    return ast;
}
