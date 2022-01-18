#include <memory>
#include <vector>
#include "AST.h"
#include "CompoundAST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"

CompoundAST::CompoundAST(const CompoundAST& other)
{
    for (auto& child : other.children)
    {
        children.push_back(std::unique_ptr<AST>(child->clone()));
    }
}

CompoundAST::CompoundAST(std::vector<std::unique_ptr<AST>> children)
    : children(std::move(children)) {}

void CompoundAST::addChild(std::unique_ptr<AST> child)
{
    children.push_back(std::move(child));
}

llvm::Value* CompoundAST::accept(CodegenVisitor& cg)
{
    return cg.codegen(this);
}

std::string CompoundAST::accept(ASTStringifier& sf, int tabs)
{
    return sf.toString(this, tabs);
}

CompoundAST* CompoundAST::cloneImpl()
{
    return new CompoundAST(*this);
}


