#include <cassert>
#include "checks.h"
#include "Token.h"
#include "Operator.h"

void runChecks()
{
    // check tokens
    unsigned int numTokens = Token::NUM_TOKS;
    assert(tokenStrings.size() == numTokens);
    
    // check operators
    unsigned int numOps = Operator::NUM_OPS;
    assert(operatorStrings.size() == numOps);
    assert(operatorValues.size() == numOps);
    
}
