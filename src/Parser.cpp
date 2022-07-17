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
    {
        modAST->addChild(parsePrimary());
    }
    return modAST;
}

std::unique_ptr<AST> Parser::parsePrimary() 
{
    switch (tok.type)
    {
        case Token::TOK_KWD:
            return parseKwd();
        case Token::TOK_ID:
            // either variable redefinition, reference
            return parseId();
        case Token::TOK_TYPE:
            // assuming variable declaration
            return parseVarDef();
        default:
        {
            std::unique_ptr<AST> exprAST = parseExpr();
            eat(Token::TOK_SCOLON);
            return exprAST;
        }
    }
}

std::unique_ptr<ExpressionAST> Parser::parseVarDef()
{
    Type allocType = typeFromString(tok.value);
    eat(Token::TOK_TYPE);
    if (isArrayType(allocType))
    {
        throw NotImplementedError("parsing of ArrayAST");
        // need to eat second type that denotes the type of the elements
        // eat(Token::TOK_LT);
        // Type eleType = typeFromString(tok.value);
        // eat(Token::TOK_TYPE);
        // eat(Token::TOK_GT);
    }

    std::string id = tok.value;
    auto allocVar = std::make_unique<VariableAST>(tok.value, allocType, VarCtx::eAlloc, tok.loc);
    eat(Token::TOK_ID);
    return createVarAssignExpr(std::move(allocVar));
}

std::unique_ptr<ExpressionAST> Parser::parseVarStore()
{
    std::string id = tok.value;
    auto storeVar = std::make_unique<VariableAST>(id, VarCtx::eStore, tok.loc);
    eat(Token::TOK_ID);
    return createVarAssignExpr(std::move(storeVar));
}

std::unique_ptr<ExpressionAST> Parser::createVarAssignExpr(std::unique_ptr<VariableAST> var)
{
    eat(Token::TOK_EQUALS);
    auto expr = std::make_unique<ExpressionAST>(std::move(var), parseExpr(), Operator::OP_EQL);
    eat(Token::TOK_SCOLON);
    return expr;
}

std::unique_ptr<AST> Parser::parseKwd()
{
    if (tok.value == "return")
    {
        return parseReturnStmt();
    }
    else if (tok.value == "func")
    {
        return parseFuncDef();
    }
    else if (tok.value == "if")
    {
        return parseIfStmt();
    }
    else if (tok.value == "for")
    {
        return parseForStmt();
    }
    else if (tok.value == "while")
    {
        return parseWhileStmt();
    }
    else if (tok.value == "extern")
    {
        return parseExternStmt();
    }
    else if (tok.value == "include")
    {
        return parseIncludeStmt();
    }
    else
    {
        std::cout << "[Dev Error] Keyword handling for '" << tok.value << "' not yet implemented in parser.\n";
        exit(1);
    }
}

std::unique_ptr<AST> Parser::parseId()
{
    if (lexer.peekToken().type == Token::TOK_EQUALS)
    {
        return parseVarStore();
    }
    std::unique_ptr<AST> expr = parseExpr();
    eat(Token::TOK_SCOLON);
    return expr;
}

std::unique_ptr<NumberAST> Parser::parseNum() 
{
    std::unique_ptr<NumberAST> numAST = std::make_unique<NumberAST>(std::stod(tok.value));
    eat(Token::TOK_NUM);
    return numAST;
}

std::unique_ptr<AST> Parser::parseArray(Type eleType)
{
    // throw NotImplementedError(fp, "array parsing", tok.loc);
    // either basic array '[a,b,c,d]' or string '"abcd"'
    // if (eleType != Type::eString) {}

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
    {
        // need to add quotes back for correct underlining
        throw FileNotFoundError(includeFp, underlineTok(includeTok), includeTok.loc);
    }
    ctx.pop();
    return incAST;
}

std::unique_ptr<PrototypeAST> Parser::parseExternStmt()
{
    eat(Token::TOK_KWD);
    eat(Token::TOK_KWD);
    std::unique_ptr<PrototypeAST> proto = parseFuncProto();
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
    // if (st.contains(name))
    //     throw ReferenceError(fp, fmt::format("function '{}' already defined", name), underlineTok(tok), tok.loc);
    // else
    //     st.insert(name, Type::eFunc);
    eat(Token::TOK_ID);
    std::vector<std::unique_ptr<VariableAST>> params = parseFuncParams();
    Type retType = Type::eVoid;
    if (tok == Token::TOK_RARROW)
    {
        eat(Token::TOK_RARROW);
        retType = tok.toType();
        eat(Token::TOK_TYPE);
    }
    return std::make_unique<PrototypeAST>(name, retType, std::move(params));
}

