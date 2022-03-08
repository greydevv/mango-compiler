#ifndef PROTOTYPE_AST_H
#define PROTOTYPE_AST_H

#include <vector>
#include <memory>
#include "AST.h"
#include "../Types.h"
#include "VariableAST.h"
#include "../visitors/CodegenVisitor.h"
#include "../visitors/ASTStringifier.h"
#include "llvm/IR/Value.h"

class PrototypeAST : public AST
{
    public:
        std::string name;
        Type retType;
        std::vector<std::unique_ptr<VariableAST>> params;

        PrototypeAST(const std::string& name, Type ret_type, std::vector<std::unique_ptr<VariableAST>> params);
        PrototypeAST(const PrototypeAST& other);
        void addParam(std::unique_ptr<VariableAST> param);
        virtual llvm::Value* accept(CodegenVisitor& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual PrototypeAST* cloneImpl() override;
};

#endif
