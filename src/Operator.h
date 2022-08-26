#ifndef OPERATOR_H
#define OPERATOR_H

#include <ostream>
#include <vector>
#include <map>
#include <string>

class Operator final
{
    public:
        enum op_type
        {
            OP_DEC,     // unary decrement (--)
            OP_INC,     // unary increment (++)
            OP_EQL,
            OP_ADD,
            OP_SUB,
            OP_MUL,
            OP_DIV,
            // OP_EXP,
            OP_BOOL_GT,
            OP_BOOL_LT,
            OP_BOOL_GTE,
            OP_BOOL_LTE,
            OP_BOOL_EQL,
            OP_BOOL_NEQL,
            OP_BOOL_OR,
            OP_BOOL_AND,
            OP_NOP,
            NUM_OPS,
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
        static Operator opSubtract();
        static Operator opMultiply();
        static Operator opDivide();
        static Operator opDecrement();
        static Operator opIncrement();
        // static Operator opPower();
        static Operator opGT();
        static Operator opLT();
        static Operator opGTE();
        static Operator opLTE();
        static Operator opEqualTo();
        static Operator opNotEqualTo();
        static Operator opLogicalOr();
        static Operator opLogicalAnd();
        static Operator opUnknown();
        bool operator==(const Operator& other) const;
        bool operator!=(const Operator& other) const;
        bool operator<=(const Operator& other) const;
        bool operator>=(const Operator& other) const;
        bool operator< (const Operator& other) const;
        bool operator> (const Operator& other) const;
        friend bool operator==(int i, const Operator& op);
        friend bool operator!=(int i, const Operator& op);
        friend bool operator<=(int i, const Operator& op);
        friend bool operator>=(int i, const Operator& op);
        friend bool operator< (int i, const Operator& op);
        friend bool operator> (int i, const Operator& op);
        friend bool operator==(const Operator& op, int i);
        friend bool operator!=(const Operator& op, int i);
        friend bool operator<=(const Operator& op, int i);
        friend bool operator>=(const Operator& op, int i);
        friend bool operator< (const Operator& op, int i);
        friend bool operator> (const Operator& op, int i);
        friend std::ostream& operator<<(std::ostream& os, const Operator& op);
    
    private:
        op_type type;
        int prec;
        op_assoc assoc;

        Operator(op_type type, int prec, op_assoc assoc);
};

static const std::vector<std::string> operatorStrings = {
    "OP_EQL",
    "OP_ADD",
    "OP_SUB",
    "OP_MUL",
    "OP_DIV",
    "OP_DEC",
    "OP_INC",
    // "OP_EXP",
    "OP_BOOL_GT",
    "OP_BOOL_LT",
    "OP_BOOL_GTE",
    "OP_BOOL_LTE",
    "OP_BOOL_EQL",
    "OP_BOOL_NEQL",
    "OP_BOOL_OR",
    "OP_BOOL_AND",
    "OP_NOP",
};

static const std::map<Operator::op_type, std::string> operatorValues = {
    {Operator::OP_EQL, "="},
    {Operator::OP_ADD, "+"},
    {Operator::OP_SUB, "-"},
    {Operator::OP_MUL, "*"},
    {Operator::OP_DIV, "/"},
    {Operator::OP_DEC, "--"},
    {Operator::OP_INC, "++"},
    {Operator::OP_BOOL_GT, ">"},
    {Operator::OP_BOOL_LT, "<"},
    {Operator::OP_BOOL_GTE, ">="},
    {Operator::OP_BOOL_LTE, "<="},
    {Operator::OP_BOOL_EQL, "=="},
    {Operator::OP_BOOL_NEQL, "!="},
    {Operator::OP_BOOL_OR, "||"},
    {Operator::OP_BOOL_AND, "&&"},
    {Operator::OP_NOP, ""},
    // {Operator::OP_EXP, "**"}
};

#endif
