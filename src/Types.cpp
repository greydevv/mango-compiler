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
    else if (str == "array")
        return Type::eArray;
    else
        return Type::eUnd;
}
