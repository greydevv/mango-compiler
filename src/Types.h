#ifndef TYPES_H
#define TYPES_H

#include <sstream>
#include <vector>
#include <map>

enum Type
{
    eVoid,
    eInt,
    eUnd,
    NUM_TYPES,
};

std::string typeToString(Type t);
Type typeFromString(const std::string& str);

static std::vector<std::string> typeStrings = {
    "eVoid",
    "eInt",
    "eUnd",
};

static std::map<Type, std::string> typeValues = {
    {Type::eVoid, "void"},
    {Type::eInt, "int"},
    {Type::eUnd, "undefined"},
};

#endif
