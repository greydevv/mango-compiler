#ifndef OPERATOR_H
#define OPERATOR_H

#include <map>
#include <string>
#include "Token.h"

class Operator final
{
    public:
        enum op_type
        {
            OP_EQL,
            OP_ADD,
            OP_SUB,
            OP_MUL,
            OP_DIV,
            OP_EXP,
            OP_NOP,
        };
        enum op_assoc
        {
            A_RIGHT,
            A_LEFT,
        };

        int getPrec();
        op_type getType();
        op_assoc getAssoc();
        static Operator opEqual();
        static Operator opAdd();
        static Operator opSub();
        static Operator opMultiply();
        static Operator opDivide();
        static Operator opPower();
        static Operator opUnknown();
        bool operator==(const Operator& other);
        bool operator!=(const Operator& other);
        bool operator<=(const Operator& other);
        bool operator>=(const Operator& other);
        bool operator< (const Operator& other);
        bool operator> (const Operator& other);
        friend bool operator==(int i, const Operator& op);
        friend bool operator!=(int i, const Operator& op);
        friend bool operator<=(int i, const Operator& op);
        friend bool operator>=(int i, const Operator& op);
        friend bool operator< (int i, const Operator& op);
        friend bool operator> (int i, const Operator& op);
        friend std::ostream& operator<<(std::ostream& os, const Operator& op);
    
    private:
        op_type type;
        int prec;
        int assoc;

        Operator(op_type type, int prec, op_assoc assoc);
};

static const std::string operatorStrings[] = {
    "OP_EQL",
    "OP_ADD",
    "OP_SUB",
    "OP_MUL",
    "OP_DIV",
    "OP_EXP",
    "OP_NOP",
};

static const std::map<Operator::op_type, std::string> operatorValues = {
    {Operator::OP_EQL, "="},
    {Operator::OP_ADD, "+"},
    {Operator::OP_SUB, "-"},
    {Operator::OP_MUL, "*"},
    {Operator::OP_DIV, "/"},
    {Operator::OP_EXP, "**"}
};

// 1: Right-associative
// 0: Left-associative
static const std::map<Operator::op_type, int> operatorAssociativity = {
    {Operator::OP_EQL, 1},
    {Operator::OP_ADD, 0},
    {Operator::OP_SUB, 0},
    {Operator::OP_MUL, 0},
    {Operator::OP_MUL, 0},
    {Operator::OP_DIV, 0},
    {Operator::OP_EXP, 1}
};

#endif