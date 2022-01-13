#ifndef FUNCTION_AST_H
#define FUNCTION_AST_H

#include <memory>
#include "AST.h"
#include "ExpressionAST.h"
#include "PrototypeAST.h"
#include "CompoundAST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

class FunctionAST : public AST
{
    public:
        std::unique_ptr<PrototypeAST> proto;
        std::unique_ptr<CompoundAST> body;

        FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<CompoundAST> body);
        FunctionAST(const FunctionAST& other);
        virtual llvm::Value* accept(CodegenVisitor& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual FunctionAST* cloneImpl() override;
};

#endif
