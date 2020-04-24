//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ELEMENTS_AROUND_2D_FACE_INCLUDED
#define ELEMENTS_AROUND_2D_FACE_INCLUDED

#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Geometry/interface/tLine.h"
#include "common/TemplateMesh/MeshElement.h"

class cElementsAround2DFace
{
public:

    enum eEvent
    {
        ELEMENT,
        CORNER,
    };

private:

    tEdge _e;
    tLine _traverseLine;
    cMeshElement<tMesh_3D> _el;
    eEvent _nextEvent;
    cMeshElement<tMesh_3D> _nextEl;

    void setNextEvent();
    void setNextEvent_EdgeStart();

public:

    cElementsAround2DFace(tFace f, const cMeshElement<tMesh_3D>& startElement);
    eEvent nextEvent(cMeshElement<tMesh_3D>& element);
};

#endif
