#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <string>
#include "Operator.h"
#include "Lexer.h"
#include "ast/AST.h"
#include "ast/ModuleAST.h"
#include "ast/NumberAST.h"

class Parser
{
    public:
        Parser(const std::string& src);
        std::unique_ptr<ModuleAST> parse();
        int getErrState();
        
    private:
        Lexer lexer;
        Token tok;
        int errState;

        std::unique_ptr<AST> parsePrimary();
        std::unique_ptr<AST> parseKwd();
        std::unique_ptr<NumberAST> parseNum();
        std::unique_ptr<VariableAST> parseId();
        std::unique_ptr<AST> parseTerm();
        std::unique_ptr<AST> parseExpr();
        std::unique_ptr<AST> parseSubExpr(std::unique_ptr<AST> L, int prec = 0);

        bool eat(Token::token_type expected_type);
        bool eat();
        void getToken();
        void setErrState(int errState);
};

#endif
