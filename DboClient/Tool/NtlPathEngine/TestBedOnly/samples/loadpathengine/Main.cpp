
#include "loadpathengine.h"
#include "i_pathengine.h"
#include "test_linkage.h"
#include "MessageBox.h"
#include <windows.h>

class cErrorHandler : public iErrorHandler
{
    cErrorHandler(){}
public:
    eAction handle(const char* error_type, const char* error_string, const char *const* attributes)
    {
        ShowMessageBox(error_type,error_string);
        return CONTINUE;
    }
    static cErrorHandler& refInstance()
    {
        static cErrorHandler the_instance;
        return the_instance;
    }
};

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    iPathEngine* pathengine = LoadPathEngine("pathengine", &cErrorHandler::refInstance());
    if(!pathengine)
        return 0;

// check if interface for dll loaded is compatible with the headers used for compilation
    if(pathengine->getInterfaceMajorVersion()!=PATHENGINE_INTERFACE_MAJOR_VERSION
        ||
        pathengine->getInterfaceMinorVersion()<PATHENGINE_INTERFACE_MINOR_VERSION)
    {
        cErrorHandler::refInstance().handle("Fatal","LoadPathEngine: pathengine version is incompatible with headers used for compilation.",0);
        return 0;
    }

// use the interface..

    if(Test_Linkage(pathengine,&cErrorHandler::refInstance()))
    {
        ShowMessageBox("'loadpathengine' example project", "Test_Linkage() succeeded.");
    }
    else
    {
        ShowMessageBox("'loadpathengine' example project", "Test_Linkage() failed.");
    }

    return 0;
}
