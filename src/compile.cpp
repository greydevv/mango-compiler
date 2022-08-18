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
    // TODO: keep a reference to the open file in the ContextManager and when
    // an error arises check to see if that file is already opened
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
