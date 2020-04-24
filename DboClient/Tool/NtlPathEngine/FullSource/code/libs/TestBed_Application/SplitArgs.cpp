//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "SplitArgs.h"
#include <string.h>

static bool
IsWhiteSpace(const char c)
{
    return c == ' ' || c == '\t';
}

void
SplitArgs(char* str, char**& argV, int& argC)
{
    char* ptr;
    tSigned32 argLen;
    int i;

    ptr = str;
    while(IsWhiteSpace(*ptr))
    {
        ptr++;
    }
    str = ptr;

    argC = 0;
    while(*ptr)
    {
        argC++;
        while(*ptr && !IsWhiteSpace(*ptr))
        {
            ptr++;
        }
        while(IsWhiteSpace(*ptr))
        {
            ptr++;
        }
    }

    if(!argC)
    {
        return;
    }

    argV = new char*[argC];

    ptr = str;
    for(i = 0; i < argC; i++)
    {
        argLen = 1;
        while(ptr[argLen] && !IsWhiteSpace(ptr[argLen]))
        {
            argLen++;
        }
        argV[i] = new char [argLen + 1];
        strncpy(argV[i], ptr, argLen);
        argV[i][argLen] = 0;
        ptr += argLen;
        while(IsWhiteSpace(*ptr))
        {
            ptr++;
        }
    }
}
void
DeleteArgs(char** argV, int argC)
{
    int i;
    if(argC == 0)
    {
        return;
    }
    for(i = 0; i < argC; i++)
        delete [] argV[i];
    delete [] argV;
}
