#include "CallAST.h"

CallAST::CallAST(const std::string& id, std::vector<std::unique_ptr<AST>> params)
    : id(id), params(std::move(params)) {}

CallAST::CallAST(const CallAST& other)
{
    // TODO: test this method - not sure if below is totally safe
    for (auto& param : other.params)
    {
        AST* cloneParam = param->clone();
        addParam(std::unique_ptr<AST>(cloneParam));
    }
}

void CallAST::addParam(std::unique_ptr<AST> param)
{
    params.push_back(std::move(param));
}

llvm::Value* CallAST::accept(CodegenVisitor& cg)
{
    return cg.codegen(this);
}

std::string CallAST::accept(ASTStringifier& sf, int tabs)
{
    return sf.toString(this, tabs);
}

CallAST* CallAST::cloneImpl()
{
    return new CallAST(*this);
}
