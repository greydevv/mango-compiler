#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include "Parser.h"
#include "ContextManager.h"
#include "Token.h"
#include "Operator.h"
#include "Types.h"
#include "Lexer.h"
#include "Exception.h"
#include "compile.h"
#include "io.h"
#include "path.h"
#include "ast/ModuleAST.h"
#include "ast/ExpressionAST.h"
#include "ast/VariableAST.h"
#include "ast/NumberAST.h"
#include "ast/ArrayAST.h"
#include "ast/FunctionAST.h"
#include "ast/CompoundAST.h"
#include "ast/PrototypeAST.h"
#include "ast/ReturnAST.h"
#include "ast/CallAST.h"
#include "ast/IfAST.h"
#include "ast/ForAST.h"
#include "ast/WhileAST.h"
#include "ast/UnaryExprAST.h"

Parser::Parser(FilePath fp, ContextManager& ctx)
    : fp(fp), 
      ctx(ctx),
      lexer(Lexer(fp, ctx)), 
      tok(lexer.nextToken()) 
{
    ctx.push(fp);
}

std::unique_ptr<ModuleAST> Parser::parse()
{
    auto modAST = std::make_unique<ModuleAST>(fp);
    while (tok.type != Token::TOK_EOF)
        modAST->addChild(parsePrimary());
    return modAST;
}

std::unique_ptr<AST> Parser::parsePrimary() 
{
    switch (tok.type)
    {
        case Token::TOK_KWD:
            return parseKwd();
        case Token::TOK_TYPE:
        case Token::TOK_ID:
            // either variable redefinition, reference
            return parseId();
        default:
        {
            std::unique_ptr<ExpressionAST> exprAST = parseExpr();
            eat(Token::TOK_SCOLON);
            return exprAST;
        }
    }
}

std::unique_ptr<AST> Parser::parseKwd()
{
    if (tok.value == "return")
        return parseReturnStmt();
    else if (tok.value == "func")
        return parseFuncDef();
    else if (tok.value == "if")
        return parseIfStmt();
    else if (tok.value == "for")
        return parseForStmt();
    else if (tok.value == "while")
        return parseWhileStmt();
    else if (tok.value == "extern")
        return parseExternStmt();
    else if (tok.value == "include")
        return parseIncludeStmt();
    else
    {
        std::cout << "[Dev Error] Keyword handling for '" << tok.value << "' not yet implemented in parser.\n";
        exit(1);
    }
}

std::unique_ptr<AST> Parser::parseId()
{
    std::unique_ptr<ExpressionAST> expr = parseExpr();
    eat(Token::TOK_SCOLON);
    return expr;
}

std::unique_ptr<NumberAST> Parser::parseNum() 
{
    auto numAST = std::make_unique<NumberAST>(std::stod(tok.value), tok.loc);
    eat(Token::TOK_NUM);
    return numAST;
}

std::unique_ptr<AST> Parser::parseArray(Type eleType)
{
    // either basic array '[a,b,c,d]' or string '"abcd"'

    auto arr = std::make_unique<ArrayAST>(eleType);
    eat(Token::TOK_OSQUARE);
    while (true)
    {
        arr->addElement(parseExpr());
        if (tok.type != Token::TOK_COMMA)
            // break and expect closing square bracket
            break;
        eat(Token::TOK_COMMA);
    }

    eat(Token::TOK_CSQUARE);

    return arr;
}

std::unique_ptr<AST> Parser::parseArray()
{
    return parseArray(Type::eUnd);
}

std::unique_ptr<ModuleAST> Parser::parseIncludeStmt()
{
    eat(Token::TOK_KWD);
    Token includeTok = tok;
    FilePath includeFp = FilePath::asImport(fp, includeTok.value);
    eat(Token::TOK_STR);
    eat(Token::TOK_SCOLON);
    std::unique_ptr<ModuleAST> incAST = getAstFromFile(includeFp, ctx);
    if (!incAST)
        // need to add quotes back for correct underlining
        throw FileNotFoundError(includeFp, getTokenLine(includeTok), includeTok.loc);
    ctx.pop();
    return incAST;
}

