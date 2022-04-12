#include <string>
#include "VariableAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

VariableAST::VariableAST(const std::string& id, Type type, VarCtx ctx)
    : id(id), type(type), ctx(ctx) {}

VariableAST::VariableAST(const std::string& id)
    : id(id), type(Type::eUnd), ctx(VarCtx::eReference) {}

VariableAST::VariableAST(const VariableAST& other)
    : id(other.id), type(other.type), ctx(other.ctx) {}

Type VariableAST::accept(ASTValidator& vd)
{
    return vd.validate(this);
}

llvm::Value* VariableAST::accept(ASTCodegenner& cg) 
{
    return cg.codegen(this);
}

std::string VariableAST::accept(ASTStringifier& sf, int tabs) 
{
    return sf.toString(this, tabs);
}

VariableAST* VariableAST::cloneImpl()
{
    return new VariableAST(*this);    
}
