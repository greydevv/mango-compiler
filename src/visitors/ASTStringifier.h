#ifndef AST_STRINGIFIER
#define AST_STRINGIFIER

#include <sstream>
#include <memory>
#include <string>

class ExpressionAST;
class ModuleAST;
class VariableAST;
class NumberAST;

class ASTStringifier
{
    public:
        ASTStringifier() {};
        std::string toString(ExpressionAST* ast, int tabs = 0);
        std::string toString(ModuleAST* ast, int tabs = 0);
        std::string toString(VariableAST* ast, int tabs = 0);
        std::string toString(NumberAST* ast, int tabs = 0);
    
    private:
        std::string indent(std::string str, int tabs = 1);
};

#endif