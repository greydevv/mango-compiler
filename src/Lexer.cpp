#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "io.h"
#include "Token.h"
#include "Lexer.h"

Lexer::Lexer(const FilePath& fp, ContextManager& ctx)
    : is(fp.abspath), c('\0'), loc()
{
    // prime lexer by getting first character and skipping any junk at
    // beginning of file
    is.get(c);
    skipJunk();
}

void Lexer::debugRead()
{
    Token tok = nextToken();
    while (tok.type != Token::TOK_EOF)
    {
      std::cout << tok << '\n';
      tok = nextToken();
    }
}

Token Lexer::peekToken()
{
    FileLocation prevLoc = loc;
    int prevPos = is.tellg();
    Token tok = nextToken();

    // reset cursor position
    // subtract 1 so next call to is.get() consumes the correct character
    is.seekg(prevPos-1, std::ios_base::beg);
    is.get(c);
    loc = prevLoc;
    return tok;
}

Token Lexer::nextToken()
{
  Token tok = getNextToken();
  skipJunk();
  return tok;
}

Token Lexer::getNextToken()
{
    if (is.eof())
        // TODO: For EOF scenario, need to get position of last token and set x
        // to be 1 space after that token
        return Token(Token::TOK_EOF, "EOF", SourceLocation(loc.x, loc.y-1, 1));
    else if (isalpha(c) || c == '_')
        return lexAlpha();
    else if (isnumber(c))
        return lexNum();
    else if (c == '"')
        return lexString();
    else
        return lexOther();
}

Token Lexer::lexAlpha()
{
    // TODO: refactor below while loop into separate function
    std::string s;
    // want loc of token to start at beginning of token's value
    FileLocation tmpLoc = loc;
    while (isalnum(c) || c == '_')
    {
        s += c;
        next();
    }

    Token::token_type type;
    if (isKwd(s))
        type = Token::TOK_KWD;
    else if (isType(s))
        type = Token::TOK_TYPE;
    else
        type = Token::TOK_ID;
    return Token(type, s, SourceLocation(tmpLoc, s.size()));
}

Token Lexer::lexNum()
{
    // TODO: refactor below while loop into separate function
    std::string s;
    FileLocation tmpLoc = loc;
    while (isalnum(c))
    {
        s += c;
        next();
    }
    return Token(Token::TOK_NUM, s, SourceLocation(tmpLoc, s.size()));
}

Token Lexer::lexString()
{
    std::string s;
    FileLocation tmpLoc = loc;
    next();  // skip opening quotes
    while (c != '"')
    {
        s += c;
        next();
    }
    next(); // skip closing quotes
    return Token(Token::TOK_STR, s, SourceLocation(tmpLoc, s.size()));
}

Token Lexer::lexOther()
{
    Token::token_type type = lexTokenType();
    if (type == Token::TOK_EOF)
        return Token(type, "EOF", SourceLocation(loc, 1));
    std::string value;
    if (type == Token::TOK_UND)
        value = c;
    else
        value = tokenValues.at(type);
    // have to subtract length to get correct starting position of token
    SourceLocation tokLoc(loc.x-(value.size()-1), loc.y, value.size());
    Token tok = Token(type, value, tokLoc);
    next();
    return tok;
}

Token::token_type Lexer::lexTokenType()
{
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
        {
            if (peek() == '+')
            {
                next();
                return Token::TOK_DPLUS;
            }
            return Token::TOK_PLUS;
        }
        case '-':
        {
            switch(peek())
            {
                case '>':
                {
                    next();
                    return Token::TOK_RARROW;
                }
                case '-':
                {
                    next();
                    return Token::TOK_DMINUS;
                }
                default:
                    return Token::TOK_MINUS;
            }
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
            || s == "true"
            || s == "false"
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
    std::regex intRegex("(?:u|i|f)(?:8|16|32|64|128)");
    return (std::regex_match(s, intRegex)
            || s == "bool"
            || s == "void"
            || s == "array");
}

void Lexer::next()
{
    if (eol())
    {
        loc.y++;
        loc.x = 1;
    } else {
        loc.x++;
    }
    is.get(c);
}

char Lexer::peek()
{
    return is.peek();
}

void Lexer::skipJunk()
{
    skipWhitespace();
    if (c == '/' && peek() == '/')
    {
      skipComment();
      skipJunk();
    }
}

void Lexer::skipWhitespace()
{
    while (!eof() && isspace(c))
        next();
}

void Lexer::skipComment()
{
    while (!eol())
        next();
}

bool Lexer::eol()
{
    return eof() || c == '\n';
}

bool Lexer::eof()
{
    return is.eof();
}

std::string Lexer::getLine(int lineNo)
{
    int prevPos = is.tellg();
    is.seekg(0);

    std::string line;
    for (int i = 1; i <= lineNo; i++)
        std::getline(is, line);

    is.seekg(prevPos, std::ios_base::beg);
    return line;
}
