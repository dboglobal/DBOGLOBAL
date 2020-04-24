//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/GeometryPersistence.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "platform_common/PointMultiplication.h"
#include "platform_common/CoordinateRanges.h"
#include "common/CLib/stdlib.h"
#include "common/CLib/string.h"
#include "common/STL/sstream.h"

void PointFromString(const char* str, tPoint& result)
{
    char *ptr;
    tSigned32 x,y;
    x=strtol(str,&ptr,10);
    assertD(*ptr==',');
    str=ptr+1;
    y=strtol(str,&ptr,10);
    assertD(*ptr==0);
    result=tPoint(x,y,WORLD_RANGE);
}
void
IntersectionFromString(const char* str, tIntersection& result)
{
    char *ptr;
    tSigned32 x, y;
    tPoint start, end;
    tPoint::tDistance n, d;

    x=strtol(str,&ptr,10);
    assertD(*ptr==',');
    str=ptr+1;

    y=strtol(str,&ptr,10);
    assertD(*ptr==',');
    str=ptr+1;

    start = tPoint(x, y, WORLD_RANGE);

    x=strtol(str,&ptr,10);
    assertD(*ptr==',');
    str=ptr+1;

    y=strtol(str,&ptr,10);
    assertD(*ptr==':');
    str=ptr+1;

    end = tPoint(x, y, WORLD_RANGE);

//...... need string to large integer conversions

    n = tPoint::tDistance(strtol(str,&ptr,10));
    assertD(*ptr=='/');
    str=ptr+1;

    d = tPoint::tDistance(strtol(str,&ptr,10));
    assertD(*ptr==0);

    result = tIntersection(tLine(start, end), n, d);
}
void Point3FromString(const char* str, tPoint3& point)
{
    char *ptr;
    tSigned32 x,y,z;
    x=strtol(str,&ptr,10);
    assertD(*ptr==',');
    str=ptr+1;
    y=strtol(str,&ptr,10);
    assertD(*ptr==',');
    str=ptr+1;
    z=strtol(str,&ptr,10);
    assertD(*ptr==0);
    point=tPoint3(x,y,z,WORLD_RANGE);
}
void LineFromString(const char* str, tLine& result)
{
    char *ptr;
    tPoint start,end;
    tSigned32 x,y;
    x=strtol(str,&ptr,10);
    assertD(*ptr==',');
    str=ptr+1;
    y=strtol(str,&ptr,10);
    start=tPoint(x,y,WORLD_RANGE);
    assertD(*ptr==',');
    str=ptr+1;
    x=strtol(str,&ptr,10);
    assertD(*ptr==',');
    str=ptr+1;
    y=strtol(str,&ptr,10);
    end=tPoint(x,y,WORLD_RANGE);
    assertD(*ptr==0);
    result=tLine(start,end);
}

void
VerticesFromString(const char* str, std::vector<tSigned32>& result)
{
    assertD(str);
    while(1)
    {
        char* nextPtr;
        result.push_back(strtol(str, &nextPtr, 10));
        assertD(*nextPtr == ',');
        str = nextPtr + 1;
        result.push_back(strtol(str, &nextPtr, 10));
        if(*nextPtr == 0)
        {
            break;
        }
        assertD(*nextPtr == ',');
        str = nextPtr + 1;
    }
}

std::string
VerticesToString(const std::vector<tSigned32>& vertices)
{
    std::ostringstream oss;
    tSigned32 i;
    for(i = 0; i != SizeL(vertices); ++i)
    {
        if(i)
        {
            oss << ',';
        }
        oss << vertices[i];
    }
    return oss.str();

}
