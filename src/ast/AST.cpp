#include "AST.h"

bool AST::isAssignable() {
    return false;
}

AST* AST::clone() 
{
    return cloneImpl();
}
