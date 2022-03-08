#include "PrototypeAST.h"
#include "VariableAST.h"
#include "../Types.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

PrototypeAST::PrototypeAST(const std::string& name, Type retType, std::vector<std::unique_ptr<VariableAST>> params)
    : name(name), retType(retType), params(std::move(params)) {}

PrototypeAST::PrototypeAST(const PrototypeAST& other)
{
    params = std::vector<std::unique_ptr<VariableAST>>();
    // TODO: test this method - not sure if below is totally safe
    for (auto& param : other.params)
    {
        VariableAST& cloneParam = dynamic_cast<VariableAST&>(*param->clone());
        params.push_back(std::make_unique<VariableAST>(cloneParam));
    }
}

void PrototypeAST::addParam(std::unique_ptr<VariableAST> param)
{
    params.push_back(std::move(param));
}

llvm::Value* PrototypeAST::accept(CodegenVisitor& cg)
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
