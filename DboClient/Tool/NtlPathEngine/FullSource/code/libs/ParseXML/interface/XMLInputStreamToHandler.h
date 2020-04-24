//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

class iXMLInputStream;
class iXMLHandler;

bool
XMLInputStreamToHandler(
        iXMLInputStream& is,
        const char *const* enclosingElementAttributes,
        iXMLHandler& handler
        );

