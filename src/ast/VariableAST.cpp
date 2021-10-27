#include <string>
#include "VariableAST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"

VariableAST::VariableAST(const std::string& id)
    : id(id) {}

VariableAST::VariableAST(const VariableAST& other)
    : id(other.id) {}

llvm::Value* VariableAST::accept(CodegenVisitor& cg) 
{
    return cg.codegen(this);
}

std::string VariableAST::accept(ASTStringifier& sf) 
{
    return sf.toString(this);
}

VariableAST* VariableAST::cloneImpl()
{
    return new VariableAST(*this);    
}