//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_XML_OUTPUT_STREAM__HELPERS_INCLUDED
#define I_XML_OUTPUT_STREAM__HELPERS_INCLUDED

#include "common/interface/iXMLInputStream.h"
#include "common/CLib/string.h"

inline bool
SkipElementsUntil(iXMLInputStream& is, const char* tag)
{
    const char* nextElement = is.peekNextElement();
    while(nextElement && strcmp(nextElement, tag))
    {
        is.skipNextElement();
        nextElement = is.peekNextElement();
    }
    return nextElement != 0;
}

#endif
