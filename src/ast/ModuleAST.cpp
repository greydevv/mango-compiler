#include <iostream>
#include <memory>
#include "ModuleAST.h"
#include "../io.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

ModuleAST::ModuleAST(const std::string& modName)
    : modName(modName) {}

ModuleAST::ModuleAST(const ModuleAST& other)
    : modName(other.modName)
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

void ModuleAST::print()
{
    std::cout << stringify(this);
}

Type ModuleAST::accept(ASTValidator& vd)
{
    return vd.validate(this);
}

llvm::Value* ModuleAST::accept(ASTCodegenner& cg) 
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
