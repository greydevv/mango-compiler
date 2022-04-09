#include "PrototypeAST.h"
#include "VariableAST.h"
#include "../Types.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

PrototypeAST::PrototypeAST(const std::string& name, Type retType, std::vector<std::unique_ptr<VariableAST>> params)
    : name(name), retType(retType), params(std::move(params)) {}

PrototypeAST::PrototypeAST(const PrototypeAST& other)
{
    // TODO: test this method - not sure if below is totally safe
    for (auto& param : other.params)
    {
        VariableAST* cloneParam = dynamic_cast<VariableAST*>(param->clone());
        addParam(std::unique_ptr<VariableAST>(cloneParam));
    }
}

void PrototypeAST::addParam(std::unique_ptr<VariableAST> param)
{
    params.push_back(std::move(param));
}

bool PrototypeAST::accept(ASTValidator& vd)
{
    return vd.validate(this);
}

llvm::Value* PrototypeAST::accept(ASTCodegenner& cg)
{
    return cg.codegen(this);
}

std::string PrototypeAST::accept(ASTStringifier& sf, int tabs)
{
    return sf.toString(this, tabs);
}

PrototypeAST* PrototypeAST::cloneImpl()
{
    return new PrototypeAST(*this);
}
