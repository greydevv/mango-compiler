#include <memory>
#include <vector>
#include "CompoundAST.h"
#include "../visitors/CodegenVisitor.h"

CompoundAST::CompoundAST(const CompoundAST& other)
{
    for (auto& child : other.children)
    {
        children.push_back(std::unique_ptr<AST>(child->clone()));
    }
}

void CompoundAST::addChild(std::unique_ptr<AST> child)
{
    children.push_back(child);
}

llvm::Value* CompoundAST::accept(CodegenVisitor& cg)
{
    return cg.codegen(this);
}


