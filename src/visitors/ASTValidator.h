#ifndef AST_VALIDATOR
#define AST_VALIDATOR

#include <string>
#include "../Types.h"

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
        ASTValidator(std::shared_ptr<ModuleAST> ast);
        bool validate(ModuleAST* ast);
        bool validate(ExpressionAST* ast);
        bool validate(VariableAST* ast);
        bool validate(NumberAST* ast);
        bool validate(ArrayAST* ast);
        bool validate(CompoundAST* ast);
        bool validate(FunctionAST* ast);
        bool validate(PrototypeAST* ast);
        bool validate(ReturnAST* ast);
        bool validate(CallAST* ast);
        bool validate(IfAST* ast);
        bool validate(ForAST* ast);
        bool validate(WhileAST* ast);

    private:
        std::shared_ptr<ModuleAST> ast;
};

#endif
