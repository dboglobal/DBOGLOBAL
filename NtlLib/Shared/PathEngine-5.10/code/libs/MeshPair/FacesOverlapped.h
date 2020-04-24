//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef FACES_OVERLAPPED_INCLUDED
#define FACES_OVERLAPPED_INCLUDED

#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "common/TemplateMesh/MeshElement.h"
#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"

template <class T> class cMeshElement;

class cFacesOverlapped
{
    cBitVector _processedFlagBuffer;
    cBitVector _overlappedFlagBuffer;
    std::vector<tFace_3D> _openBuffer;
    std::vector<tSigned32> _overlapped;

    cMeshElement<tMesh_3D> _startElement;
    tFace_3D _currentFace;
    tEdge_3D _afterFirstBoundary;
    tEdge_3D _beforeBoundary;
    std::vector<cMeshElement<tMesh_3D> > _buffer;

    void addFace(tFace_3D f);
    void addFaceIfNotFlagged(tFace_3D f);
    void markFacesOutsideBoundary(tEdge_3D before, tEdge_3D after);

public:

    cFacesOverlapped(tSigned32 facesOnMesh);

    void init();
    void elementTraversed(const cMeshElement<tMesh_3D>& element);
    void closeAndTakeResult(std::vector<tSigned32>& swapResultTo);
};

#endif
