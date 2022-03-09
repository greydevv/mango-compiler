#ifndef AST_STRINGIFIER
#define AST_STRINGIFIER

#include <sstream>
#include <memory>
#include <string>

class ModuleAST;
class ExpressionAST;
class VariableAST;
class NumberAST;
class CompoundAST;
class FunctionAST;
class PrototypeAST;
class ReturnAST;
class CallAST;
class IfAST;

class ASTStringifier
{
    public:
        ASTStringifier(bool simpleExpr = false);
        std::string toString(ModuleAST* ast, int tabs = 0);
        std::string toString(ExpressionAST* ast, int tabs = 0);
        std::string toString(VariableAST* ast, int tabs = 0);
        std::string toString(NumberAST* ast, int tabs = 0);
        std::string toString(CompoundAST* ast, int tabs = 0);
        std::string toString(FunctionAST* ast, int tabs = 0);
        std::string toString(PrototypeAST* ast, int tabs = 0);
        std::string toString(ReturnAST* ast, int tabs = 0);
        std::string toString(CallAST* ast, int tabs = 0);
        std::string toString(IfAST* ast, int tabs = 0);
    
    private:
        bool simpleExpr;
        std::string indent(std::string str, int tabs = 1);
};

#endif
