#ifndef TYPES_H
#define TYPES_H

#include <sstream>
#include <vector>
#include <map>

enum Type
{
    eVoid,
    eFunc,
    eInt,
    eBool,
    eArray,
    eUnd,
    eNot,
    NUM_TYPES,
};

std::string typeToString(Type t);
Type typeFromString(const std::string& str);
bool isNumeric(Type t);
bool typeCompat(Type a, Type b);
bool isArrayType(Type t);

static std::vector<std::string> typeStrings = {
    "eVoid",
    "eFunc",
    "eInt",
    "eBool",
    "eArray",
    "eUnd",
    "eNot",
};

static std::map<Type, std::string> typeValues = {
    {Type::eVoid, "void"},
    {Type::eFunc, "function"},
    {Type::eInt, "int"},
    {Type::eBool, "bool"},
    {Type::eArray, "array"},
    {Type::eUnd, "undefined"},
    {Type::eNot, "no_type"},
};

#endif
