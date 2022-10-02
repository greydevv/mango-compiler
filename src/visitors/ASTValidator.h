#ifndef AST_VALIDATOR_H
#define AST_VALIDATOR_H

#include <string>
#include <memory>
#include <vector>
#include <cmath>
#include "../Types.h"
#include "../ContextManager.h"
#include "../SymbolTable.h"

#define UINT128_MAX (pow(2, 128) - 1)
#define INT128_MAX (pow(2, 127) - 1)
#define INT128_MIN (-1 * pow(2, 127))

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
        SymbolTable<ProtoSymbol> fst;  // function symbol table

        Type expectedTy;

        void validateBoolExpr(std::shared_ptr<ExpressionAST> expr);
};

#endif
