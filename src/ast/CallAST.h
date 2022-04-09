#ifndef CALL_AST_H
#define CALL_AST_H

#include <memory>
#include <vector>
#include "AST.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

class CallAST : public AST
{
    public:
        const std::string id;
        std::vector<std::unique_ptr<AST>> params;

        CallAST(const std::string& id, std::vector<std::unique_ptr<AST>> param);
        CallAST(const CallAST& other);
        void addParam(std::unique_ptr<AST> param);
        virtual llvm::Value* accept(ASTCodegenner& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual CallAST* cloneImpl() override;
};

#endif