std::unique_ptr<PrototypeAST> Parser::parseExternStmt()
{
    eat(Token::TOK_KWD);
    eat(Token::TOK_KWD);
    std::unique_ptr<PrototypeAST> proto = parseFuncProto();
    proto->isExtern = true;
    eat(Token::TOK_SCOLON);
    return proto;
}

std::unique_ptr<FunctionAST> Parser::parseFuncDef()
{
    eat(Token::TOK_KWD);
    auto ast = std::make_unique<FunctionAST>(parseFuncProto(), parseCompound());
    return ast;
}

std::unique_ptr<PrototypeAST> Parser::parseFuncProto()
{
    std::string name = tok.value;
    SourceLocation nameLoc = tok.loc;
    eat(Token::TOK_ID);
    eat(Token::TOK_OPAREN);
    std::vector<std::unique_ptr<VariableAST>> params = parseFuncParams();
    eat(Token::TOK_CPAREN);
    Type retType = Type::eVoid;
    if (tok == Token::TOK_RARROW)
    {
        eat(Token::TOK_RARROW);
        retType = tok.toType();
        eat(Token::TOK_TYPE);
    }
    return std::make_unique<PrototypeAST>(name, retType, std::move(params), nameLoc);
}

std::vector<std::unique_ptr<VariableAST>> Parser::parseFuncParams()
{
    // TODO: refactor this and parseFuncParams into a method like parseCsv
    std::vector<std::unique_ptr<VariableAST>> params;
    if (tok.type != Token::TOK_CPAREN)
    {
        while (true)
        {
            Type paramType = typeFromString(tok.value);
            eat(Token::TOK_TYPE);
            // st.insert(tok.value, paramType);
            auto param = std::make_unique<VariableAST>(tok.value, paramType, VarCtx::eParam, tok.loc);
            params.push_back(std::move(param));
            eat(Token::TOK_ID);
            if (tok == Token::TOK_COMMA)
                eat(Token::TOK_COMMA);
            else
                break;
        }
    }
    return params;
}

std::vector<std::unique_ptr<ExpressionAST>> Parser::parseCallParams()
{
    std::vector<std::unique_ptr<ExpressionAST>> params;
    if (tok.type != Token::TOK_CPAREN)
    {
        while (true)
        {
            params.push_back(parseExpr());
            if (tok == Token::TOK_COMMA)
                eat(Token::TOK_COMMA);
            else
                break;
        }
    }
    return params;
}

std::unique_ptr<CompoundAST> Parser::parseCompound()
{
    eat(Token::TOK_OBRACK);
    auto compound = std::make_unique<CompoundAST>();

    // bool to catch early returns
    bool didReturn = false;
    int prevWarnLine = -1;
    while (tok != Token::TOK_CBRACK)
    {
        if (!didReturn && tok.type == Token::TOK_KWD && tok.value == "return")
        {
            didReturn = true;
            compound->setRetStmt(parseReturnStmt());
        }
        else
        {
            // check if early return was hit
            if (didReturn)
            {
                // if parsing multiple statements on same line separated by
                // semicolon, need to only output warning once
                if (prevWarnLine != tok.loc.y)
                {
                    std::cout << "Warning! Parsing line past early return.\n";
                    std::cout << "  " << lexer.getLine(tok.loc.y) << '\n';
                }
                prevWarnLine = tok.loc.y;
                parsePrimary();
            }
            else
            {
                compound->addChild(parsePrimary());
            }
        }

    }
    eat(Token::TOK_CBRACK);
    if (tok.type == Token::TOK_SCOLON)
        eat(Token::TOK_SCOLON);
    return compound;
}

