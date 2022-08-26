#include "NumberAST.h"
#include "../Token.h"
#include "../Types.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

NumberAST::NumberAST(double val, Type type, SourceLocation loc)
    : AST(loc), val(val), width(widthFromType(type)), type(type) {}

NumberAST::NumberAST(double val, SourceLocation loc)
    : AST(loc), val(val), width(32), type(Type::eInt32) {}

NumberAST::NumberAST(const NumberAST& other)
    : AST(other.loc), val(other.val), type(other.type) {}

Type NumberAST::accept(ASTValidator& vd)
{
    return vd.validate(this);
}

llvm::Value* NumberAST::accept(ASTCodegenner& cg)
{
    return cg.codegen(this);
}

std::string NumberAST::accept(ASTStringifier& sf, int tabs)
{
    return sf.toString(this, tabs);
}

NumberAST* NumberAST::cloneImpl()
{
    return new NumberAST(*this);
}
