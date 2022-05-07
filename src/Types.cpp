#include "Types.h"
#include <sstream>

std::string typeToString(Type t)
{
    return typeValues[t];
}

Type typeFromString(const std::string& str)
{
    if (str == "void")
        return Type::eVoid;
    else if (str == "int")
        return Type::eInt;
    else if (str == "bool")
        return Type::eBool;
    else if (str == "int")
        return Type::eInt;
    else if (str == "array")
        return Type::eArray;
    else
        return Type::eUnd;
}

bool isArrayType(Type t)
{
    return (t == Type::eArray);
}

Type getArrayType(Type eleType)
{
    switch (eleType)
    {
        case Type::eInt:
            return Type::eArray;
        default:
            return Type::eUnd;
    }
}
