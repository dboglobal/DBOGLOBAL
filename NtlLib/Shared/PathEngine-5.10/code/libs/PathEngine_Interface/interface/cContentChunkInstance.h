//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_CHUNK_INSTANCE_INCLUDED
#define CONTENT_CHUNK_INSTANCE_INCLUDED

#include "i_pathengine.h"
#include "common/STL/vector.h"

class cContentChunk;
class iAPIObjectWrapper;
class cBridgingModifier;

class cContentChunkInstance : public iContentChunkInstance
{
    cContentChunk* _base;
    tSigned32 _indexInBase;
    const iFaceVertexMesh* _faceVertex;
    cBridgingModifier* _bridgingModifier;
    const iFaceVertexMesh* _beforeBridgingModifier;
    const iAnchorsAndPinnedShapes* _anchorsAndShapes;
    cHorizontalRange _horizontalRange;
    bool _hasGroundFootprint;
    cHorizontalRange _footprintRange;
    tSigned32 _sectionID;

public:

    iAPIObjectWrapper* _wrapper;

    cContentChunkInstance(
            cContentChunk* base, tSigned32 indexInBase,
            const iFaceVertexMesh* faceVertex,
            const std::vector<tSigned32>& connectingEdgeInfo,
            const iAnchorsAndPinnedShapes* anchorsAndShapes,
            tSigned32 sectionID
            );
    ~cContentChunkInstance();

    tSigned32 getSectionID() const
    {
        return _sectionID;
    }

    bool hasGroundFootprint() const
    {
        return _hasGroundFootprint;
    }
    void getGroundFootprintRange(cHorizontalRange& result) const;

// iContentChunkInstance

    void destroy();
    iContentChunk* getContentChunk() const;
    const iFaceVertexMesh* getGroundPointer() const;
    const iAnchorsAndPinnedShapes* getAnchorsAndShapesPointer() const;
    void addAnchorsAndShapes(iMesh* mesh, const char* idPrefix, tSigned32 numberOfTerrainLayers) const;
    void getHorizontalRange(cHorizontalRange& result) const;
    tSigned32 numberOfConnectingEdges() const;
    void getConnectingEdgeGeometry(tSigned32 connectingEdgeIndex, tSigned32& startX, tSigned32& startY, float& startZ, tSigned32& endX, tSigned32& endY, float& endZ) const;
    bool edgesCanConnect(tSigned32 connectingEdgeOnThis, iContentChunkInstance* targetInstance, tSigned32 connectingEdgeOnTarget) const;
    void makeEdgeConnection(tSigned32 connectingEdgeOnThis, iContentChunkInstance* targetInstance, tSigned32 connectingEdgeOnTarget) const;
    void operator delete(void* p) {::operator delete(p);}

//==================
};

inline cContentChunkInstance* FromI(iContentChunkInstance* ptr)
{
    return static_cast<cContentChunkInstance*>(ptr);
}
inline const cContentChunkInstance* FromI(const iContentChunkInstance* ptr)
{
    return static_cast<const cContentChunkInstance*>(ptr);
}

#endif
