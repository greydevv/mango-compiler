#ifndef TYPES_H
#define TYPES_H

#include <sstream>
#include <vector>
#include <map>

enum Type
{
    eVoid,
    eFunc,
    eInt8,
    eInt16,
    eInt32,
    eInt64,
    eInt128,
    eUInt8,
    eUInt16,
    eUInt32,
    eUInt64,
    eUInt128,
    eFloat32,
    eFloat64,
    eBool,
    eArray,
    eUnd,
    eNot,
    NUM_TYPES,
};

std::string typeToString(Type t);
Type typeFromString(const std::string& str);
int widthFromType(Type t);
bool isNumeric(Type t);
bool isSigned(Type t);
bool typeCompat(Type a, Type b);
bool isArrayType(Type t);

static std::vector<std::string> typeStrings = {
    "eVoid",
    "eFunc",
    "eInt8",
    "eInt16",
    "eInt32",
    "eInt64",
    "eInt128",
    "eUInt8",
    "eUInt16",
    "eUInt32",
    "eUInt64",
    "eUInt128",
    "eFloat32",
    "eFloat64",
    "eBool",
    "eArray",
    "eUnd",
    "eNot",
};

static std::map<Type, std::string> typeValues = {
    {Type::eVoid, "void"},
    {Type::eFunc, "function"},
    {Type::eInt8, "i8"},
    {Type::eInt16, "i16"},
    {Type::eInt32, "i32"},
    {Type::eInt64, "i64"},
    {Type::eInt128, "i128"},
    {Type::eUInt8, "u8"},
    {Type::eUInt16, "u16"},
    {Type::eUInt32, "u32"},
    {Type::eUInt64, "u64"},
    {Type::eUInt128, "u128"},
    {Type::eFloat32, "f32"},
    {Type::eFloat64, "f64"},
    {Type::eBool, "bool"},
    {Type::eArray, "array"},
    {Type::eUnd, "undefined"},
    {Type::eNot, "no_type"},
};

#endif
