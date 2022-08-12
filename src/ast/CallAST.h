#ifndef CALL_AST_H
#define CALL_AST_H

#include <string>
#include <memory>
#include <vector>
#include "AST.h"
#include "../visitors/ASTValidator.h"
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
        Type accept(ASTValidator& vd) override;
        llvm::Value* accept(ASTCodegenner& cg) override;
        std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        CallAST* cloneImpl() override;
};

#endif
