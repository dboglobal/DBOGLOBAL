//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_OUTPUT_STREAM_OPERATORS_DEFINED
#define I_OUTPUT_STREAM_OPERATORS_DEFINED

#include "common/interface/iOutputStream.h"
#include "common/CLib/string.h"
#include "common/STL/string.h"

inline iOutputStream& operator<<(iOutputStream& stream, const std::string& str)
{
    stream.put(str.c_str(), SizeL(str));
    return stream;
}
inline iOutputStream& operator<<(iOutputStream& stream, const char* c_str)
{
    stream.put(c_str, static_cast<tSigned32>(strlen(c_str)));
    return stream;
}
inline iOutputStream& operator<<(iOutputStream& stream, char value)
{
    stream.put(&value, 1);
    return stream;
}
inline iOutputStream& operator<<(iOutputStream& stream, tSigned32 value)
{
    unsigned char asBytes[4];
    asBytes[0] = static_cast<unsigned char>(value & 0xff);
    value >>= 8;
    asBytes[1] = static_cast<unsigned char>(value & 0xff);
    value >>= 8;
    asBytes[2] = static_cast<unsigned char>(value & 0xff);
    value >>= 8;
    asBytes[3] = static_cast<unsigned char>(value);
    stream.put((char*)asBytes, 4);
    return stream;
}

#endif
