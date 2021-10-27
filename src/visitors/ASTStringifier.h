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
        std::string toString(ExpressionAST* ast);
        std::string toString(ModuleAST* ast);
        std::string toString(VariableAST* ast);
        std::string toString(NumberAST* ast);
};

#endif