std::vector<std::unique_ptr<VariableAST>> Parser::parseFuncParams()
{
    // TODO: refactor this and parseFuncParams into a method like parseCsv
    std::vector<std::unique_ptr<VariableAST>> params;
    eat(Token::TOK_OPAREN);
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
            {
                eat(Token::TOK_COMMA);
            }
            else
            {
                break;
            }
        }
    }
    eat(Token::TOK_CPAREN);
    return params;
}

std::vector<std::unique_ptr<AST>> Parser::parseCallParams()
{
    // TODO: refactor this and parseFuncParams into a method like parseCsv
    std::vector<std::unique_ptr<AST>> params;
    eat(Token::TOK_OPAREN);
    if (tok.type != Token::TOK_CPAREN)
    {
        while (true)
        {
            std::unique_ptr<AST> param = parseExpr();
            params.push_back(std::move(param));
            if (tok == Token::TOK_COMMA)
            {
                eat(Token::TOK_COMMA);
            }
            else
            {
                break;
            }
        }
    }
    eat(Token::TOK_CPAREN);
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
    std::unique_ptr<AST> expr = parseExpr();
    eat(Token::TOK_SCOLON);
    return std::make_unique<ReturnAST>(std::move(expr));
}

std::unique_ptr<IfAST> Parser::parseIfStmt()
{
    eat(Token::TOK_KWD);
    eat(Token::TOK_OPAREN);
    if (tok.type == Token::TOK_CPAREN)
        throw SyntaxError("expected expression", underlineTok(tok), tok.loc);
    std::unique_ptr<AST> expr = parseExpr();
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
    eat(Token::TOK_KWD);
    eat(Token::TOK_OPAREN);
    if (tok.type == Token::TOK_CPAREN)
        throw SyntaxError("expected expression", underlineTok(tok), tok.loc);
    std::unique_ptr<VariableAST> var;
    if (tok.isType())
    {
        Type varType = tok.toType();
        eat(Token::TOK_TYPE);
        var = std::make_unique<VariableAST>(tok.value, varType, VarCtx::eAlloc, tok.loc);
    }
    else
    {
        var = std::make_unique<VariableAST>(tok.value, tok.loc);
    }
    eat(Token::TOK_ID);
    eat(Token::TOK_KWD);
    std::unique_ptr<ArrayAST> gen = parseIntArrayGen();
    auto expr = std::make_unique<ExpressionAST>(std::move(var), std::unique_ptr<AST>(gen->elements[0]->clone()), Operator::OP_EQL);
    eat(Token::TOK_CPAREN);
    std::unique_ptr<CompoundAST> body = parseCompound();
    return std::make_unique<ForAST>(std::move(expr), std::move(gen), std::move(body));
    // throw NotImplementedError(fp, "For loop parsing.", SourceLocation(0,0));
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
    int start, stop;
    int step = 1;
    start = std::stoi(tok.value);
    eat(Token::TOK_NUM);
    eat(Token::TOK_PERIOD);
    eat(Token::TOK_PERIOD);
    stop = std::stoi(tok.value);
    eat(Token::TOK_NUM);
    if (tok.type == Token::TOK_PERIOD)
    {
        eat(Token::TOK_PERIOD);
        eat(Token::TOK_PERIOD);
        step = std::stoi(tok.value);
        eat(Token::TOK_NUM);
    }
    std::vector<std::unique_ptr<AST>> values;
    for (int i = start; i < stop; i += step)
    {
        values.push_back(std::make_unique<NumberAST>(i));
    }

    return std::make_unique<ArrayAST>(Type::eInt, std::move(values));
}

std::unique_ptr<WhileAST> Parser::parseWhileStmt()
{
    eat(Token::TOK_KWD);
    eat(Token::TOK_OPAREN);
    if (tok.type == Token::TOK_CPAREN)
        throw SyntaxError("expected expression", underlineTok(tok), tok.loc);
    std::unique_ptr<AST> expr = parseExpr();
    eat(Token::TOK_CPAREN);
    std::unique_ptr<CompoundAST> body = parseCompound();
    return std::make_unique<WhileAST>(std::move(expr), std::move(body));
}