std::unique_ptr<ReturnAST> Parser::parseReturnStmt()
{
    eat(Token::TOK_KWD);
    if (tok.type == Token::TOK_SCOLON)
    {
        eat(Token::TOK_SCOLON);
        return ReturnAST::retVoid();
    }
    std::unique_ptr<ExpressionAST> expr = parseExpr();
    eat(Token::TOK_SCOLON);
    return std::make_unique<ReturnAST>(std::move(expr));
}

std::unique_ptr<IfAST> Parser::parseIfStmt()
{
    eat(Token::TOK_KWD);
    eat(Token::TOK_OPAREN);
    if (tok.type == Token::TOK_CPAREN)
        throw SyntaxError("expected expression", getTokenLine(tok), tok.loc);
    std::unique_ptr<ExpressionAST> expr = parseExpr();
    eat(Token::TOK_CPAREN);
    std::unique_ptr<CompoundAST> body = parseCompound();
    std::unique_ptr<IfAST> other = nullptr;
    if (tok.type == Token::TOK_KWD && tok.value == "else")
    {
        eat();
        if (tok.type == Token::TOK_KWD && tok.value == "if")
            // parse 'else if'
            other = parseIfStmt();
        else
            // parse 'else' (neither 'expr' nor 'other' should be present)
            other = std::make_unique<IfAST>(nullptr, parseCompound(), nullptr);
    }
    return std::make_unique<IfAST>(std::move(expr), std::move(body), std::move(other));
}

std::unique_ptr<ForAST> Parser::parseForStmt()
{
    // eat(Token::TOK_KWD);
    // eat(Token::TOK_OPAREN);
    // if (tok.type == Token::TOK_CPAREN)
    //     throw SyntaxError("expected expression", getTokenLine(tok), tok.loc);
    // std::unique_ptr<VariableAST> var;
    // if (tok.isType())
    // {
    //     Type varType = tok.toType();
    //     eat(Token::TOK_TYPE);
    //     var = std::make_unique<VariableAST>(tok.value, varType, VarCtx::eAlloc, tok.loc);
    // }
    // else
    // {
    //     var = std::make_unique<VariableAST>(tok.value, tok.loc);
    // }
    // eat(Token::TOK_ID);
    // eat(Token::TOK_KWD);
    // std::unique_ptr<ArrayAST> gen = parseIntArrayGen();
    // auto expr = std::make_unique<ExpressionAST>(std::move(var), std::unique_ptr<AST>(gen->elements[0]->clone()), Operator::OP_EQL);
    // eat(Token::TOK_CPAREN);
    // std::unique_ptr<CompoundAST> body = parseCompound();
    // return std::make_unique<ForAST>(std::move(expr), std::move(gen), std::move(body));
    throw NotImplementedError("parsing of ForAST");
}

std::unique_ptr<ArrayAST> Parser::parseIntArrayGen()
{

    /*
     * XXX XXX TODO XXX XXX
     * This is a terrible temporary solution. This should be a generator
     * instead of an allocated array because the next value can be computed
     * very easily. For example:
     *
     * for (int i in 0..2000) {}
     *
     * allocates an array with 2000 elements in it. That's dumb. What can be
     * done is on each iteration, the control value, in this case 'i', can be
     * set to the next value of the iteration. If 'i' is 10, just set it to 11
     * on the next loop because we know that is what it will be if this close
     * ranged is used (start..stop..step). There's no point in allocating an
     * entire array for this.
     *
     * The purpose of this shit solution is a proof-of-concept for arrays and
     * iterating through them.
     *
     *
     */
    // int start, stop;
    // int step = 1;
    // start = std::stoi(tok.value);
    // eat(Token::TOK_NUM);
    // eat(Token::TOK_PERIOD);
    // eat(Token::TOK_PERIOD);
    // stop = std::stoi(tok.value);
    // eat(Token::TOK_NUM);
    // if (tok.type == Token::TOK_PERIOD)
    // {
    //     eat(Token::TOK_PERIOD);
    //     eat(Token::TOK_PERIOD);
    //     step = std::stoi(tok.value);
    //     eat(Token::TOK_NUM);
    // }
    // std::vector<std::unique_ptr<AST>> values;
    // for (int i = start; i < stop; i += step)
    //     values.push_back(std::make_unique<NumberAST>(i));
    //
    // return std::make_unique<ArrayAST>(Type::eInt, std::move(values));
    throw NotImplementedError("parsing of intArrayGen");
}

