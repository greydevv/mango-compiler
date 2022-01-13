#include "PrototypeAST.h"
#include "VariableAST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

// PrototypeAST::PrototypeAST(const std::string name, const std::string ret_type, std::vector<std::unique_ptr<VariableAST>> params)
//     : name(name), ret_type(ret_type)
// {
//     for (auto& )
// }

PrototypeAST::PrototypeAST(const PrototypeAST& other)
{
    params = std::vector<std::unique_ptr<VariableAST>>();
    // for (auto& param : other.params)
    // {
    //     params.push_back(std::unique_ptr<AST>(param->clone()));
    // }
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
