//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/CLib/string.h"

namespace nStringFormatting
{

inline tSigned32
bufferSize_Long()
{
    return 12;
}
inline char*
format_Long(tSigned32 value, char* buffer)
{
    if(value == -2147483647L - 1)
    {
        char* negativeMax = "-2147483648";
        tSigned32 i;
        for(i = 0; i != 12; ++i)
        {
            buffer[i] = negativeMax[i];
        }
        return buffer;
    }
    tSigned32 magnitude = value;
    if(magnitude < 0)
    {
        magnitude = -magnitude;
    }
    char* ptr = buffer + 11;
    *ptr-- = 0;
    while(magnitude >= 10)
    {
        char digit = static_cast<char>(magnitude % 10);
        *ptr-- = '0' + digit;
        magnitude /= 10;
    }
    *ptr = '0' + static_cast<char>(magnitude);
    if(value < 0)
    {
        ptr--;
        *ptr = '-';
    }
    return ptr;
}

}
