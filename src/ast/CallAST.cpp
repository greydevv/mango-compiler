#include <string>
#include <memory>
#include <utility>
#include "CallAST.h"
#include "AST.h"
#include "../Token.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

CallAST::CallAST(const std::string& id, std::vector<std::unique_ptr<ExpressionAST>> params, SourceLocation loc, SourceLocation paramsStartLoc)
    : AST(loc), id(id), params(std::move(params)), paramsStartLoc(paramsStartLoc) {}

CallAST::CallAST(const CallAST& other)
    : AST(other.loc), id(other.id)
{
    // TODO: test this method - not sure if below is totally safe
    for (auto& param : other.params)
        addParam(std::unique_ptr<ExpressionAST>(dynamic_cast<ExpressionAST*>(param->clone())));
}

void CallAST::addParam(std::unique_ptr<ExpressionAST> param)
{
    params.push_back(std::move(param));
}

Type CallAST::accept(ASTValidator& vd)
{
    return vd.validate(this);
}

llvm::Value* CallAST::accept(ASTCodegenner& cg)
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