std::unique_ptr<WhileAST> Parser::parseWhileStmt()
{
    eat(Token::TOK_KWD);
    eat(Token::TOK_OPAREN);
    if (tok.type == Token::TOK_CPAREN)
        throw SyntaxError("expected expression", getTokenLine(tok), tok.loc);
    auto expr = std::make_shared<ExpressionAST>(parseExpr());
    eat(Token::TOK_CPAREN);
    std::unique_ptr<CompoundAST> body = parseCompound();
    return std::make_unique<WhileAST>(expr, std::move(body));
}

std::unique_ptr<AST> Parser::parseIdTerm() 
{
    bool isAlloc = tok.type == Token::TOK_TYPE;
    Type allocType = Type::eNot;
    if (isAlloc)
    {
        allocType = typeFromString(tok.value);
        eat(Token::TOK_TYPE);
    }

    Token tmpIdTok = tok;
    eat(Token::TOK_ID);
    // check if function call
    if (tok.type == Token::TOK_OPAREN)
    {
        if (isAlloc)
            // force a syntax error
            eat(Token::TOK_EQUALS); 
        SourceLocation paramsStartLoc = SourceLocation(tok.loc.x + 1, tok.loc.y, tok.loc.len);
        eat(Token::TOK_OPAREN);
        auto callAST = std::make_unique<CallAST>(tmpIdTok.value, parseCallParams(), tmpIdTok.loc, paramsStartLoc);
        eat(Token::TOK_CPAREN);
        return callAST;
    }
    if (isAlloc && tok.type != Token::TOK_EQUALS)
      throw SyntaxError("expected assignment expression", getTokenLine(tok), tok.loc);

    std::unique_ptr<VariableAST> varAst;
    if (isAlloc)
      varAst = std::make_unique<VariableAST>(tmpIdTok.value, allocType, VarCtx::eAlloc, tmpIdTok.loc);
    else
      varAst = std::make_unique<VariableAST>(tmpIdTok.value, tmpIdTok.loc);
    return varAst;
}

std::unique_ptr<UnaryExprAST> Parser::parsePreUnaryExpr()
{
    Operator::op_type unaryOpType = tok.toOperator().getType();
    eat();
    std::unique_ptr<AST> operand = parseOperand(); 
    return UnaryExprAST::unaryPrefix(std::move(operand), unaryOpType);
}

std::unique_ptr<UnaryExprAST> Parser::parsePostUnaryExpr(std::unique_ptr<AST> operand)
{
    // refactor this (duplicated in parsePreUnaryExpr()
    Operator::op_type unaryOpType = tok.toOperator().getType();
    eat();
    return UnaryExprAST::unaryPostfix(std::move(operand), unaryOpType);
}

std::unique_ptr<AST> Parser::parseOperand() 
{
    switch (tok.type)
    {
        case Token::TOK_NUM:
            return parseNum();
        case Token::TOK_TYPE:
        case Token::TOK_ID:
            return parseIdTerm();
        case Token::TOK_OPAREN:
            {
                eat(Token::TOK_OPAREN);
                if (tok.type == Token::TOK_CPAREN)
                    throw SyntaxError("expected expression", getTokenLine(tok), tok.loc);
                std::unique_ptr<ExpressionAST> expr = parseExpr();
                eat(Token::TOK_CPAREN);
                return expr;
            }
        case Token::TOK_OSQUARE:
            return parseArray();
        case Token::TOK_SCOLON:
            // specific error message
            throw SyntaxError("expected expression", getTokenLine(tok), tok.loc);
        case Token::TOK_KWD:
            {
              SourceLocation numLoc = tok.loc;
              if (tok.value == "true")
              {
                eat(Token::TOK_KWD);
                return std::make_unique<NumberAST>(1, Type::eBool, numLoc);
              }
              else if (tok.value == "false")
              {
                eat(Token::TOK_KWD);
                return std::make_unique<NumberAST>(0, Type::eBool, numLoc);
              }
            }
        default:
            {
                std::string msg = fmt::format("'{}' is not a valid operand", tok.value);
                throw SyntaxError(msg, getTokenLine(tok), tok.loc);
            }
    }

}

