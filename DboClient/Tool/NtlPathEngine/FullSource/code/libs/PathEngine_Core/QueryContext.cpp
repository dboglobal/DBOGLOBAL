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
#include "interface/QueryContext.h"
#include "common/Containers/FlagVector.h"
#include "common/Containers/BitVector.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "AStarHeap.h"

cQueryContext::cQueryContext(tMesh& mesh, tSigned32 numberOfInternalFaces, bool meshHasInternalVertices, cAStarHeapPool& heapPool) :
 _mesh(mesh),
 _numberOfInternalFaces(numberOfInternalFaces),
 _meshHasInternalVertices(meshHasInternalVertices),
 _heapPool(heapPool)
{
    _flagVector = new cFlagVector(_mesh.faces());
    _flagVectorLocked = false;

    tSigned32 facesSize = _mesh.faces() * sizeof(tFace);
    _generalPurposeBuffer = new char [facesSize];
    _generalPurposeBufferSize = facesSize;
    _generalPurposeBufferLocked = false;

    _elementBitBuffer = new cBitVector;
    _elementBitBufferLocked = false;

    _maxElementCountRequirement = 0;

    _currentQuery = -1;
    _nextQuery = 0;
}
cQueryContext::~cQueryContext()
{
    assertD(!insideQuery());
    delete _flagVector;
    delete _elementBitBuffer;
    delete [] _generalPurposeBuffer;
}

void
cQueryContext::enter()
{
    assertD(!insideQuery());
    _currentQuery = _nextQuery;
    if(_nextQuery == 0x7fffffffL)
    {
        _nextQuery = -1;
    }
    ++_nextQuery;
}
void
cQueryContext::exit()
{
    assertD(insideQuery());
    _currentQuery = -1;
    assertD(!_flagVectorLocked);
    assertD(!_generalPurposeBufferLocked);
    assertD(!_elementBitBufferLocked);
}

void
cQueryContext::addElementCountRequirement(tSigned32 elementCount)
{
//    assertD(!insideQuery()); //... this element count requirement is now changed inside query context locking
    assertD(!_flagVectorLocked);
    assertD(!_elementBitBufferLocked);
    if(elementCount > _maxElementCountRequirement)
    {
        _maxElementCountRequirement = elementCount;
        if(_maxElementCountRequirement > _flagVector->size())
        {
            _flagVector->resize(_maxElementCountRequirement);
        }
        _elementBitBuffer->resize(_maxElementCountRequirement);
    }
}

cFlagVector&
cQueryContext::lockFlagVector(tSigned32 sizeRequired)
{
    assertD(insideQuery());
    assertD(!_flagVectorLocked);
    assertD(sizeRequired >= 0);
    assertD(sizeRequired <= _flagVector->size());
    _flagVectorLocked = true;
    return *_flagVector;
}
cFlagVector&
cQueryContext::lockFaceFlagVector()
{
    return lockFlagVector(_mesh.faces());
}
cFlagVector&
cQueryContext::lockElementFlagVector()
{
    return lockFlagVector(_maxElementCountRequirement);
}
void
cQueryContext::releaseFlagVector()
{
    assertD(insideQuery());
    assertD(_flagVectorLocked);
    _flagVectorLocked = false;
}
char*
cQueryContext::lockGeneralPurposeBuffer(tSigned32 sizeRequired)
{
    assertD(insideQuery());
    assertD(!_generalPurposeBufferLocked);
    assertD(sizeRequired >= 0);
    assertD(sizeRequired <= _generalPurposeBufferSize);
    _generalPurposeBufferLocked = true;
    return _generalPurposeBuffer;
}
tFace*
cQueryContext::lockFaceBuffer()
{
    return (tFace*)lockGeneralPurposeBuffer(_mesh.faces() * sizeof(tFace));
}
void
cQueryContext::releaseGeneralPurposeBuffer()
{
    assertD(insideQuery());
    assertD(_generalPurposeBufferLocked);
    _generalPurposeBufferLocked = false;
}

cBitVector&
cQueryContext::lockElementBitBuffer()
{
    assertD(insideQuery());
    assertD(!_elementBitBufferLocked);
    _elementBitBufferLocked = true;
    return *_elementBitBuffer;
}
void
cQueryContext::releaseElementBitBuffer()
{
    assertD(insideQuery());
    assertD(_elementBitBufferLocked);
    _elementBitBufferLocked = false;
}
