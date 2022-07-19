#include "FunctionAST.h"
#include "CompoundAST.h"
#include "PrototypeAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<CompoundAST> body)
    : proto(std::move(proto)), body(std::move(body))
{
    // set return statement to void if none was given
    if (!this->body->hasRetStmt())
        this->body->setRetStmt(ReturnAST::retVoid());
}

FunctionAST::FunctionAST(const FunctionAST& other)
    : proto(std::unique_ptr<PrototypeAST>(dynamic_cast<PrototypeAST*>(other.proto->clone()))),
      body(std::unique_ptr<CompoundAST>(dynamic_cast<CompoundAST*>(other.body->clone()))) {}

Type FunctionAST::accept(ASTValidator& vd)
{
    return vd.validate(this);
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
