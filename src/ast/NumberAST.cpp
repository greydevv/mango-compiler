#include "NumberAST.h"
#include "../Types.h"
#include "../visitors/ASTValidator.h"
#include "../visitors/ASTCodegenner.h"
#include "../visitors/ASTStringifier.h"

NumberAST::NumberAST(double val, Type type)
    : val(val), type(type) {}

NumberAST::NumberAST(double val)
    : val(val), type(Type::eInt) {}

NumberAST::NumberAST(const NumberAST& other)
    : val(other.val), type(other.type) {}

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
