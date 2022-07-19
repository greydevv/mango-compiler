#ifndef FOR_AST_H
#define FOR_AST_H

#include <string>
#include <memory>
#include "AST.h"
#include "ExpressionAST.h"
#include "ArrayAST.h"
#include "CompoundAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

class ForAST : public AST
{
    public:
        std::unique_ptr<ExpressionAST> expr;
        std::unique_ptr<ArrayAST> iter;
        std::unique_ptr<CompoundAST> body;

        ForAST(std::unique_ptr<ExpressionAST> expr, std::unique_ptr<ArrayAST> iter, std::unique_ptr<CompoundAST> body);
        ForAST(const ForAST& other);
        Type accept(ASTValidator& vd) override;
        llvm::Value* accept(ASTCodegenner& cg) override;
        std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        ForAST* cloneImpl() override;
};

#endif
