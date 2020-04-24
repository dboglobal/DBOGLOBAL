//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LINE_COLLIDES_WITH_TRIANGLE_INCLUDED
#define LINE_COLLIDES_WITH_TRIANGLE_INCLUDED

#include "PlueckerLine.h"

namespace nLineCollidesWithTriangle
{

class cLinePreprocess
{
public:

    cLinePreprocess()
    {
    }
    cLinePreprocess(const cVector3F& start, const cVector3F& end);

    cVector3F start, end;
    cPlueckerLine pLine;
};

class cTriPreprocess
{
public:

    cTriPreprocess()
    {
    }
    cTriPreprocess(const cVector3F& v1, const cVector3F& v2, const cVector3F& v3);

    //... just for checking
    cVector3F v1, v2, v3;

    cVector3F origin, normal;
    cPlueckerLine edge1, edge2, edge3;
};

bool infiniteLineCollides(const cTriPreprocess& tri, const cLinePreprocess& line);
bool lineCrossesPlane(const cTriPreprocess& tri, const cLinePreprocess& line);

bool lineCollidesWithTriangle(const cTriPreprocess& tri, const cLinePreprocess& line);
bool rayCollidesWithTriangle(const cTriPreprocess& tri, const cLinePreprocess& line, float& fractionAlong);

};

#endif
