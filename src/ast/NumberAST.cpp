#include "NumberAST.h"
#include "../Token.h"
#include "../Types.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"


// #include <iostream>
NumberAST::NumberAST(double val, Type type, SourceLocation loc)
    : AST(loc), val(val), type(type)
{
  // std::cout << loc.y << "<---(" << val << ")\n";
}

NumberAST::NumberAST(double val, SourceLocation loc)
    : AST(loc), val(val), type(Type::eInt)
{
  // std::cout << loc.y << "<---(" << val << ")\n";
}

NumberAST::NumberAST(const NumberAST& other)
    : AST(other.loc), val(other.val), type(other.type)
{
  // std::cout << other.loc.y << "<---(copy" << "(" << other.val << ")" << ")\n";
  // std::cout << loc.y << "<---(copy" << "(" << val << ")" << ")\n";
}

Type NumberAST::accept(ASTValidator& vd)
{
    return vd.validate(this);
}

llvm::Value* NumberAST::accept(ASTCodegenner& cg)
{
    return cg.codegen(this);
}

std::string NumberAST::accept(ASTStringifier& sf, int tabs)
{
    return sf.toString(this, tabs);
}

NumberAST* NumberAST::cloneImpl()
{
    return new NumberAST(*this);
}
