//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PATHENGINE_CORE_COLLISION_PREPROCESS_DEFINED
#define PATHENGINE_CORE_COLLISION_PREPROCESS_DEFINED

#include "libs/PathEngine_Core/interface/iFirstCollisionTester.h"
#include "libs/PathEngine_Core/interface/ManagedShapeSet.h"
#include "libs/PathEngine_Core/interface/EdgeExpansion.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "platform_common/ThreadSafeHandleTarget.h"
#include "common/Handle.h"
#include "common/STL/list.h"
#include "common/STL/vector.h"

class cConvexPoly;
class cAxisFraction;
class cLocalisedConvexShapeSet;
class cLocalisedConvexShape;
class cAttributesBuilder;
class cElementInFaceEntry;
class cElementLookupCollisionTester;
class cObstacleInfo;
class cQueryContext;
class cInternalPosition;
class iXMLInputStream;
class iXMLOutputStream;
class cCombinedLineCollisionTester;
class cCombinedPointCollisionTester;
class cEdgeExpansion;
template <class T> class cMeshWithTrackedTransform;

class cCollisionPreprocess : public bThreadSafeHandleTarget
{
    cAttributesBuilder* _attributesBuilder;

    cHandle<cEdgeExpansion> _edgeExpansion;
    cHandle<cManagedShapeSet> _baseObstacleExpansions;

    cCombinedLineCollisionTester* _baseLineTester_All;
    cCombinedPointCollisionTester* _basePointTester_All;
    
    void processAttributes(const char *const* attributes);

    void
    buildBaseObstacleShapes(
            cQueryContext& qc,
            const std::vector<cObstacleInfo>& baseObstacles,
            const std::vector<cConvexPoly>& baseObstacleShapes,
            const cConvexPoly& expandShape
            );

    void constructorCommon(
        cQueryContext& qc,
        const std::vector<cObstacleInfo>& baseObstacles,
        const std::vector<cConvexPoly>& baseObstacleShapes,
        const cConvexPoly& expandShape,
        const char *const* attributes
        );

public:

    // perform expansion
    cCollisionPreprocess(
        cQueryContext& qc,
        cMeshWithTrackedTransform<tMesh>* mesh2D_NoInternalVertices,
        const std::vector<cObstacleInfo>& baseObstacles,
        const std::vector<cConvexPoly>& baseObstacleShapes,
        const cConvexPoly& expandShape,
        const char *const* attributes
        );

    // load from persistence
    cCollisionPreprocess(
        cQueryContext& qc,
        iXMLInputStream& is,
        const std::vector<cObstacleInfo>& baseObstacles,
        const std::vector<cConvexPoly>& baseObstacleShapes,
        const cConvexPoly& expandShape,
        const char *const* attributes
        );
    
    ~cCollisionPreprocess();

    void save(iXMLOutputStream& os) const;

    const char *const* getAttributes() const;

    tSigned32 getElementCountRequirement() const;

    const tMeshCutSequence& refEdgeExpansion() const;
//    const std::vector<std::vector<cElementInFaceEntry> >& refEdgeExpansionLookup() const;
    cHandle<cEdgeExpansion> getEdgeExpansion() const
    {
        return _edgeExpansion;
    }

    const cLocalisedConvexShapeSet&
    refBaseObstacleExpansions() const
    {
        return _baseObstacleExpansions->refShapeSet();
    }
    cHandle<cManagedShapeSet> getBaseObstacleExpansions() const
    {
        return _baseObstacleExpansions;
    }

    void firstCollision(cQueryContext& qc, tFace startF, const tLine& line, bool& collided, cAxisFraction& distanceAlong, tLine& collidingLine) const;
    void firstCollision(
            cQueryContext& qc,
            tFace startF,
            const tLine& line, const cAxisFraction& startFraction, const cAxisFraction& endFraction,
            bool& collided, cAxisFraction& distanceAlong, tLine& collidingLine
            ) const;

    const cCombinedLineCollisionTester& refLineTester_All() const
    {
        return *_baseLineTester_All;
    }
    const cCombinedPointCollisionTester& refPointTester_All() const
    {
        return *_basePointTester_All;
    }
    const cElementLookupCollisionTester& refLineTester_Edges() const;
    const cElementLookupCollisionTester& refPointTester_Edges() const;

    static tSigned32 majorVersion();
    static tSigned32 minorVersion();
};

#endif
