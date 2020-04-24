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
#include "SurfaceTypeCosts.h"
#include "libs/MeshPair/interface/Mesh3DCostCalculator.h"
#include "libs/MeshPair/interface/Mesh3DCorners.h"
#include "common/interface/iOutputStream_Operators.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/interface/iXMLInputStream.h"
#include "common/interface/Attributes.h"
#include "platform_common/FloatPersistence.h"
#include "platform_common/CoordinateRanges.h"

static tSigned32
FindTopValue(const std::vector<tSigned32>& v)
{
    tSigned32 result = -1;
    for(tSigned32 i = 0; i != SizeL(v); ++i)
    {
        if(v[i] > result)
        {
            result = v[i];
        }
    }
    return result;
}


cSurfaceTypeCosts::cSurfaceTypeCosts(const std::vector<tSigned32>* surfaceTypesVector, tSigned32 topSurfaceType) :
 _surfaceTypesVector(surfaceTypesVector),
 _topSurfaceType(topSurfaceType),
 _costs(_topSurfaceType + 1, 0.f),
 _costDirections(_topSurfaceType + 1, tPoint(0,0,0)),
 _numberSet(0)
{
}

cSurfaceTypeCosts::cSurfaceTypeCosts(const std::vector<tSigned32>* surfaceTypesVector) :
 _surfaceTypesVector(surfaceTypesVector),
 _topSurfaceType(FindTopValue(*surfaceTypesVector)),
 _costs(_topSurfaceType + 1, 0.f),
 _costDirections(_topSurfaceType + 1, tPoint(0,0,0)),
 _numberSet(0)
{
}

void
cSurfaceTypeCosts::save(iXMLOutputStream& os) const
{
    tSigned32 i;
    for(i = 0; i < SizeL(_costs); ++i)
    {
        if(_costs[i] == 0.f && _costDirections[i] == tPoint(0,0,0))
        {
            continue;
        }
        os.openElement("entry");
        os.addAttribute("surfaceType", i);
        if(_costs[i] != 0.f)
        {
            std::string persistent = FloatToPersistent(_costs[i]);
            os.addAttribute("cost", persistent.c_str());
        }
        if(_costDirections[i].getX() != 0)
        {
            os.addAttribute("costDirectionX", _costDirections[i].getX());
        }
        if(_costDirections[i].getY() != 0)
        {
            os.addAttribute("costDirectionY", _costDirections[i].getY());
        }
        os.closeElement("entry");
    }
}

void
cSurfaceTypeCosts::setCost(tSigned32 surfaceType, float value)
{
    if(surfaceType > _topSurfaceType)
    {
        return;
    }
    if(_costs[surfaceType] == 0.f && value != 0.f)
    {
        _numberSet++;
    }
    else
    if(_costs[surfaceType] != 0.f && value == 0.f)
    {
        _numberSet--;
    }
    _costs[surfaceType] = value;
}

void
cSurfaceTypeCosts::setCostDirection(tSigned32 surfaceType, const tPoint& value)
{
    if(surfaceType > _topSurfaceType)
    {
        return;
    }
    _costDirections[surfaceType] = value;
}

void
cSurfaceTypeCosts::buildCornersAndCostCalculator(
                    tMesh_3D& mesh3D,
                    const cTranslationToOverlapped& translation,
                    iMovementCostCalculator*& calculator,
                    cCornerProvider& corners)
{
    calculator = new cMesh3DCostCalculator(
                    translation,
                    *_surfaceTypesVector,
                    _costs,
                    _costDirections);
    AddMesh3DSoftCorners(
                    mesh3D,
                    translation,
                    *_surfaceTypesVector,
                    _costs,
                    _costDirections,
                    corners);
}

void
cSurfaceTypeCosts::checksum(iOutputStream& os) const
{
    tSigned32 i;
    for(i = 0; i < SizeL(_costs); ++i)
    {
        std::string persistent = FloatToPersistent(_costs[i]);
        os << persistent;
        os << _costDirections[i].getX();
        os << _costDirections[i].getY();
    }
}


iXMLHandler*
cSurfaceTypeCostsLoader::element(const char* el, const char *const* attributes)
{
    if(strcmp(el, "entry"))
    {
        return 0;
    }
    tSigned32 surfaceType = ValueAsLong(attributes, "surfaceType");
    _target->setCost(surfaceType, ValueAsPersistentFloatWithDefault(attributes, "cost", 0.f));
    tSigned32 x, y;
    x = ValueAsLongWithDefault(attributes, "costDirectionX", 0);
    y = ValueAsLongWithDefault(attributes, "costDirectionY", 0);
    _target->setCostDirection(surfaceType, tPoint(x, y, DIRECTION_VECTOR_RANGE));
    return 0;
}

iXMLHandler*
cSurfaceTypeCostsVectorLoader::element(const char* el, const char *const* attributes)
{
    if(strcmp(el, "costs"))
    {
        return 0;
    }
    _target.push_back(new cSurfaceTypeCosts(_surfaceTypesVector));
    _targetLoader.setTarget(_target.back());
    return &_targetLoader;
}
