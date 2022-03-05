#ifndef TYPES_H
#define TYPES_H

#include <sstream>
#include <map>

enum class Type
{
    eVoid,
    eInt,
    eUnd,
};

std::string typeToString(Type t);
Type typeFromString(const std::string& str);

static std::map<Type, std::string> typeValues = {
    {Type::eVoid, "void"},
    {Type::eInt, "int"},
    {Type::eUnd, "undefined"},
};

#endif
