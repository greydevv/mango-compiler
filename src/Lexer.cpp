#include <iostream>
#include <fstream>
#include <string>
#include "io.h"
#include "Token.h"
#include "Lexer.h"

Lexer::Lexer(const FilePath& fp, const std::string& src)
    : is(fp.abspath), src(src), pos(0), c('\0'), loc({1,1})
{
    is.get(c);
    // std::cout << "========== START DEBUG READ ==========\n";
    // debugRead(true);
    // std::cout << "=========== END DEBUG READ ===========\n";
}

void Lexer::debugRead(bool pretty)
{
    int debugLineNo = 1;
    std::string line;
    while (std::getline(is, line))
    {
        std::cout << fmt::format("{}: {}\n", debugLineNo, line);
        debugLineNo++;
    }
}

Token Lexer::peekToken()
{
    SourceLocation prevLoc = loc;
    // get next token
    Token tok = nextToken();

    // seek back to original position
    // need to make value negative because it moves relative to the current
    // position. also need to subtract one so that the 'next()' call consumes the correct character
    is.seekg(-tok.value.size()-1, std::ios_base::cur);
    next();
    loc = prevLoc;
    return tok;
}

Token Lexer::nextToken()
{
    if (is.eof())
    {
        return Token(Token::TOK_EOF, loc);
    }
    else if (isspace(c))
    {
        // remove whitespace
        skipWhitespace();
        return nextToken();
    }
    else if (c == '/' && peek() == '/')
    {
        // remove comments
        skipComment();
        return nextToken();
    }
    else if (isalpha(c) || c == '_')
    {
        return lexAlpha();
    }
    else if (isnumber(c))
    {
        return lexNum();
    }
    else if (c == '"')
    {
        return lexString();
    }
    else
    {
        return lexOther();
    }
}

Token Lexer::lexAlpha()
{
    // TODO: refactor below while loop into separate function
    std::string s;
    // want loc of token to start at beginning of token's value
    SourceLocation tmpLoc(loc.x, loc.y);
    while (isalnum(c) || c == '_')
    {
        s += c;
        next();
    }

    Token::token_type type = Token::TOK_ID;
    if (isKwd(s))
        type = Token::TOK_KWD;
    else if (isType(s))
        type = Token::TOK_TYPE;
    return Token(type, s, tmpLoc);
}

Token Lexer::lexNum()
{
    // TODO: refactor below while loop into separate function
    std::string s;
    SourceLocation tmpLoc(loc.x, loc.y);
    while (isalnum(c))
    {
        s += c;
        next();
    }
    return Token(Token::TOK_NUM, s, tmpLoc);
}

Token Lexer::lexString()
{
    std::string s;
    SourceLocation tmpLoc(loc.x, loc.y);
    // external 'next' calls are for skipping the quotes. If encountering
    // "hello world", we only want to store hello world without the quotes
    next();
    while (c != '"')
    {
        s += c;
        next();
    }
    next();
    return Token(Token::TOK_STR, s, tmpLoc);
}

Token Lexer::lexOther()
{
    Token::token_type type = lexTokenType();
    if (type == Token::TOK_EOF)
        return Token(type, "EOF", SourceLocation(loc.x, loc.y));
    std::string value;
    if (type == Token::TOK_UND)
        value = c;
    else
        value = tokenValues.at(type);
    // have to subtract length to get correct start position
    SourceLocation tokLoc(loc.x-(value.size()-1), loc.y);
    Token tok = Token(type, value, tokLoc);
    assert(tok.value.size() > 0);
    next();
    return tok;
}

Token::token_type Lexer::lexTokenType()
{
    skipWhitespace();
    switch(c)
    {
        case '(':
            return Token::TOK_OPAREN;
        case ')':
            return Token::TOK_CPAREN;
        case '[':
            return Token::TOK_OSQUARE;
        case ']':
            return Token::TOK_CSQUARE;
        case '{':
            return Token::TOK_OBRACK;
        case '}':
            return Token::TOK_CBRACK;
        case ';':
            return Token::TOK_SCOLON;
        case ':':
            return Token::TOK_COLON;
        case ',':
            return Token::TOK_COMMA;
        case '.':
            return Token::TOK_PERIOD;
        case '+':
            return Token::TOK_PLUS;
        case '-':
        {
            if (peek() == '>')
            {
                next();
                return Token::TOK_RARROW;
            }
            return Token::TOK_MINUS;
        }
        case '*':
            return Token::TOK_STAR;
        case '/':
        {
            return Token::TOK_FSLASH;
        }
        case '!':
        {
            if (peek() == '=')
            {
                next();
                return Token::TOK_NEQUALS;
            }
        }
        case '=':
        {
            if (peek() == '=')
            {
                next();
                return Token::TOK_DEQUALS;
            }
            return Token::TOK_EQUALS;
        }
        case '<':
        {
            if (peek() == '=')
            {
                next();
                return Token::TOK_LTE;
            }
            return Token::TOK_LT;
        }
        case '>':
        {
            if (peek() == '=')
            {
                next();
                return Token::TOK_GTE;
            }
            return Token::TOK_GT;
        }
        case '&':
        {
            if (peek() == '&')
            {
                next();
                return Token::TOK_DAMPSND;
            }
        }
        case '|':
        {
            if (peek() == '|')
            {
                next();
                return Token::TOK_DBAR;
            }
        }
        default:
        {
            std::cout << "HERE\n";
            if (is.eof())
                return Token::TOK_EOF;
            return Token::TOK_UND;
        }
    }
}

bool Lexer::isKwd(const std::string& s)
{
    // reserved keywords
    return (s == "return"
            || s == "func"
            || s == "if"
            || s == "else"
            || s == "for"
            || s == "while"
            || s == "in"
            || s == "extern"
            || s == "include");
}

bool Lexer::isType(const std::string& s)
{
    return (s == "int"
            || s == "void"
            || s == "array");
}

void Lexer::next()
{
    pos++;
    loc.x++;
    // c = src[pos];
    is.get(c);
}

char Lexer::peek()
{
    return is.peek();
}

void Lexer::skipWhitespace()
{
    while (!is.eof() && isspace(c))
    {
        if (c == '\n')
        {
            loc.y++;
            loc.x = 0;
        }
        next();
    }
}

void Lexer::skipComment()
{
    while (!is.eof() && c != '\n')
        next();
    loc.y++;
    loc.x = 0;
    if (isspace(c))
        skipWhitespace();
}

std::string Lexer::getLine(int lineNo)
{
    int currPos = is.tellg();
    is.seekg(0);

    std::string line;
    for (int i = 1; i <= lineNo; i++)
        std::getline(is, line);

    is.seekg(currPos, std::ios_base::beg);
    return line;
}
