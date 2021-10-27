#include "AST.h"

AST* AST::clone() 
{
    return cloneImpl();
}