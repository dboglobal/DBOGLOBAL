//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "common/interface/Error.h"
#include "Logger.h"
#include "GetWrapperPointerAccessFunction.h"
#include "WrapperManagement.h"
#include "i_pathengine.h"
#include "common/STL/sstream.h"
#include "common/LoggingHooks_Stub.h"
#include <windows.h>

void PathEngine_DebugBreak()
{
    DebugBreak();
}
void PathEngine_Abort()
{
    _exit(1);
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
    if(strcmp(errorType, "Fatal") == 0)
    {
        exit(1);
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
bool&
FullAssertsOn()
{
    static bool value = false;
    return value;
}

bool ReportInterfaceCheckFailure(const char *file, tSigned32 line, const char *comment)
{
    std::ostringstream oss;
    oss << file << '(' << line << "): Interface check failed <" << comment << ">.";
    Error("Assertion", oss.str().c_str(), 0);
    return true;
}

typedef iPathEngine* (__stdcall* tGetInterfaceFunction)(iErrorHandler*);

iPathEngine*
LoadAndWrap(const char* fileName1, const char* fileName2, iErrorHandler* handler)
{
    HINSTANCE pathengine_hinstance;
    pathengine_hinstance = LoadLibrary(fileName1);
    if(!pathengine_hinstance)
    {
        pathengine_hinstance = LoadLibrary(fileName2);
        if(!pathengine_hinstance)
        {
            Error("Fatal", "Query Logging Wrapper: Failed to load base PathEngine dll.");
            return 0;
        }
    }

    FARPROC procaddress;
    SetLastError(0);
    procaddress = GetProcAddress(pathengine_hinstance, (LPCSTR)1);
    if(!procaddress)
    {
        Error("Fatal", "Query Logging Wrapper: GetProcAddress() failed to obtain entry point in wrapped dll.");
        return 0;
    }

    tGetInterfaceFunction getInterfaceFunction = (tGetInterfaceFunction)procaddress;
    iPathEngine* pathEngine = getInterfaceFunction(handler);

    if(pathEngine->getInterfaceMajorVersion() != PATHENGINE_INTERFACE_MAJOR_VERSION
        ||
        pathEngine->getInterfaceMinorVersion() > PATHENGINE_INTERFACE_MINOR_VERSION)
    {
        Error("NonFatal", "Query Logging Wrapper: Target dll interface version is not compatible, falling back to pass-through mode.");
        return pathEngine;
    }

    if(!GetWrapperPointerAccessFunction(pathengine_hinstance))
    {
        Error("NonFatal", "Query Logging Wrapper: GetProcAddress() failed for wrapper pointer access hooks."
            "  Note that only PathEngine releases 4_65 and above support the query logging wrapper."
            "  Falling back to pass-through mode."
            );
        return pathEngine;
    }

    return WrapSingleton(pathEngine);
}

iPathEngine* gPathEngine = 0;

extern "C"
{
    iPathEngine* __stdcall DLLExport_GetIPathEngine(iErrorHandler* handler)
    {
        if(!gPathEngine)
        {
            gLogger.initialise();
            gPathEngine = LoadAndWrap("PathEngine_Renamed", "PathEngine", handler);
        }
        return gPathEngine;
    }
    iPathEngine* __stdcall DllExport_GetIPathEngine(iErrorHandler* handler)
    {
        if(!gPathEngine)
        {
            gLogger.initialise();
            gPathEngine = LoadAndWrap("PathEngine_Renamed", "PathEngine", handler);
        }
        Error("Warning", "Obtaining the PathEngine DLL entrypoint by name, with GetProcAddress(hInstance, \"_DllExport_GetIPathEngine@4\") is depreciated. Use GetProcAddress(hInstance, (LPCSTR)1) instead.");
        return gPathEngine;
    }
}

BOOL APIENTRY
DllMain(HINSTANCE hi, DWORD reasonForCall, LPVOID)
{
    switch(reasonForCall)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return true;
}