std::unique_ptr<AST> Parser::parseTerm() 
{
    // check if current token is a unary operator
    if (isTokUnary())
        return parsePreUnaryExpr();

    // if not a pre-unary, it could be post-unary
    std::unique_ptr<AST> operand = parseOperand();
    if (isTokUnary())
        return parsePostUnaryExpr(std::move(operand));
    return operand;
}

bool Parser::isTokUnary()
{
    return (tok.type == Token::TOK_DPLUS || tok.type == Token::TOK_DMINUS);
}

std::unique_ptr<ExpressionAST> Parser::parseExpr()
{  
  return parseSubExpr(std::make_unique<ExpressionAST>(parseTerm()));
}

std::unique_ptr<ExpressionAST> Parser::parseSubExpr(std::unique_ptr<ExpressionAST> L, int prec)
{
    Operator nextOp = tok.toOperator();
    SourceLocation opLoc = tok.loc;
    while (nextOp >= prec)
    {
        Operator currOp = nextOp;
        eat();
        std::unique_ptr<AST> R = parseTerm();
        nextOp = tok.toOperator();
        while ((nextOp > currOp) || ((currOp == nextOp) && (nextOp.getAssoc() == Operator::A_RIGHT)))
        {
            auto RHS = std::unique_ptr<AST>(R->clone());
            int nextPrec = currOp.getPrec();
            if (currOp.getAssoc() == Operator::A_LEFT)
                nextPrec++;
            R = parseSubExpr(std::make_unique<ExpressionAST>(std::move(RHS)), nextPrec);
            nextOp = tok.toOperator();
            opLoc = tok.loc;
        }
        auto LHS = std::unique_ptr<AST>(L->clone());
        L = std::make_unique<ExpressionAST>(std::move(LHS), std::move(R), currOp.getType(), opLoc);
    }
    return L;
}

bool Parser::eat(Token::token_type expectedType)
{
    bool badToken = (tok.type != expectedType);
    if (badToken)
    {
        std::string msg;
        if (tok.type == Token::TOK_EOF)
        {
            msg = fmt::format("expected '{}' but encountered end-of-file (EOF)", tokenValues.at(expectedType));
        }
        else
        {
            switch (expectedType)
            {
                case Token::TOK_EOF:
                    break;
                case Token::TOK_ID:
                    msg = "expected identifier";
                    break;
                case Token::TOK_TYPE:
                    msg = fmt::format("expected type but got '{}' instead", tok.value);
                    break;
                default:
                    msg = fmt::format("expected '{}' but got '{}' instead", tokenValues.at(expectedType), tok.value);
                    break;
            }
        }
        throw SyntaxError(msg, getTokenLine(tok), tok.loc);
    }
    getToken();
    return badToken;
}

bool Parser::eat()
{
    // this method is for eating current token no matter its type
    return eat(tok.type);
}

void Parser::getToken()
{
    tok = lexer.nextToken();
    if (tok.type == Token::TOK_UND)
    {
        std::string msg = fmt::format("encountered unknown character '{}'", tok.value);
        throw SyntaxError(msg, getTokenLine(tok), tok.loc);
    }
}

std::string Parser::getTokenLine(Token tok)
{
    return lexer.getLine(tok.loc.y);
}

std::unique_ptr<ModuleAST> getAstFromFile(const FilePath& fp, ContextManager& ctx)
{
    if (!fp.exists())
        return nullptr;
    Parser parser(fp, ctx);
    return parser.parse();
}
