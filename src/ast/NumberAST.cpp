#include "NumberAST.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

NumberAST::NumberAST(double val)
    : val(val) {}

NumberAST::NumberAST(const NumberAST& other)
    : val(other.val) {}

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
