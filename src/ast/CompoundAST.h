#ifndef COMPOUND_AST_H
#define COMPOUND_AST_H

#include <string>
#include <memory>
#include <vector>
#include "AST.h"
#include "ReturnAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

class CompoundAST : public AST
{
    public:
        std::vector<std::unique_ptr<AST>> children;
        std::unique_ptr<ReturnAST> retStmt;

        CompoundAST(std::vector<std::unique_ptr<AST>> children, std::unique_ptr<ReturnAST> retStmt);
        CompoundAST(std::vector<std::unique_ptr<AST>> children);
        CompoundAST();
        CompoundAST(const CompoundAST& other);
        void addChild(std::unique_ptr<AST> child);
        bool hasRetStmt();
        void setRetStmt(std::unique_ptr<ReturnAST> retStmt);
        Type accept(ASTValidator& vd) override;
        llvm::Value* accept(ASTCodegenner& cg) override;
        std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        CompoundAST* cloneImpl() override;
};

#endif
