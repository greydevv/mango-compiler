#include "Types.h"
#include <sstream>

std::string typeToString(Type t)
{
    return typeValues[t];
}

Type typeFromString(const std::string& str)
{
    if (str == "int")
        return Type::eInt;
    else if (str == "void")
        return Type::eVoid;
    else
        return Type::eUnd;
}
