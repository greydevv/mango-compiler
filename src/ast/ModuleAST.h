#ifndef MODULE_AST_H
#define MODULE_AST_H

#include <memory>
#include <vector>
#include "AST.h"
#include "../path.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

class ModuleAST : public AST
{
    public:
        std::vector<std::unique_ptr<AST>> children;
        const FilePath fp;

        ModuleAST(FilePath fp);
        ModuleAST(const ModuleAST& other);
        void addChild(std::unique_ptr<AST> child);
        void print();
        virtual Type accept(ASTValidator& vd) override;
        virtual llvm::Value* accept(ASTCodegenner& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual ModuleAST* cloneImpl() override;
};

#endif
