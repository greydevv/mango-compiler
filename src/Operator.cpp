#include <iostream>
#include "Token.h"
#include "Operator.h"

Operator::Operator(op_type type, int prec, op_assoc assoc)
    : type(type), prec(prec), assoc(assoc) {}

int Operator::getPrec()
{
    return prec;
}

Operator::op_type Operator::getType()
{
    return type;
}

Operator::op_assoc Operator::getAssoc()
{
    return assoc;
}

Operator Operator::opEqual()
{
    return Operator(Operator::OP_EQL, 1, Operator::A_RIGHT);
}

Operator Operator::opAdd()
{
    return Operator(Operator::OP_ADD, 2, Operator::A_LEFT);
}

Operator Operator::opSub()
{
    return Operator(Operator::OP_SUB, 2, Operator::A_LEFT);
}

Operator Operator::opMultiply()
{
    return Operator(Operator::OP_SUB, 3, Operator::A_LEFT);
}

Operator Operator::opDivide()
{
    return Operator(Operator::OP_SUB, 3, Operator::A_LEFT);
}

Operator Operator::opPower()
{
    return Operator(Operator::OP_SUB, 4, Operator::A_LEFT);
}

Operator Operator::opUnknown()
{
    return Operator(Operator::OP_SUB, -1, Operator::A_LEFT);
}

bool Operator::operator==(const Operator& other)
{
    return prec == other.prec;    
}

bool Operator::operator!=(const Operator& other)
{
    return prec != other.prec;
}

bool Operator::operator<=(const Operator& other)
{
    return prec <= other.prec;
}

bool Operator::operator>=(const Operator& other)
{
    return prec >= other.prec;
}

bool Operator::operator<(const Operator& other)
{
    return prec < other.prec;
}

bool Operator::operator>(const Operator& other)
{
    return prec > other.prec;
}

bool Operator::operator==(const Operator& other)
{
    return prec == other.prec;
}

bool Operator::operator!=(const Operator& other)
{
    return prec != other.prec;
}

bool Operator::operator<=(const Operator& other)
{
    return prec <= other.prec;
}

bool Operator::operator>=(const Operator& other)
{
    return prec >= other.prec;
}

bool Operator::operator< (const Operator& other)
{
    return prec < other.prec;
}

bool Operator::operator> (const Operator& other)
{
    return prec > other.prec;
}

std::ostream& operator<<(std::ostream& os, const Operator& op)
{
    os << '<' << operatorStrings[op.type] << '>';
}