#include <memory>
#include "ModuleAST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"

ModuleAST::ModuleAST(const ModuleAST& other)
{
    for (auto& child : other.children)
    {
        children.push_back(std::unique_ptr<AST>(child->clone()));
    }
}

void ModuleAST::addChild(std::unique_ptr<AST> child) 
{
    children.push_back(std::move(child));
}

llvm::Value* ModuleAST::accept(CodegenVisitor& cg) 
{
    return cg.codegen(this);
}

std::string ModuleAST::accept(ASTStringifier& sf, int tabs) 
{
    return sf.toString(this, tabs);
}

ModuleAST* ModuleAST::cloneImpl()
{
    return new ModuleAST(*this);
}
