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
    else if (str == "i8")
        return Type::eInt8;
    else if (str == "i16")
        return Type::eInt16;
    else if (str == "i32")
        return Type::eInt32;
    else if (str == "i64")
        return Type::eInt64;
    else if (str == "i128")
        return Type::eInt128;
    else if (str == "u8")
        return Type::eUInt8;
    else if (str == "u16")
        return Type::eUInt16;
    else if (str == "u32")
        return Type::eUInt32;
    else if (str == "u64")
        return Type::eUInt64;
    else if (str == "u128")
        return Type::eUInt128;
    else if (str == "f32")
        return Type::eFloat32;
    else if (str == "f64")
        return Type::eFloat64;
    else if (str == "bool")
        return Type::eBool;
    else if (str == "array")
        return Type::eArray;
    else
        return Type::eUnd;
}

int widthFromType(Type t)
{
    switch (t)
    {
        case Type::eInt8:
        case Type::eUInt8:
            return 8;
        case Type::eInt16:
        case Type::eUInt16:
            return 16;
        case Type::eInt32:
        case Type::eUInt32:
        case Type::eFloat32:
            return 32;
        case Type::eInt64:
        case Type::eUInt64:
        case Type::eFloat64:
            return 64;
        case Type::eInt128:
        case Type::eUInt128:
            return 128;
        case Type::eBool:
            return 1;
        default:
            return -1;
    }
}

bool isNumeric(Type t)
{
    switch (t)
    {
        case Type::eInt8:
        case Type::eInt16:
        case Type::eInt32:
        case Type::eInt64:
        case Type::eInt128:
        case Type::eUInt8:
        case Type::eUInt16:
        case Type::eUInt32:
        case Type::eUInt64:
        case Type::eUInt128:
        case Type::eFloat32:
        case Type::eFloat64:
        case Type::eBool:
            return true;
        default:
            return false;
    }
}

bool isSigned(Type t)
{
    switch (t)
    {
        case Type::eInt8:
        case Type::eInt16:
        case Type::eInt32:
        case Type::eInt64:
        case Type::eInt128:
        case Type::eFloat32:
        case Type::eFloat64:
            return true;
        case Type::eUInt8:
        case Type::eUInt16:
        case Type::eUInt32:
        case Type::eUInt64:
        case Type::eUInt128:
        case Type::eBool:
        default:
            return false;
    }
}

bool typeCompat(Type a, Type b)
{
    // check if type 'a' is compatible with type 'b' for weak-typing
    // right now, only numeric types are compatible with eachother
    // if (isNumeric(a) && isNumeric(b))
    //     return true;
    // return false;
    return a == b;
}

bool isArrayType(Type t)
{
    return (t == Type::eArray);
}

Type getArrayType(Type eleType)
{
    // switch (eleType)
    // {
    //     default:
    //         return Type::eUnd;
    // }
    return Type::eUnd;
}
