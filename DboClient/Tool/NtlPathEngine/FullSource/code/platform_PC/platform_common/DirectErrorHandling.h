//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

// to be included in just one .obj

#include "common/STL/sstream.h"
#include "common/interface/Error.h"
#include "platform_common/Process.h"
#include <windows.h>

bool&
FullAssertsOn()
{
    static bool value = false;
    return value;
}

void
Error(const char* errorType, const char* errorString, const char *const* attributes)
{
    std::ostringstream message;
    message << "Error type: " << errorType << std::endl;
    message << "Error string: " << errorString << std::endl;
    if(attributes && *attributes)
    {
        message << "Error attributes:" << std::endl;
        do
        {
            message << " " << attributes[0] << " = " << attributes[1] << std::endl;
            attributes += 2;
        }
        while(*attributes);
    }
    MessageBox(0, message.str().c_str(), "Error", MB_OK);
    Platform_DebugBreak();
    if(strcmp(errorType, "Fatal") == 0)
    {
        Platform_Abort();
    }
}
bool
HandleAssertion(const char *file, tSigned32 line, const char *comment)
{
    std::ostringstream oss;
    oss << file << '(' << line << "): Assert(" << comment << ").";
    Error("Assertion", oss.str().c_str(), 0);
    return true;
}
bool
ReportInterfaceCheckFailure(const char *file, tSigned32 line, const char *comment)
{
    std::ostringstream oss;
    oss << file << '(' << line << "): Interface check failed <" << comment << ">.";
    Error("Assertion", oss.str().c_str(), 0);
    return true;
}
