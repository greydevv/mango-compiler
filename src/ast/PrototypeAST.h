#ifndef PROTOTYPE_AST_H
#define PROTOTYPE_AST_H

#include <string>
#include <memory>
#include <vector>
#include "AST.h"
#include "../Types.h"
#include "VariableAST.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

class PrototypeAST : public AST
{
    public:
        std::string name;
        Type retType;
        std::vector<std::unique_ptr<VariableAST>> params;
        bool isExtern;

        PrototypeAST(const std::string& name, Type ret_type, std::vector<std::unique_ptr<VariableAST>> params, SourceLocation loc);
        PrototypeAST(const PrototypeAST& other);
        void addParam(std::unique_ptr<VariableAST> param);
        Type accept(ASTValidator& vd) override;
        llvm::Value* accept(ASTCodegenner& cg) override;
        std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        PrototypeAST* cloneImpl() override;
};

#endif
