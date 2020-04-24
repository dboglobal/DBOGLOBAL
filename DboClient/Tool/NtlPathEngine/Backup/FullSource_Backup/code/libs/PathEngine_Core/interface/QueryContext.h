//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_CONTEXT_INCLUDED
#define QUERY_CONTEXT_INCLUDED

#include "common/interface/Assert.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"
class cFlagVector;
class cAStarHeapPool;
class cBitVector;

class cQueryContext
{
    tSigned32 _currentQuery, _nextQuery;

    const tMesh& _mesh;
    bool _meshHasInternalVertices;
    tSigned32 _numberOfInternalFaces;
    tSigned32 _maxElementCountRequirement;

    cFlagVector* _flagVector;
    bool _flagVectorLocked;

    char* _generalPurposeBuffer;
    tSigned32 _generalPurposeBufferSize;
    bool _generalPurposeBufferLocked;

    cBitVector* _elementBitBuffer;
    bool _elementBitBufferLocked;

public:
    
    cAStarHeapPool& _heapPool;

    cQueryContext(tMesh& mesh, tSigned32 numberOfInternalFaces, bool meshHasInternalVertices, cAStarHeapPool& heapPool);
    ~cQueryContext();

    void enter();
    void exit();

    bool insideQuery() const
    {
        return _currentQuery >= 0;
    }

//.. add when required
//    void addFlagsSizeRequirement(tSigned32 maxSize);
//    void addGeneralPurposeSizeRequirement(tSigned32 maxSize);

    void addElementCountRequirement(tSigned32 elementCount);

// interface provided for use by code inside a query

    //... support const meshes
    tMesh& refMesh() const
    {
        assertD(insideQuery());
        return const_cast<tMesh&>(_mesh);
    }
    tSigned32 getNumberOfInternalFaces() const
    {
        return _numberOfInternalFaces;
    }
    bool meshHasInternalVertices() const
    {
        return _meshHasInternalVertices;
    }

    tSigned32 getMaxElementCount() const
    {
        return _maxElementCountRequirement;
    }

    cFlagVector& lockFlagVector(tSigned32 sizeRequired);
    cFlagVector& lockFaceFlagVector();
    cFlagVector& lockElementFlagVector();
    void releaseFlagVector();

    char* lockGeneralPurposeBuffer(tSigned32 sizeRequired);
    tFace* lockFaceBuffer();
    void releaseGeneralPurposeBuffer();

    cBitVector& lockElementBitBuffer();
    void releaseElementBitBuffer();
};

#endif
