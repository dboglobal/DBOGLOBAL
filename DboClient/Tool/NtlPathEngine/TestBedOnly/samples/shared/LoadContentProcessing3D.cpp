
#include "LoadContentProcessing3D.h"
#include <windows.h>
#include <stdio.h>

typedef iContentProcessing3D* (__stdcall* tGetInterfaceFunction)(iErrorHandler*);

iContentProcessing3D*
LoadContentProcessing3D(const char* fileName, iErrorHandler* handler)
{
    char buffer[500];
    DWORD errorValue;
    HINSTANCE hInstance;
    hInstance = LoadLibrary(fileName);
    if(!hInstance)
    {
        errorValue = GetLastError();
        MessageBox(NULL, fileName, "Error: failed calling LoadLibrary() for", MB_OK);
        sprintf(buffer, "%d", errorValue);
        MessageBox(NULL, buffer, "GetLastError()", MB_OK);
        return 0;
    }
    FARPROC procAddress;
    SetLastError(0);
    procAddress = GetProcAddress(hInstance, (LPCSTR)1);
    if(!procAddress)
    {
        errorValue = GetLastError();
        MessageBox(NULL, fileName, "Error: Failed to obtain PathEngine 3D content processing entrypoint in", MB_OK);
        sprintf(buffer, "%d", errorValue);
        MessageBox(NULL, buffer, "GetLastError()", MB_OK);
        return 0;
    }

    tGetInterfaceFunction getInterfaceFunction = (tGetInterfaceFunction)procAddress;
    return getInterfaceFunction(handler);
}

