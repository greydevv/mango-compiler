#ifndef UNARY_EXPR_AST
#define UNARY_EXPR_AST

#include <string>
#include <memory>
#include "AST.h"
#include "../Operator.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

class UnaryExprAST : public AST
{
    public:
        enum unary_type
        {
            UNARY_PRE,
            UNARY_POST,
        };

        std::unique_ptr<AST> operand;
        Operator::op_type op;

        UnaryExprAST(std::unique_ptr<AST> operand, Operator::op_type op, unary_type type);
        UnaryExprAST(const UnaryExprAST& other);
        bool isPrefix();
        bool isPostfix();
        bool isInc();
        bool isDec();
        static std::unique_ptr<UnaryExprAST> unaryPrefix(std::unique_ptr<AST> operand, Operator::op_type op);
        static std::unique_ptr<UnaryExprAST> unaryPostfix(std::unique_ptr<AST> operand, Operator::op_type op);
        virtual Type accept(ASTValidator& vd) override;
        virtual llvm::Value* accept(ASTCodegenner& cg) override;
        virtual std::string accept(ASTStringifier& sf, int tabs = 0) override;
    protected:
        virtual UnaryExprAST* cloneImpl() override;
    private:
        unary_type type;
};

#endif
