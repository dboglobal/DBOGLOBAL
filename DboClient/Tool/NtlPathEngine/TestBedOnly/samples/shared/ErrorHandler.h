
#ifndef ERROR_HANDLER_INCLUDED
#define ERROR_HANDLER_INCLUDED

#include "i_pathengine.h"
#include "MessageBox.h"

class cErrorHandler : public iErrorHandler
{
    cErrorHandler(){}
public:
    eAction handle(const char* error_type, const char* error_string, const char *const* attributes)
    {
        ShowMessageBox(error_type, error_string);
        return CONTINUE;
    }
    static cErrorHandler& refInstance()
    {
        static cErrorHandler the_instance;
        return the_instance;
    }
};

#endif
