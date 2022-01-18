#ifndef TYPES_H
#define TYPES_H

#include <sstream>
#include <map>

enum class Type
{
    tVoid,
    tInt,
    tUnd,
};

std::string typeToString(Type t);

static std::map<Type, std::string> typeValues = {
    {Type::tVoid, "void"},
    {Type::tInt, "int"},
    {Type::tUnd, "UND"},
};

#endif
