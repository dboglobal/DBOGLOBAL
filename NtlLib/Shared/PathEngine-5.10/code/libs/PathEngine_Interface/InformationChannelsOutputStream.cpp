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
#include "InformationChannelsOutputStream.h"
#include "common/interface/Assert.h"
#include "common/CLib/string.h"

void
cInformationChannelsOutputStream::openElement(const char* element)
{
    if(_depth == 1 && strcmp(element, "mesh3D") == 0)
    {
        _inMesh3D = true;
        _currentIndex = 0;
    }
    _base.openElement(element);
    if(_depth == 3 && _inMesh3D && strcmp(element, "tri") == 0)
    {
    // add information channel attributes to this polygon where these differ from the defaults
        if(_surfaceTypes && (*_surfaceTypes)[_currentIndex] != 0)
        {
            _base.addAttribute("surfaceType", (*_surfaceTypes)[_currentIndex]);
        }
        if(_userData && (*_userData)[_currentIndex] != 0)
        {
            _base.addAttribute("userData", (*_userData)[_currentIndex]);
        }
        if(_sectionIDs && (*_sectionIDs)[_currentIndex] != -1)
        {
            _base.addAttribute("sectionID", (*_sectionIDs)[_currentIndex]);
        }
        _currentIndex++;
    }
    _depth++;
}
void
cInformationChannelsOutputStream::addAttribute(const char* attribute, const char* value)
{
    _base.addAttribute(attribute, value);
}
void
cInformationChannelsOutputStream::addAttribute(const char* attribute, tSigned32 value)
{
    _base.addAttribute(attribute, value);
}
void
cInformationChannelsOutputStream::addAttribute(const char* attribute, tUnsigned32 value)
{
    _base.addAttribute(attribute, value);
}
void
cInformationChannelsOutputStream::closeElement(const char* element)
{
    _depth--;
    if(_depth == 1)
    {
        _inMesh3D = false;
    }
    _base.closeElement(element);
}
