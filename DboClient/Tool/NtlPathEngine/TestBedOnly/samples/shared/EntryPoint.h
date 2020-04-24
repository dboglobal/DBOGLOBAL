
#include "i_pathengine.h"
#include "i_testbed.h"
void demo(iPathEngine *pathengine, iTestBed *testbed);
extern "C"
{
    __declspec(dllexport) void __stdcall TestApplicationEntryPoint(
                                            iPathEngine *pathengine, iTestBed *testbed)
    {             
    // check if interfaces are compatible with the headers used for compilation
        if(testbed->getInterfaceMajorVersion()!=TESTBED_INTERFACE_MAJOR_VERSION
            ||
            testbed->getInterfaceMinorVersion()<TESTBED_INTERFACE_MINOR_VERSION)
        {
            testbed->reportError("Fatal","Checking version in entrypoint.h: testbed version is incompatible with headers used for compilation.");
            return;
        }
        if(pathengine->getInterfaceMajorVersion()!=PATHENGINE_INTERFACE_MAJOR_VERSION
            ||
            pathengine->getInterfaceMinorVersion()<PATHENGINE_INTERFACE_MINOR_VERSION)
        {
            testbed->reportError("Fatal","Checking version in entrypoint.h: pathengine version is incompatible with headers used for compilation.");
            return;
        }
        demo(pathengine,testbed);
    }
}
