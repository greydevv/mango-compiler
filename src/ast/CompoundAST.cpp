#include <memory>
#include <vector>
#include "AST.h"
#include "CompoundAST.h"
#include "ReturnAST.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

CompoundAST::CompoundAST(std::vector<std::unique_ptr<AST>> children, std::unique_ptr<ReturnAST> retStmt)
    : children(std::move(children)), retStmt(std::move(retStmt)) {}

CompoundAST::CompoundAST(std::vector<std::unique_ptr<AST>> children)
    : children(std::move(children)), retStmt(nullptr) {}

CompoundAST::CompoundAST()
    : children(std::vector<std::unique_ptr<AST>>()), retStmt(nullptr) {}

CompoundAST::CompoundAST(const CompoundAST& other)
{
    for (auto& child : other.children)
    {
        addChild(std::unique_ptr<AST>(child->clone()));
    }
}

void CompoundAST::addChild(std::unique_ptr<AST> child)
{
    children.push_back(std::move(child));
}

void CompoundAST::setRetStmt(std::unique_ptr<ReturnAST> newRetStmt)
{
    retStmt = std::move(newRetStmt);
}

llvm::Value* CompoundAST::accept(ASTCodegenner& cg)
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


