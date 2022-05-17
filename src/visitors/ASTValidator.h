#ifndef AST_VALIDATOR_H
#define AST_VALIDATOR_H

#include <string>
#include "../Types.h"
#include "../ContextManager.h"
#include "../SymbolTable.h"

class ModuleAST;
class ExpressionAST;
class UnaryExprAST;
class VariableAST;
class NumberAST;
class ArrayAST;
class CompoundAST;
class FunctionAST;
class PrototypeAST;
class ReturnAST;
class CallAST;
class IfAST;
class ForAST;
class WhileAST;

class ASTValidator
{
    public:
        ASTValidator(std::shared_ptr<ModuleAST> ast, ContextManager& ctx);
        void validate();
        Type validate(ModuleAST* ast);
        Type validate(ExpressionAST* ast);
        Type validate(UnaryExprAST* ast);
        Type validate(VariableAST* ast);
        Type validate(NumberAST* ast);
        Type validate(ArrayAST* ast);
        Type validate(CompoundAST* ast);
        Type validate(FunctionAST* ast);
        Type validate(PrototypeAST* ast);
        Type validate(ReturnAST* ast);
        Type validate(CallAST* ast);
        Type validate(IfAST* ast);
        Type validate(ForAST* ast);
        Type validate(WhileAST* ast);

    private:
        std::shared_ptr<ModuleAST> ast;
        ContextManager& ctx;
        SymbolTable<Type> st;
        SymbolTable<std::vector<Type>> fst; // function symbol table

        void validateBoolExpr(Type exprType);
};

#endif