std::unique_ptr<AST> Parser::parseIdTerm() 
{
    Token tmpIdTok = tok;
    eat(Token::TOK_ID);
    // check if function call
    if (tok.type == Token::TOK_OPAREN)
    {
        // if (!st.contains(id))
        //     throw ReferenceError(fp, fmt::format("unknown function name '{}'", id), underlineTok(tmpIdTok), tmpIdTok.loc);
        auto callAST = std::make_unique<CallAST>(tmpIdTok.value, parseCallParams());
        return callAST;
    }
    // if (!st.contains(id))
    //     throw ReferenceError(fp, fmt::format("unknown variable name '{}'", id), underlineTok(tmpIdTok), tmpIdTok.loc);
    auto varAST = std::make_unique<VariableAST>(tmpIdTok.value, tmpIdTok.loc);
    return varAST;
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
        case Token::TOK_ID:
            return parseIdTerm();
        case Token::TOK_OPAREN:
            {
                eat(Token::TOK_OPAREN);
                if (tok.type == Token::TOK_CPAREN)
                    throw SyntaxError("expected expression", underlineTok(tok), tok.loc);
                std::unique_ptr<AST> expr = parseExpr();
                eat(Token::TOK_CPAREN);
                return expr;
            }
        case Token::TOK_OSQUARE:
            return parseArray();
        case Token::TOK_SCOLON:
            {
                // specific error message
                throw SyntaxError("expected expression", underlineTok(tok), tok.loc);
            }
        case Token::TOK_KWD:
            if (tok.value == "true")
            {
                eat(Token::TOK_KWD);
                // TODO: NumberAST creates 32-bit ints, but we need 1-bit for
                // bool type
                return std::make_unique<NumberAST>(1);
            }
            else if (tok.value == "false")
            {
                eat(Token::TOK_KWD);
                return std::make_unique<NumberAST>(0);
            }
        default:
            {
                std::string msg = fmt::format("'{}' is not a valid operand", tok.value);
                throw SyntaxError(msg, underlineTok(tok), tok.loc);
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

std::unique_ptr<AST> Parser::parseExpr()
{  
    return parseSubExpr(parseTerm());
}

std::unique_ptr<AST> Parser::parseSubExpr(std::unique_ptr<AST> L, int prec)
{
    Operator nextOp = tok.toOperator();
    while (nextOp >= prec)
    {
        if (nextOp.getType() == Operator::OP_EQL)
        {
            throw SyntaxError("expression is not assignable", underlineTok(tok), tok.loc);
        }
        Operator currOp = nextOp;
        eat();
        std::unique_ptr<AST> R = parseTerm();
        nextOp = tok.toOperator();
        while ((nextOp > currOp) || ((currOp == nextOp) && (nextOp.getAssoc() == Operator::A_RIGHT)))
        {
            if (nextOp.getType() == Operator::OP_EQL)
                throw SyntaxError("expression is not assignable", underlineTok(tok), tok.loc);
            auto RHS = std::unique_ptr<AST>(R->clone());
            if (currOp.getAssoc() == Operator::A_RIGHT)
                R = parseSubExpr(std::move(RHS), currOp.getPrec());
            else
                R = parseSubExpr(std::move(RHS), currOp.getPrec()+1);
            nextOp = tok.toOperator();
        }
        auto LHS = std::unique_ptr<AST>(L->clone());
        L = std::make_unique<ExpressionAST>(std::move(LHS), std::move(R), currOp.getType());
    }
    return L;
}

bool Parser::eat(Token::token_type expectedType)
{
    bool badToken = (tok.type != expectedType);
    if (badToken)
    {
        setErrState(1);
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
        throw SyntaxError(msg, underlineTok(tok), tok.loc);
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
    // std::cout << tok << '\n';
    if (tok.type == Token::TOK_UND)
    {
        std::string msg = fmt::format("encountered unknown character '{}'", tok.value);
        throw SyntaxError(msg, underlineTok(tok), tok.loc);
    }
}

int Parser::getErrState()
{
    return errState;
}

void Parser::setErrState(int errState) 
{
    this->errState = errState;
}

std::string Parser::underlineTok(Token tok)
{
    int len = tok.value.size();
    if (tok.type == Token::TOK_EOF)
        // set len to 1, real value is 'EOF' but we don't need to underline
        // that
        len = 1;
    else if (tok.type == Token::TOK_STR)
        // because value of TOK_STR is stored w/o quotes, we need to add 2 to
        // include the quotes in the underline
        len+=2;

    // helper method for io/underlineError
    return underlineError(lexer.getLine(tok.loc.y), tok.loc.x, len);
}

std::unique_ptr<ModuleAST> getAstFromFile(const FilePath& fp, ContextManager& ctx)
{
    if (!fp.exists())
        return nullptr;
    Parser parser(fp, ctx);
    return parser.parse();
}
