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
    else if (str == "array")
        return Type::eArray;
    else
        return Type::eUnd;
}

bool isNumeric(Type t)
{
    // return 'true' if type is considered numeric, otherwise false
    switch (t)
    {
        case Type::eInt:
        case Type::eBool:
            return true;
        default:
            return false;
    }
}

bool typeCompat(Type a, Type b)
{
    // check if type 'a' is compatible with type 'b' for weak-typing
    // right now, only numeric types are compatible with eachother
    if (isNumeric(a) && isNumeric(b))
        return true;
    return false;
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
