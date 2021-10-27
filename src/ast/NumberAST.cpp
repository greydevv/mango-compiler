#include "NumberAST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"

NumberAST::NumberAST(double val)
    : val(val) {}

NumberAST::NumberAST(const NumberAST& other)
    : val(other.val) {}

llvm::Value* NumberAST::accept(CodegenVisitor& cg) 
{
    return cg.codegen(this);
}

std::string NumberAST::accept(ASTStringifier& sf) 
{
    return sf.toString(this);
}

NumberAST* NumberAST::cloneImpl()
{
    return new NumberAST(*this);
}