#ifndef LEXER_H
#define LEXER_H

#include "io.h"
#include "Token.h"

class Lexer
{
    public:
        Lexer(const FilePath& fp, const std::string& src);
        void debugRead(bool pretty = false);
        Token peekToken();
        Token nextToken();
        std::string getLine(int lineNo);

    private:
        std::ifstream is;
        const std::string src;
        int pos;
        char c;
        SourceLocation loc;

        Token lexAlpha();
        Token lexNum();
        Token lexString();
        Token lexOther();
        Token::token_type lexTokenType();
        bool isKwd(const std::string& s);
        bool isType(const std::string& s);
        char peek();
        void next();
        void skipWhitespace();
        void skipComment();
};

#endif
