//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EDGES_INTO_2D_FACE_INCLUDED
#define EDGES_INTO_2D_FACE_INCLUDED

#include "ElementsAround2DFace.h"
#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"

class cEdgesInto2DFace
{
public:

    enum eEvent
    {
        NONE = 0,
        EDGES = 1,
        CORNER = 2,
        COMPLETED = 4,
    };

private:

    std::vector<cMeshElement<tMesh_3D> > _elementWindow;
    cBitVector _containsCornerWindow;
    cElementsAround2DFace _elementsAround;
    tEdge _e;
    bool _completed;

    cEdgesInto2DFace::eEvent
    rotateWindowAndCheckEvent(
            const cMeshElement<tMesh_3D>& nextEl,
            tEdge_3D& firstEdge, tEdge_3D& lastEdge
            );

public:

    cEdgesInto2DFace(tFace f, const cMeshElement<tMesh_3D>& startElement);
    eEvent nextEvent(tEdge_3D& firstEdge, tEdge_3D& lastEdge);
};

#endif
