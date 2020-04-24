//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "FormatVersionString.h"
#include "i_pathengine.h"
#include "common/CLib/stdlib.h"

std::string
FormatVersionString(iPathEngine& pathEngine)
{
    tSigned32 major, minor, internal;
    pathEngine.getReleaseNumbers(major, minor, internal);
    char formattedVersionNumber[50];
    if(internal != 0)
    {
        sprintf(formattedVersionNumber, "%i.%02i.%02i", major, minor, internal);
    }
    else
    {
        sprintf(formattedVersionNumber, "%i.%02i", major, minor);
    }
    return std::string(formattedVersionNumber);
}
