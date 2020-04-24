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
#include "interface/XMLInputStreamToHandler.h"
#include "interface/iXMLHandler.h"
#include "common/interface/iXMLInputStream.h"

bool
XMLInputStreamToHandler(
        iXMLInputStream& is,
        const char *const* enclosingElementAttributes,
        iXMLHandler& handler
        )
{
    handler.init(enclosingElementAttributes);
    iXMLHandler* currentHandler = &handler;
    tSigned32 nestedLevel = 0;
    while(1)
    {
        const char* nextElement = is.peekNextElement();
        if(!is.good())
        {
            return false;
        }
        if(nextElement)
        {
            const char *const* attributes = is.openNextElement();
            if(!is.good())
            {
                return false;
            }
            currentHandler = currentHandler->start(nextElement, attributes);
            nestedLevel++;
        }
        else if(nestedLevel)
        {
            currentHandler = currentHandler->end();
            is.closeElement();
            nestedLevel--;
        }
        else
        {
            currentHandler->completed();
            return is.good();
        }
    }
}
