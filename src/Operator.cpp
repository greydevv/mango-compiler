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

Operator Operator::opLogicalOr()
{
    return Operator(Operator::OP_BOOL_OR, 2, Operator::A_LEFT);
}

Operator Operator::opLogicalAnd()
{
    return Operator(Operator::OP_BOOL_AND, 3, Operator::A_LEFT);
}

Operator Operator::opEqualTo()
{
    return Operator(Operator::OP_BOOL_EQL, 4, Operator::A_LEFT);
}

Operator Operator::opNotEqualTo()
{
    return Operator(Operator::OP_BOOL_NEQL, 4, Operator::A_LEFT);
}

Operator Operator::opGT()
{
    return Operator(Operator::OP_BOOL_GT, 5, Operator::A_LEFT);
}

Operator Operator::opGTE()
{
    return Operator(Operator::OP_BOOL_GTE, 5, Operator::A_LEFT);
}

Operator Operator::opLT()
{
    return Operator(Operator::OP_BOOL_LT, 5, Operator::A_LEFT);
}

Operator Operator::opLTE()
{
    return Operator(Operator::OP_BOOL_LTE, 5, Operator::A_LEFT);
}

Operator Operator::opAdd()
{
    return Operator(Operator::OP_ADD, 6, Operator::A_LEFT);
}

Operator Operator::opSubtract()
{
    return Operator(Operator::OP_SUB, 6, Operator::A_LEFT);
}

Operator Operator::opMultiply()
{
    return Operator(Operator::OP_MUL, 7, Operator::A_LEFT);
}

Operator Operator::opDivide()
{
    return Operator(Operator::OP_DIV, 7, Operator::A_LEFT);
}

// Operator Operator::opPower()
// {
//     // TODO: should I support this?
//     return Operator(Operator::OP_EXP, 4, Operator::A_RIGHT);
// }

Operator Operator::opUnknown()
{
    return Operator(Operator::OP_NOP, -1, Operator::A_LEFT);
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
