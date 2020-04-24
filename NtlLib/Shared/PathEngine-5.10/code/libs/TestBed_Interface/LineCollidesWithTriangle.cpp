//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "common/interface/Assert.h"
#include "LineCollidesWithTriangle.h"
#include "Side3D.h"

namespace nLineCollidesWithTriangle
{

cLinePreprocess::cLinePreprocess(const cVector3F& start, const cVector3F& end) :
 start(start),
 end(end),
 pLine(start, end)
{
}

cTriPreprocess::cTriPreprocess(const cVector3F& v1, const cVector3F& v2, const cVector3F& v3) :
 v1(v1),
 v2(v2),
 v3(v3),
 origin(v1),
 edge1(v1, v2),
 edge2(v2, v3),
 edge3(v3, v1)
{
    cVector3F axis1 = v2 - v1;
    cVector3F axis2 = v3 - v1;
    normal = axis2.crossProduct(axis1);
}

//bool
//infiniteLineCollides(const cTriPreprocess& tri, const cLinePreprocess& line)
//{
//    eSide3D side1 = Side3D(line.pLine, tri.edge1);
//    assertD(side1 == Side3D_Reference(line.start, line.end, tri.v1, tri.v2));
//    if(side1 == S3D_ANTICLOCKWISE)
//    {
//        return false;
//    }
//    eSide3D side2 = Side3D(line.pLine, tri.edge2);
//    assertD(side2 == Side3D_Reference(line.start, line.end, tri.v2, tri.v3));
//    if(side2 == S3D_ANTICLOCKWISE)
//    {
//        return false;
//    }
//    eSide3D side3 = Side3D(line.pLine, tri.edge3);
//    assertD(side3 == Side3D_Reference(line.start, line.end, tri.v3, tri.v1));
//    if(side3 == S3D_ANTICLOCKWISE)
//    {
//        return false;
//    }
//    return side1 == S3D_CLOCKWISE || side2 == S3D_CLOCKWISE || side3 == S3D_CLOCKWISE;
//}

bool
infiniteLineCollides(const cTriPreprocess& tri, const cLinePreprocess& line)
{
    eSide3D side, side2;
        
    side = Side3D(line.pLine, tri.edge1);
//    assertD(side == Side3D_Reference(line.start, line.end, tri.v1, tri.v2));

    side2 = Side3D(line.pLine, tri.edge2);
//    assertD(side2 == Side3D_Reference(line.start, line.end, tri.v2, tri.v3));
    if(side != S3D_PARALLEL_OR_INTERSECTS)
    {
        if(side2 != S3D_PARALLEL_OR_INTERSECTS && side2 != side)
        {
            return false;
        }
    }
    else
    {
        side = side2;
    }

    side2 = Side3D(line.pLine, tri.edge3);
//    assertD(side2 == Side3D_Reference(line.start, line.end, tri.v3, tri.v1));
    if(side != S3D_PARALLEL_OR_INTERSECTS)
    {
        if(side2 != S3D_PARALLEL_OR_INTERSECTS && side2 != side)
        {
            return false;
        }
    }
    else
    {
        side = side2;
    }

    return side != S3D_PARALLEL_OR_INTERSECTS;
}

bool
infiniteLineCollides_BackFaceCull(const cTriPreprocess& tri, const cLinePreprocess& line)
{
    eSide3D side1, side2, side3;
        
    side1 = Side3D(line.pLine, tri.edge1);
    if(side1 == S3D_ANTICLOCKWISE)
    {
        return false;
    }
    side2 = Side3D(line.pLine, tri.edge2);
    if(side2 == S3D_ANTICLOCKWISE)
    {
        return false;
    }
    side3 = Side3D(line.pLine, tri.edge3);
    if(side3 == S3D_ANTICLOCKWISE)
    {
        return false;
    }
    return side1 == S3D_CLOCKWISE || side2 == S3D_CLOCKWISE || side3 == S3D_CLOCKWISE;
}

bool
lineCrossesPlane(const cTriPreprocess& tri, const cLinePreprocess& line)
{
    cVector3F offset = line.start - tri.origin;
    float p1, p2;
    p1 = tri.normal.dotProduct(offset);
    offset = line.end - tri.origin;
    p2 = tri.normal.dotProduct(offset);
    return (p1 < 0.0f && p2 > 0.0f) || (p2 < 0.0f && p1 > 0.0f);
}

bool
lineCollidesWithTriangle(const cTriPreprocess& tri, const cLinePreprocess& line)
{
    return infiniteLineCollides(tri, line) && lineCrossesPlane(tri, line);
}

bool
rayCollidesWithTriangle(const cTriPreprocess& tri, const cLinePreprocess& line, float& fractionAlong)
{
    if(!infiniteLineCollides_BackFaceCull(tri, line))
    {
        return false;
    }
    cVector3F offset = line.start - tri.origin;
    float p1 = tri.normal.dotProduct(offset);
    if(p1 <= 0.0f)
    {
    // starts on or behind the tri
        return false;
    }
    offset = line.end - tri.origin;
    float p2 = tri.normal.dotProduct(offset);
    fractionAlong = p1 / (p1 - p2);
    return true;
}

}
