//**********************************************************************
//
// Copyright (c) 2005
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON_TOANDFROMBYTEBUFFER_INCLUDED
#define PLATFORM_COMMON_TOANDFROMBYTEBUFFER_INCLUDED

#include "common/interface/Assert.h"

inline void
SignedToByteBuffer(tSigned32 value, tSigned32 tokenSize, char* bufferPtr)
{
    switch(tokenSize)
    {
    default:
        invalid();
    case 1:
        {
            signed char asChar = static_cast<signed char>(value);
            assertD(value == asChar);
            *(signed char*)bufferPtr = asChar;
            break;
        }
    case 2:
        {
            short asShort = static_cast<short>(value);
            assertD(value == asShort);
            *(short*)bufferPtr = asShort;
            break;
        }
    case 4:
        {
            *(tSigned32*)bufferPtr = value;
            break;
        }
    }
}
inline void
UnsignedToByteBuffer(tUnsigned32 value, tSigned32 tokenSize, char* bufferPtr)
{
    switch(tokenSize)
    {
    default:
        invalid();
    case 1:
        {
            unsigned char asChar = static_cast<unsigned char>(value);
            assertD(value == asChar);
            *(signed char*)bufferPtr = asChar;
            break;
        }
    case 2:
        {
            unsigned short asShort = static_cast<unsigned short>(value);
            assertD(value == asShort);
            *(unsigned short*)bufferPtr = asShort;
            break;
        }
    case 4:
        {
            *(tUnsigned32*)bufferPtr = value;
            break;
        }
    }
}

inline tSigned32
SignedFromByteBuffer(const char* bufferPtr, tSigned32 tokenSize)
{
    switch(tokenSize)
    {
    default:
        invalid();
    case 1:
        return *(const signed char*)bufferPtr;
    case 2:
        return *(const short*)bufferPtr;
    case 4:
        return *(const tSigned32*)bufferPtr;
    }
}
inline tUnsigned32
UnsignedFromByteBuffer(const char* bufferPtr, tSigned32 tokenSize)
{
    switch(tokenSize)
    {
    default:
        invalid();
    case 1:
        return *(const unsigned char*)bufferPtr;
    case 2:
        return *(const unsigned short*)bufferPtr;
    case 4:
        return *(const tUnsigned32*)bufferPtr;
    }
}

inline void
FloatToByteBuffer(float value, char* bufferPtr)
{
    UnsignedToByteBuffer(*(reinterpret_cast<tUnsigned32*>(&value)), 4, bufferPtr);
}
inline float
FloatFromByteBuffer(const char* bufferPtr)
{
    tUnsigned32 ul = UnsignedFromByteBuffer(bufferPtr, 4);
    return *(reinterpret_cast<float*>(&ul));
}
static const tSigned32 FLOAT_BYTE_SIZE = 4;

#endif
