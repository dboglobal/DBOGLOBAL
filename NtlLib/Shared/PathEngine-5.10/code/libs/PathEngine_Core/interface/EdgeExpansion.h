//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PATHENGINE_CORE_EDGE_EXPANSION_DEFINED
#define PATHENGINE_CORE_EDGE_EXPANSION_DEFINED

#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "libs/Geometry/interface/tPoint.h"
#include "platform_common/ThreadSafeHandleTarget.h"
#include "common/STL/list.h"
#include "common/STL/vector.h"

class cQueryContext;
class cConvexPoly;
class cElementInFaceEntry;
class cElementLookupCollisionTester;
class cInternalPosition;
class cCombinedLineCollisionTester;
class cCombinedPointCollisionTester;
class iXMLInputStream;
class iXMLOutputStream;
class cAxisFraction;
template <class T> class cMeshWithTrackedTransform;

class cEdgeExpansion_CornerInFaceIterator
{
public:
    tSigned32 _nextI;
    tPoint _p;
    cEdgeExpansion_CornerInFaceIterator()
    {
        _nextI = 0;
    }
};

class cEdgeExpansion : public bThreadSafeHandleTarget
{
    cHandle<tMeshCutSequence> _edgeExpansion;
    tSigned32 _edgeExpansionElementCount;
    std::vector<std::vector<cElementInFaceEntry> >* _lookup;
    cElementLookupCollisionTester* _edgeExpansionCollisionTester;

public:

    typedef cEdgeExpansion_CornerInFaceIterator tCornerInFaceIterator;

    // perform expansion
    cEdgeExpansion(
        cQueryContext& qc,
        cMeshWithTrackedTransform<tMesh>* mesh2D_NoInternalVertices,
        const cConvexPoly& expandShape
        );

    // load from persistence
    cEdgeExpansion(
        cQueryContext& qc,
        iXMLInputStream& is
        );

    ~cEdgeExpansion();

    void save(iXMLOutputStream& os) const;

    tSigned32 getElementCountRequirement() const;

//....... get rid of these
    const tMeshCutSequence& refEdgeExpansion() const
    {
        return *_edgeExpansion;
    }
//    const std::vector<std::vector<cElementInFaceEntry> >& refEdgeExpansionLookup() const
//    {
//        return *_lookup;
//    }

    void firstCollision(cQueryContext& qc, tFace startF, const tLine& line, bool& collided, cAxisFraction& distanceAlong, tLine& collidingLine) const;
    void firstCollision(
            cQueryContext& qc,
            tFace startF,
            const tLine& line, const cAxisFraction& startFraction, const cAxisFraction& endFraction,
            bool& collided, cAxisFraction& distanceAlong, tLine& collidingLine
            ) const;

    const cElementLookupCollisionTester& refLineTester() const
    {
        return *_edgeExpansionCollisionTester;
    }
    const cElementLookupCollisionTester& refPointTester() const
    {
        return *_edgeExpansionCollisionTester;
    }

    bool iterateCornersInFace(tSigned32 fIndex, cEdgeExpansion_CornerInFaceIterator& it) const;

    tSigned32 elementsInFace(tSigned32 fIndex) const;
    const cElementInFaceEntry& elementInFaceInfo(tSigned32 fIndex, tSigned32 indexInFace) const;

};

#endif
