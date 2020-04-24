//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_ICONTENTCHUNKINSTANCE_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_ICONTENTCHUNKINSTANCE_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iContentChunkInstance_Wrapper : public iContentChunkInstance, public iAPIObjectWrapper
{
public:

    typedef iContentChunkInstance tWrapped;

	iContentChunkInstance* _target;

    static tSigned32 interfaceIndex()
    {
        return 11;
    }

    void
    destroy();

    iContentChunk*
    getContentChunk() const;

    const iFaceVertexMesh*
    getGroundPointer() const;

    const iAnchorsAndPinnedShapes*
    getAnchorsAndShapesPointer() const;

    void
    addAnchorsAndShapes(iMesh* mesh, const char* idPrefix, tSigned32 numberOfTerrainLayers) const;

    void
    getHorizontalRange(cHorizontalRange& result) const;

    tSigned32
    numberOfConnectingEdges() const;

    void
    getConnectingEdgeGeometry(tSigned32 connectingEdgeIndex, tSigned32& startX, tSigned32& startY, float& startZ, tSigned32& endX, tSigned32& endY, float& endZ) const;

    bool
    edgesCanConnect(tSigned32 connectingEdgeOnThis, iContentChunkInstance* targetInstance, tSigned32 connectingEdgeOnTarget) const;

    void
    makeEdgeConnection(tSigned32 connectingEdgeOnThis, iContentChunkInstance* targetInstance, tSigned32 connectingEdgeOnTarget) const;

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
