#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <string>
#include "Operator.h"
#include "SymbolTable.h"
#include "Lexer.h"
#include "ContextManager.h"
#include "ast/AST.h"
#include "ast/FunctionAST.h"
#include "ast/ModuleAST.h"
#include "ast/VariableAST.h"
#include "ast/NumberAST.h"
#include "ast/ReturnAST.h"
#include "ast/CompoundAST.h"
#include "ast/PrototypeAST.h"
#include "ast/ForAST.h"

class Parser
{
    public:
        Parser(FilePath fp, ContextManager& ctx);
        std::unique_ptr<ModuleAST> parse();
        
    private:
        const FilePath fp;
        ContextManager& ctx;
        Lexer lexer;
        Token tok;

        std::unique_ptr<AST> parsePrimary();
        std::unique_ptr<AST> parseKwd();
        std::unique_ptr<AST> parseId();
        std::unique_ptr<NumberAST> parseNum();
        std::unique_ptr<AST> parseArray(Type eleType);
        std::unique_ptr<AST> parseArray();
        std::unique_ptr<ModuleAST> parseIncludeStmt();
        std::unique_ptr<PrototypeAST> parseExternStmt();
        std::unique_ptr<FunctionAST> parseFuncDef();
        std::unique_ptr<PrototypeAST> parseFuncProto();
        std::vector<std::unique_ptr<VariableAST>> parseFuncParams();
        std::vector<std::unique_ptr<ExpressionAST>> parseCallParams();
        std::unique_ptr<CompoundAST> parseCompound();
        std::unique_ptr<ReturnAST> parseReturnStmt();
        std::unique_ptr<IfAST> parseIfStmt();
        std::unique_ptr<ForAST> parseForStmt();
        std::unique_ptr<ArrayAST> parseIntArrayGen();
        std::unique_ptr<WhileAST> parseWhileStmt();
        std::unique_ptr<AST> parseIdTerm();
        std::unique_ptr<UnaryExprAST> parsePreUnaryExpr();
        std::unique_ptr<UnaryExprAST> parsePostUnaryExpr(std::unique_ptr<AST> operand);
        std::unique_ptr<AST> parseOperand();
        std::unique_ptr<AST> parseTerm();
        bool isTokUnary();
        std::unique_ptr<ExpressionAST> parseExpr();
        std::unique_ptr<ExpressionAST> parseSubExpr(std::unique_ptr<ExpressionAST> L, int prec = 0);

        bool eat(Token::token_type expected_type);
        bool eat();
        void getToken();
        std::string getTokenLine(Token tok);
};

std::unique_ptr<ModuleAST> getAstFromFile(const FilePath& fp, ContextManager& ctx);

#endif
