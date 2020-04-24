//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/LoadAnchorsAndShapesSet.h"
#include "interface/AnchorsAndPinnedShapes.h"
#include "libs/ParseXML/interface/XMLInputStreamToHandler.h"
#include "common/interface/iXMLInputStream.h"
#include "i_pathengine.h"
#include "common/CLib/string.h"

cAnchorsAndShapesSetLoader::~cAnchorsAndShapesSetLoader()
{
    tSigned32 i;
    for(i = 0; i != SizeL(_result); ++i)
    {
        delete _result[i];
    }
}

iXMLHandler*
cAnchorsAndShapesSetLoader::element(const char* el, const char *const* attr)
{
    if(strcmp(el, "anchorsAndPinnedShapes") == 0)
    {
        cAnchorsAndPinnedShapes* m = new cAnchorsAndPinnedShapes;
        _result.push_back(m);
        return m;
    }
    return 0;
}

