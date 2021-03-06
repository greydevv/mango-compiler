#ifndef COMPOUND_AST_H
#define COMPOUND_AST_H

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
        virtual Type accept(ASTValidator& vd) override;
        virtual llvm::Value* accept(ASTCodegenner& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual CompoundAST* cloneImpl() override;
};

#endif
