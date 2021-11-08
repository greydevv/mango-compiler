#include <ostream>
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

Operator Operator::opSubtract()
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

bool operator==(int i, const Operator& op)
{
    return i == op.prec;
}

bool operator!=(int i, const Operator& op)
{
    return i != op.prec;
}

bool operator<=(int i, const Operator& op)
{
    return i <= op.prec;
}

bool operator>=(int i, const Operator& op)
{
    return i >= op.prec;
}

bool operator< (int i, const Operator& op)
{
    return i < op.prec;
}

bool operator> (int i, const Operator& op)
{
    return i > op.prec;
}

bool operator==(const Operator& op, int i)
{
    return op.prec == i;
}

bool operator!=(const Operator& op, int i)
{
    return op.prec != i;
}

bool operator<=(const Operator& op, int i)
{
    return op.prec <= i;
}

bool operator>=(const Operator& op, int i)
{
    return op.prec >= i;
}

bool operator< (const Operator& op, int i)
{
    return op.prec < i;
}

bool operator> (const Operator& op, int i)
{
    return op.prec > i;
}

std::ostream& operator<<(std::ostream& os, const Operator& op)
{
    os << '<' << operatorStrings[op.type] << '>';
    return os;
}
