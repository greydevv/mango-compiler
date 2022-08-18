#include "AST.h"
#include "../Token.h"

AST::AST(SourceLocation loc)
    : loc(loc) {}

bool AST::isAssignable()
{
    return false;
}

AST* AST::clone()
{
    return cloneImpl();
}
