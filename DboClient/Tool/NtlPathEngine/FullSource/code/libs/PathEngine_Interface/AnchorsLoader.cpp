//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "AnchorsLoader.h"
#include "common/interface/Attributes.h"
#include "common/interface/Assert.h"
#include "common/CLib/stdlib.h"

// TODO organise conversions to and from strings better
static void
PositionFromString(const char *str, cPosition &position)
{
    char *ptr;
    position.cell=strtol(str,&ptr,10);
    if(position.cell==-1)
    {
        assertD(*ptr==0);
        return;
    }
    assertD(*ptr==':');
    str=ptr+1;
    position.x=strtol(str,&ptr,10);
    assertD(*ptr==',');
    str=ptr+1;
    position.y=strtol(str,&ptr,10);
    assertD(*ptr==0);
}

iXMLHandler*
cAnchorsLoader::element(const char* el, const char *const* attributes)
{
    if(strcmp(el, "anchor") != 0)
    {
        return 0;
    }
    const char* id = GetValueForAttribute(attributes, "id");
    const char* position = GetValueForAttribute(attributes, "position");
    assertR(id);
    assertR(position);
    cPosition p;
    PositionFromString(position, p);

    const char* orientation = GetValueForAttribute(attributes, "orientation");
    tSigned32 orientationL = 0;
    if(orientation)
    {
        orientationL = ValueAsLong(orientation);
    }
    _infos.push_back(cAnchorInfo(id, p, orientationL));
    return 0;
}
