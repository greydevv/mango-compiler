#ifndef AST_VALIDATOR_H
#define AST_VALIDATOR_H

#include <string>
#include "../Types.h"
#include "../SymbolTable.h"

class ModuleAST;
class ExpressionAST;
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
        ASTValidator(const std::string& fname, std::shared_ptr<ModuleAST> ast);
        void validate();
        Type validate(ModuleAST* ast);
        Type validate(ExpressionAST* ast);
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
        const std::string fname;
        std::shared_ptr<ModuleAST> ast;
        SymbolTable st;
};

#endif
