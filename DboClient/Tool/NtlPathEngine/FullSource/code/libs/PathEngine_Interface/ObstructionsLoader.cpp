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
#include "ObstructionsLoader.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "platform_common/CoordinateRanges.h"
#include "common/interface/Attributes.h"
#include "common/interface/Assert.h"
#include "common/CLib/stdlib.h"
#include "common/STL/vector.h"

// TODO organise conversions to and from strings better
static void
PositionFromString(const char* str, cPosition& position)
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
cObstructionsLoader::element(const char* el, const char *const* attributes)
{
    if(strcmp(el, "obstacle") != 0)
    {
        return 0;
    }

    const char* position = GetValueForAttribute(attributes, "position");
    assertR(position);
    cPosition p;
    PositionFromString(position, p);
    _loadedPositions.push_back(p);
//    if(!_mesh.positionIsValid(p))
//    {
//        Error("NonFatal", "The position for an obstacle being loaded is not valid. The obstacle will be skipped.");
//        return 0;
//    }

    const char* id = GetValueForAttribute(attributes, "id");
    if(id)
    {
        _ids.push_back(id);
    }
    else
    {
        _ids.push_back(std::string());
    }

    const char* coordsString = GetValueForAttribute(attributes, "vertices");
    std::vector<tSigned32> coordsVector;
    while(1)
    {
        char* nextPtr;
        coordsVector.push_back(strtol(coordsString, &nextPtr, 10));
        assertD(*nextPtr == ',');
        coordsString = nextPtr + 1;
        coordsVector.push_back(strtol(coordsString, &nextPtr, 10));
        if(*nextPtr == 0)
        {
            break;
        }
        assertD(*nextPtr == ',');
        coordsString = nextPtr + 1;
    }
    tSigned32 range = SHAPE_RANGE;
    tSigned32 i;
    for(i = 0; i != SizeL(coordsVector); ++i)
    {
        if(coordsVector[i] < -SHAPE_RANGE || coordsVector[i] > SHAPE_RANGE)
        {
            range = WORLD_RANGE * 2;
            break;
        }
    }
    tSigned32 points = SizeL(coordsVector) / 2;
    tPoint* pointArray = new tPoint[points];
    for(i = 0; i != points; ++i)
    {
        pointArray[i] = tPoint(coordsVector[i * 2], coordsVector[i * 2 + 1], range);
    }
    _shapes.push_back(cConvexPoly(points, pointArray));

    cObstacleInfo info;
    info._costToTraverse = ValueAsPersistentFloatWithDefault(attributes, "costToTraverse", -1.f);
    {
        tSigned32 x = ValueAsLongWithDefault(attributes, "costDirectionX", 0);
        tSigned32 y = ValueAsLongWithDefault(attributes, "costDirectionY", 0);
        info._directionForCostToTraverse = tPoint(x, y, DIRECTION_VECTOR_RANGE);
    }

    _infos.push_back(info);

    return 0;
}
