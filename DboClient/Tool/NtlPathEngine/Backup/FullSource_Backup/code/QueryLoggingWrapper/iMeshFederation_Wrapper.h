//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_IMESHFEDERATION_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_IMESHFEDERATION_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iMeshFederation_Wrapper : public iMeshFederation, public iAPIObjectWrapper
{
public:

    typedef iMeshFederation tWrapped;

	iMeshFederation* _target;

    static tSigned32 interfaceIndex()
    {
        return 12;
    }

    void
    destroy();

    void
    save(const char* format, iOutputStream* outputStream) const;

    tSigned32
    size() const;

    bool
    coordinatesAreInsideRange(tSigned32 x, tSigned32 y) const;

    tSigned32
    tileForQuery(tSigned32 queryStartX, tSigned32 queryStartY) const;

    void
    getTileCentre(tSigned32 tileIndex, tSigned32& centreX, tSigned32& centreY) const;

    void
    getHandledRegion_World(tSigned32 tileIndex, cHorizontalRange& result) const;

    void
    getRepresentedRegion_World(tSigned32 tileIndex, cHorizontalRange& result) const;

    void
    getHandledRegion_Local(tSigned32 tileIndex, cHorizontalRange& result) const;

    void
    getRepresentedRegion_Local(tSigned32 tileIndex, cHorizontalRange& result) const;

    cPosition
    translatePosition(tSigned32 fromTileIndex, const iMesh* fromMesh, tSigned32 toTileIndex, const iMesh* toMesh, const cPosition& fromPosition) const;

    void
    getRepresentedRegionsOverlapped(const cHorizontalRange& queryRegion, tSigned32* resultsBuffer, tSigned32& numberOverlapped) const;

    iMesh*
    buildTileMeshFromContent(tSigned32 tileIndex, const iContentChunkInstance* const* instancePointers, tSigned32 numberOfInstances, const char *const* options, iOutputStream* os);

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
