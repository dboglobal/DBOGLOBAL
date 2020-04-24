//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON__BASIC_ERROR_HANDLER
#define PLATFORM_COMMON__BASIC_ERROR_HANDLER

#include "i_pathengine.h"
#include <windows.h>
#include <stdio.h>
#include <string>

class cBasicErrorHandler : public iErrorHandler
{
    cBasicErrorHandler() {}

public:

    eAction handle(const char* error_type, const char* errorString, const char *const* attributes)
    {
        std::string displayString(errorString);
        if(attributes && *attributes)
        {
            displayString += "\n\nAttributes:\n";
            do
            {
                displayString += "attribute \"";
                displayString += *attributes;
                displayString += "\"=\"";
                attributes++;
                displayString += *attributes;
                displayString += "\"\n";
                attributes++;
            }
            while(*attributes);
        }

        char title[1000];
        sprintf(title, "Error, type: %s", error_type);

        int choice;

        if(strcmp(error_type, "Assertion") ==0 )
        {
            displayString += "\nSelect 'Yes' to continue, and continue to report this assertion in future,\n"
                "'No' to continue, and ignore this assertion in future,\n"
                " or 'Cancel' to trigger a debug break.";
            choice = MessageBox(0, displayString.c_str(), title, MB_YESNOCANCEL);
            if(choice == IDNO)
            {
                return CONTINUE_IGNORE_IN_FUTURE;
            }
        }
        else
        {
            displayString += "\n\nSelect 'Ok' to continue execution,\n"
                " or 'Cancel' to trigger a debug break.";
            choice = MessageBox(0, displayString.c_str(), title, MB_OKCANCEL);
        }

        if(choice == IDCANCEL)
        {
            DebugBreak();
            return CONTINUE;
        }
        return CONTINUE;
    }
    static cBasicErrorHandler& refInstance()
    {
        static cBasicErrorHandler theInstance;
        return theInstance;
    }
};

#endif
