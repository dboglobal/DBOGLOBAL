//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "InformationChannels3DLoader.h"
#include "common/interface/Attributes.h"
#include "common/interface/Assert.h"
#include "interface/cMesh.h"
#include "common/CLib/stdlib.h"

void
cInformationChannels3DLoader::processFaceAttributes(tSigned32 faceIndex, const char *const* attributes)
{
    tSigned32 value;
    const char* valueString;

    value = 0;
    valueString = GetValueForAttribute(attributes, "surfaceType");
    if(valueString == 0)
    {
        valueString = GetValueForAttribute(attributes, "surfacetype");
        if(valueString)
        {
            Error("Warning", "Legacy 'surfacetype' face attribute string encountered. This should be 'surfaceType' (capitalisation). "
                "The attribute will be ignored.");
            valueString = 0;
        }
    }
    if(valueString)
    {
        value = ValueAsLong(valueString);
    }
    if(value)
    {
        if(_surfaceTypes == 0)
        {
        // create the channel
            _surfaceTypes = new std::vector<tSigned32>(_facesIn3DMesh, 0);
        }
        (*_surfaceTypes)[faceIndex] = value;
    }

    value = -1;
    valueString = GetValueForAttribute(attributes, "sectionID");
    if(valueString == 0)
    {
        valueString = GetValueForAttribute(attributes, "terrainlayer");
        if(valueString)
        {
            Error("Warning", "Legacy 'terrainlayer' face attribute string encountered. This should be 'sectionID'.  "
                "The attribute will be ignored.");
            valueString = 0;
        }
    }
    if(valueString)
    {
        value = ValueAsLong(valueString);
    }
    if(value != -1)
    {
        if(_sectionIDs == 0)
        {
        // create the channel
            _sectionIDs = new std::vector<tSigned32>(_facesIn3DMesh, -1);
        }
        (*_sectionIDs)[faceIndex] = value;
    }

    value = 0;
    valueString = GetValueForAttribute(attributes, "userData");
    if(valueString)
    {
        value = ValueAsLong(valueString);
    }
    if(value)
    {
        if(_userData == 0)
        {
        // create the channel
            _userData = new std::vector<tSigned32>(_facesIn3DMesh, 0);
        }
        (*_userData)[faceIndex] = value;
    }
}

void
cInformationChannels3DLoader::init(const char *const* attr)
{
    assertD(_surfaceTypes == 0);
    delete _surfaceTypes;
    _surfaceTypes = 0;
    assertD(_userData == 0);
    delete _userData;
    _userData = 0;
    assertD(_sectionIDs == 0);
    delete _sectionIDs;
    _sectionIDs = 0;
    _currentIndex = 0;
}

iXMLHandler*
cInformationChannels3DLoader::element(const char* el, const char *const* attributes)
{
    if(getDepth() == 0)
    {
        if(strcmp(el, "mesh3D") == 0)
        {
            return this;
        }
        return 0;
    }
    if(getDepth() == 1)
    {
        if(strcmp(el, "tris") == 0)
        {
            return this;
        }
        return 0;
    }
    if(strcmp(el, "tri") == 0)
    {
        processFaceAttributes(_currentIndex++, attributes);
    }
    return 0;
}
