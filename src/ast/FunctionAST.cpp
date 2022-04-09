#include "FunctionAST.h"
#include "CompoundAST.h"
#include "PrototypeAST.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<CompoundAST> body)
    : proto(std::move(proto)), body(std::move(body)) {}

FunctionAST::FunctionAST(const FunctionAST& other)
    // : proto(std::unique_ptr<PrototypeAST>(other.proto->clone())),
    //   body(std::unique_ptr<CompoundAST>(other.body->clone()))
{
}

llvm::Value* FunctionAST::accept(ASTCodegenner& cg) 
{
    return cg.codegen(this);
}

std::string FunctionAST::accept(ASTStringifier& sf, int tabs) 
{
    return sf.toString(this, tabs);
}

FunctionAST* FunctionAST::cloneImpl()
{
    return new FunctionAST(*this);
}
