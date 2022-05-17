#ifndef AST_STRINGIFIER
#define AST_STRINGIFIER

#include <sstream>
#include <memory>
#include <string>

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

class ASTStringifier
{
    public:
        ASTStringifier(bool simpleExpr = false);
        std::string toString(ModuleAST* ast, int tabs = 0);
        std::string toString(ExpressionAST* ast, int tabs = 0);
        std::string toString(UnaryExprAST* ast, int tabs);
        std::string toString(VariableAST* ast, int tabs = 0);
        std::string toString(NumberAST* ast, int tabs = 0);
        std::string toString(ArrayAST* ast, int tabs = 0);
        std::string toString(CompoundAST* ast, int tabs = 0);
        std::string toString(FunctionAST* ast, int tabs = 0);
        std::string toString(PrototypeAST* ast, int tabs = 0);
        std::string toString(ReturnAST* ast, int tabs = 0);
        std::string toString(CallAST* ast, int tabs = 0);
        std::string toString(IfAST* ast, int tabs = 0);
        std::string toString(ForAST* ast, int tabs = 0);
        std::string toString(WhileAST* ast, int tabs = 0);
    
    private:
        bool simpleExpr;
        std::string indent(std::string str, int tabs = 1);
};

#endif
