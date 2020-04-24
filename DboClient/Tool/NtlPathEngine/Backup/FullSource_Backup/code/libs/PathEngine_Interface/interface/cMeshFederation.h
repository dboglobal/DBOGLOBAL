//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_FEDERATION_INCLUDED
#define MESH_FEDERATION_INCLUDED

#include "libs/Geometry/interface/tPoint_Header.h"
#include "common/Containers/ReplacementList.h"
#include "i_pathengine.h"

class iAPIObjectWrapper;
class iOutputStream;
class iXMLOutputStream;
class cSimpleDOM;
class cMesh;
template <class T> class cCollapsedVectorVector;

class cMeshFederation : public iMeshFederation
{
  // immutable, except on construction and deletion
    cList_NoSize<cMeshFederation*>::iterator _heldNode;
    tSigned32 _startX, _startY;
    tSigned32 _width, _height;
    tSigned32 _tileSize, _overlap;
    tSigned32 _tilesInX, _tilesInY, _tiles;
    bool _translatesBySectionID;

    void pushBackSubsets(const tPoint& bottomLeft, const tPoint& topRight, cCollapsedVectorVector<tSigned32>& result);
    void buildSubsetsLookup(const cMesh& worldMesh, cCollapsedVectorVector<tSigned32>& result);

    bool isInHandledRegion_Local(tSigned32 tileIndex, tSigned32 x, tSigned32 y) const;

    void doSave(iXMLOutputStream& xos) const;

    void constructorCommon();

public:

    iAPIObjectWrapper* _wrapper;

    cMeshFederation(
            const cHorizontalRange& worldRange,
            tSigned32 tileSize, tSigned32 overlap,
            cList_NoSize<cMeshFederation*>::iterator heldNode
            );
    cMeshFederation(
            const cMesh& worldMesh,
            tSigned32 tileSize, tSigned32 overlap,
            iMeshFederationTileGeneratedCallBack& tileGeneratedCallBack,
            cList_NoSize<cMeshFederation*>::iterator heldNode
            );
    cMeshFederation(
            const char* format, const char* dataBuffer, tUnsigned32 bufferSize,
            cList_NoSize<cMeshFederation*>::iterator heldNode
            );
    ~cMeshFederation();

// iMeshFederation

    void destroy();
    void save(const char* format, iOutputStream* outputStream) const;
    tSigned32 size() const;
    bool coordinatesAreInsideRange(tSigned32 x, tSigned32 y) const;
    tSigned32 tileForQuery(tSigned32 queryStartX, tSigned32 queryStartY) const;
    void getTileCentre(tSigned32 tileIndex, tSigned32& centreX, tSigned32& centreY) const;
    void getHandledRegion_World(tSigned32 tileIndex, cHorizontalRange& result) const;
    void getRepresentedRegion_World(tSigned32 tileIndex, cHorizontalRange& result) const;
    void getHandledRegion_Local(tSigned32 tileIndex, cHorizontalRange& result) const;
    void getRepresentedRegion_Local(tSigned32 tileIndex, cHorizontalRange& result) const;
    cPosition translatePosition(tSigned32 fromTileIndex, const iMesh* fromMesh, tSigned32 toTileIndex, const iMesh* toMesh, const cPosition& fromPosition) const;
    void getRepresentedRegionsOverlapped(const cHorizontalRange& queryRegion, tSigned32* resultsBuffer, tSigned32& numberOverlapped) const;
    iMesh* buildTileMeshFromContent(tSigned32 tileIndex, const iContentChunkInstance* const* instancePointers, tSigned32 numberOfInstances, const char *const* options, iOutputStream* os);
    void operator delete(void* p) {::operator delete(p);}

//==================
};

inline cMeshFederation* FromI(iMeshFederation* ptr)
{
    return static_cast<cMeshFederation*>(ptr);
}

#endif
