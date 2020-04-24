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
#include "interface/Mesh2DPersistence.h"
#include "interface/InternalPosition.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "platform_common/CoordinateRanges.h"
#include "common/CLib/stdlib.h"
#include "common/STL/string.h"
#include "common/STL/sstream.h"

void
EdgeFromString(tMesh& mesh, const char* str, tEdge& result)
{
    char *ptr;
    tSigned32 edgeIndex=strtol(str,&ptr,10);
    assertD(*ptr==0);
    result = mesh.edgeAt(edgeIndex);
}
void
FaceFromString(tMesh& mesh, const char* str, tFace& result)
{
    char *ptr;
    tSigned32 faceIndex=strtol(str,&ptr,10);
    assertD(*ptr==0);
    result = mesh.faceAt(faceIndex);
}

void
EntryInfoFromString(tMesh& mesh, const char* str, tEdge& e, bool& fromVertex)
{
    char* ptr;
    tSigned32 edgeIndex = strtol(str, &ptr, 10);
    assertD(edgeIndex >= 0);
    e = mesh.edgeAt(edgeIndex);
    assertD(*ptr == ':');
    str = ptr + 1;
    if(strcmp("EDGE", str) == 0)
    {
        fromVertex = false;
    }
    else
    {
        assertD(!strcmp("FROM_VERTEX", str));
        fromVertex = true;
    }
}
void ExitInfoFromString(tMesh& mesh, const char* str, tEdge& e, bool& toVertex)
{
    char* ptr;
    tSigned32 edgeIndex = strtol(str, &ptr, 10);
    assertD(edgeIndex >= 0);
    e = mesh.edgeAt(edgeIndex);
    assertD(*ptr == ':');
    str = ptr + 1;
    if(strcmp("EDGE", str) == 0)
    {
        toVertex = false;
    }
    else
    {
        assertD(!strcmp("TO_VERTEX", str));
        toVertex = true;
    }
}

std::string
PositionToString(const cInternalPosition& p)
{
    std::ostringstream oss;
    oss << p.f.index() << ':' << p.p.getX() << ',' << p.p.getY();
    return oss.str();
}

cInternalPosition
PositionFromString(tMesh& mesh, const char* str)
{
    tSigned32 fIndex;
    char* ptr;
    fIndex = strtol(str, &ptr, 10);
    assertD(*ptr == ':');
    str = ptr + 1;
    tSigned32 x, y;
    x = strtol(str, &ptr, 10);
    assertD(*ptr==',');
    str = ptr + 1;
    y = strtol(str, &ptr, 10);
    assertD(*ptr == 0);
    return cInternalPosition(mesh.faceAt(fIndex), tPoint(x, y, WORLD_RANGE));
}
