#ifndef TYPES_H
#define TYPES_H

#include <sstream>
#include <vector>
#include <map>

enum Type
{
    eVoid,
    eInt,
    eArray,
    eUnd,
    NUM_TYPES,
};

std::string typeToString(Type t);
Type typeFromString(const std::string& str);

static std::vector<std::string> typeStrings = {
    "eVoid",
    "eInt",
    "eArray",
    "eUnd",
};

static std::map<Type, std::string> typeValues = {
    {Type::eVoid, "void"},
    {Type::eInt, "int"},
    {Type::eArray, "array"},
    {Type::eUnd, "undefined"},
};

#endif
