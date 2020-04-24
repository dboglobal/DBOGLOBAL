//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/ToAndFromByteBuffer.h"
#include "common/STL/string.h"
#include "common/STL/sstream.h"
#include <time.h>

inline std::string
FormatTimeStamp(const char* timeStamp)
{
    tUnsigned32 afterPoint = UnsignedFromByteBuffer(timeStamp, 2);
    tUnsigned32 seconds = UnsignedFromByteBuffer(timeStamp + 2, 4);

    time_t time = static_cast<tSigned32>(seconds); //.. fails in 2038
    struct tm* t = localtime(&time); //.. might not be threadsafe on some platforms

    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y %A %d %H:%M:%S", t);

    float milliseconds = static_cast<float>(afterPoint) / 65.536f;

    std::ostringstream os;
    os << buffer << " ";
    os.fill('0');
    os.width(3);
    os << static_cast<tSigned32>(milliseconds);
    return os.str();
}
