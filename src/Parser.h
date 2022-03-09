#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <string>
#include "Operator.h"
#include "Lexer.h"
#include "ast/AST.h"
#include "ast/FunctionAST.h"
#include "ast/ModuleAST.h"
#include "ast/NumberAST.h"
#include "ast/PrototypeAST.h"

class Parser
{
    public:
        Parser(const std::string& fname, const std::string& src);
        std::unique_ptr<ModuleAST> parse();
        int getErrState();
        
    private:
        std::string fname;
        Lexer lexer;
        Token tok;
        int errState;

        std::unique_ptr<AST> parsePrimary();
        std::unique_ptr<AST> parseKwd();
        std::unique_ptr<AST> parseId();
        std::unique_ptr<NumberAST> parseNum();
        std::unique_ptr<FunctionAST> parseFuncDef();
        std::unique_ptr<PrototypeAST> parseFuncProto();
        std::vector<std::unique_ptr<VariableAST>> parseFuncParams();
        std::vector<std::unique_ptr<AST>> parseCallParams();
        std::unique_ptr<CompoundAST> parseCompound();
        std::unique_ptr<ReturnAST> parseReturnStmt();
        std::unique_ptr<IfAST> parseIfStmt();
        std::unique_ptr<AST> parseIdTerm();
        std::unique_ptr<AST> parseTerm();
        std::unique_ptr<AST> parseExpr();
        std::unique_ptr<AST> parseSubExpr(std::unique_ptr<AST> L, int prec = 0);

        bool eat(Token::token_type expected_type);
        bool eat();
        void getToken();
        void setErrState(int errState);
};

#endif
