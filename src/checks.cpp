#include <iostream>
#include <cassert>
#include "checks.h"
#include "Token.h"
#include "Operator.h"

void runChecks()
{
    // tokens
    unsigned numTokens = Token::NUM_TOKS;
    assert(tokenStrings.size() == numTokens);
    assert(tokenValues.size() == numTokens);
    
    // operators
    unsigned numOps = Operator::NUM_OPS;
    assert(operatorStrings.size() == numOps);
    assert(operatorValues.size() == numOps);
   
    // types
    unsigned numTypes = Type::NUM_TYPES;
    assert(typeStrings.size() == numTypes);
    assert(typeValues.size() == numTypes);
    std::cout << "\nAll pre-compilation assertions passed!\n\n";
}
