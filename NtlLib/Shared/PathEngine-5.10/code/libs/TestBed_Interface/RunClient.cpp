//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/RunClient.h"
#include "interface/Singletons_TestBed.h"
#include "libs/PathEngine_Interface/interface/cPathEngine.h"
#include "cTestBed.h"
#include <windows.h>
#include "common/STL/string.h"
#include "common/STL/sstream.h"
#include "platform_common/PointMultiplication.h"

static std::string
ErrorString(DWORD errorCode)
{
    LPTSTR ptr;
    if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER + FORMAT_MESSAGE_FROM_SYSTEM, 0, errorCode, 0, (LPTSTR)&ptr, 0, 0))
    {
      // Format Message succeeded
        LPTSTR crlfPtr = strchr(ptr,'\r');
        if(crlfPtr)
        {
            *crlfPtr = 0;
        }
        std::string result(ptr);
        LocalFree(ptr);
        return result;
	}
    return "Unknown error code";
}

static FARPROC
DoGetProcAddress(HINSTANCE hi, const char* functionname)
{
    std::string withunderscore="_";
    withunderscore+=functionname;
    SetLastError(0);
    FARPROC procaddress;
    procaddress=GetProcAddress(hi,withunderscore.c_str());
    if(!procaddress)
    {
        procaddress=GetProcAddress(hi,functionname);
    }
    return procaddress;
}

typedef void (__stdcall *runclientroutine)(iPathEngine *pathengine, iTestBed *interface_to_this);
void
RunClient(const char* name)
{
    iPathEngine* pathengine = &nSingletons::pathEngine();
    iTestBed* interface_to_this = &nSingletons::testBed();

    HINSTANCE hi;
    FARPROC procAddress;
    hi = LoadLibrary(name); 
    if(!hi)
    {
        std::string errorString = ErrorString(GetLastError());
        std::ostringstream s;
        s << "The Testbed could load the specified client application dll.\n";
        s << "This could be because the specified dll is not in the dll search path, or because it fails to load for some other reason.\n\n";
        s << "Try putting the Testbed application dll in the same directory as the Testbed executable.\n\n";
        s << "LoadLibrary() was called for:\n\"";
        s << name;
        s << "\"\n\n";
        s << "And failed, with the following error:\n\"";
        s << errorString;
        s << "\"";
        MessageBox(0, s.str().c_str(), "PathEngine Testbed: failed to load client application", MB_OK );
        exit(1);
    }
    procAddress = DoGetProcAddress(hi, "TestApplicationEntryPoint@8");
    if(!procAddress)
    {
        std::string errorString = ErrorString(GetLastError());
        std::ostringstream s;
        s << "The Testbed could not obtain obtain an entry point within the specified client application dll.\n";
        s << "Perhaps the specified dll is not a testbed application, or the entrypoint is setup incorrectly.\n\n";
        s << "Dll loaded:\n\"";
        s << name;
        s << "\"\n\n";
        s << "DoGetProcAddress() was called for:\n\"";
        s << "TestApplicationEntryPoint@8";
        s << "\"\n\n";
        s << "And failed, with the following error:\n\"";
        s << errorString;
        s << "\"";
        MessageBox(0, s.str().c_str(), "PathEngine Testbed: failed to start client application", MB_OK );
        exit(1);
    }
    runclientroutine rcr;
    rcr = (runclientroutine)procAddress; 
    rcr(pathengine, interface_to_this);
}
