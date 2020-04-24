//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PATHENGINE_CORE_PATHFIND_PREPROCESS_DEFINED
#define PATHENGINE_CORE_PATHFIND_PREPROCESS_DEFINED

#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "libs/Mesh2D/interface/MeshCut_Header.h"
#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/PathEngine_Core/interface/CombinedCostCalculator.h"
#include "platform_common/ThreadSafeHandleTarget.h"
#include "common/Handle.h"
#include "common/Containers/BitVector.h"
#include "common/Containers/CollapsedVectorVector.h"
#include "common/STL/vector.h"

class cAttributesBuilder;
class cEdgeExpansion;
class cManagedShapeSet;
class cOffMeshConnections;
class cCornerProvider;
class iLineCollisionTester;
class cMeshPair;
class cVisibilityGraph;
template <class T> class cMeshWithTrackedTransform;
template <class T> class bStateSpace;
class cInternalPosition;
class iXMLInputStream;
class iXMLOutputStream;
class cQueryContext;
class cLocalisedConvexShape;
class cShapesCostCalculator;
class iMovementCostCalculator;
template <class T1, class T2> class cCombinedCostCalculator;
typedef cCombinedCostCalculator<cShapesCostCalculator, iMovementCostCalculator> tCombinedCostCalculator;
class cCompoundContextCollisionPreprocess;
class cContextPathfindPreprocess;
class cSmallConvexSplit;
class cCombinedStaticCircuits;
class iQueryCallBack;
class cCombinedPointCollisionTester;
class cManagedSoftObstacleExpansions;
class cGraph_StaticComponent;
class cStaticLineCollisionTester;

class iRandomStream
{
public:
    virtual ~iRandomStream() {}
    virtual tSigned32 random15() = 0;
};

class cPathfindPreprocess : public bThreadSafeHandleTarget
{
    cHandle<cEdgeExpansion> _edgeExpansion;
    cHandle<cManagedShapeSet> _baseObstacleExpansions;

    cAttributesBuilder* _attributesBuilder;

    bool _smallConvexWereSplit;
    tSigned32 _smallConvexMaximumCircumference;
    cSmallConvexSplit* _smallConvexSplit;
    cCombinedStaticCircuits* _keptCombinedCircuits;

    cStaticLineCollisionTester* _baseLineTester_NonSmallConvex;
    cCombinedPointCollisionTester* _basePointTester_NonSmallConvex;

    cHandle<cManagedSoftObstacleExpansions> _softObstacleExpansions;

    const std::vector<iMovementCostCalculator*>& _staticMesh3DCostCalculators;
    tCombinedCostCalculator _staticCostCalculator;
    cCornerProvider* _staticCorners;
    const cOffMeshConnections& _offMeshConnections;
    cBitVector _endPointObstructed;
    tSigned32 _numberOfHardCorners;

    cGraph_StaticComponent* _graphStaticComponent;

    // can be retained to help with debugging
    cMeshWithTrackedTransform<tMesh>* _partitioned;

    void processAttributes(
        const char *const* attributes,
        const char *const* collisionPreprocessAttributes
        );
    
    void
    buildSmallConvexSplitAndCombinedCircuits(
            cQueryContext& qc,
            bool keepPartitioned,
            bool justGeneratePartitioned,
            cCombinedStaticCircuits*& combinedCircuits
            );
    void
    loadSmallConvexSplit(
            cQueryContext& qc,
            iXMLInputStream& is
            );
    void setupCollisionTesters(cQueryContext& qc);

    cMeshWithTrackedTransform<tMesh>*
    cPathfindPreprocess::generateSilhouetteRegionsPartition(
            cQueryContext& qc,
            const cCombinedStaticCircuits& combinedCircuits
            ) const;

    void
    createCostCalculator();

    void
    createStaticCorners(
            cQueryContext& qc,
            const cCornerProvider& mesh3DCorners,
            const cCombinedStaticCircuits& combinedCircuits
            );

    void saveElementCorners(iXMLOutputStream& os) const;

    void
    createStaticCornersFromPersistence(
            cQueryContext& qc,
            iXMLInputStream& is,
            const cCornerProvider& mesh3DCorners
            );

    void buildEndPointObstructed(cQueryContext& qc);

public:

    // offMeshConnections cannot be modified over the lifetime of the preprocess

  // build
    cPathfindPreprocess(
        cQueryContext& qc,

//        cHandle<cCollisionPreprocess> cp,
        cHandle<cEdgeExpansion> edgeExpansion,
        cHandle<cManagedShapeSet> baseObstacleExpansions,
        const char *const* collisionPreprocessAttributes,

        const cOffMeshConnections& offMeshConnections,
        cHandle<cManagedSoftObstacleExpansions> softObstacleExpansions,
        const cCornerProvider& mesh3DCorners,
        const std::vector<iMovementCostCalculator*>& mesh3DCostCalculators,
        const char *const* attributes,
        bool keepPartitioned,
        bool justGeneratePartitioned = false
        );

  // load from persistence
    cPathfindPreprocess(
        cQueryContext& qc,

//        cHandle<cCollisionPreprocess> cp,
        cHandle<cEdgeExpansion> edgeExpansion,
        cHandle<cManagedShapeSet> baseObstacleExpansions,
        const char *const* collisionPreprocessAttributes,

        const cOffMeshConnections& offMeshConnections,
        cHandle<cManagedSoftObstacleExpansions> softObstacleExpansions,
        const cCornerProvider& mesh3DCorners,
        const std::vector<iMovementCostCalculator*>& mesh3DCostCalculators,
        iXMLInputStream& is,
        const char *const* attributes        
        );

    ~cPathfindPreprocess();

    void save(iXMLOutputStream& os) const;

    const char *const* getAttributes() const;

    bool testCollision_SmallConvex(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const;
    bool testPointCollision_SmallConvex(cQueryContext& qc, tFace face, const tPoint& point) const;
    const iLineCollisionTester& refLineTester_NonSmallConvex() const;
    const cCombinedPointCollisionTester& refPointTester_NonSmallConvex() const;

    void
    pushPathAroundSmallConvex(
            const std::vector<cInternalPosition>& path,
            const std::vector<tSigned32>* connectionIndices,
            std::vector<cInternalPosition>& result,
            std::vector<tSigned32>* connectionIndicesResult
            ) const;

    void
    pushPathAroundSmallConvex(
            const std::vector<cInternalPosition>& path,
            const std::vector<tSigned32>* connectionIndices,
            std::vector<cInternalPosition>& result,
            std::vector<tSigned32>* connectionIndicesResult,
            const cLocalisedConvexShapeSet& dynamicExpansions
            ) const;

    void getSmallConvexPotentiallyOverlapped(const cLocalisedConvexShape& shape, std::vector<tSigned32>& appendTo) const;


    const cLocalisedConvexShapeSet* getSmallConvexOnOriginal() const;
    const cLocalisedConvexShapeSet* getNonSmallConvex() const;
    const cMeshWithTrackedTransform<tMesh>* getSmallConvexPartitioned() const;
    const cMeshWithTrackedTransform<tMesh>& refCircuitCombinationPartitioned();
    bool smallConvexWereSplit() const;

    void findShortestPath(
                cQueryContext& qc, 
                const cInternalPosition& start, const cInternalPosition& end,
                std::vector<cInternalPosition>& result,
                std::vector<tSigned32>*& connectionIndices,
                iQueryCallBack* queryCallBack
                ) const;
    void findShortestPath(
                cQueryContext& qc, 
                const cInternalPosition& start, const cInternalPosition& end,
                const cCompoundContextCollisionPreprocess& cccp,
                const cContextPathfindPreprocess& cpp,
                std::vector<cInternalPosition>& result,
                std::vector<tSigned32>*& connectionIndices,
                iQueryCallBack* queryCallBack
                ) const;

    void findPathAway(
                cQueryContext& qc, 
                const cInternalPosition& start, const cInternalPosition& awayFrom,
                tSigned32 radius,
                std::vector<cInternalPosition>& result,
                std::vector<tSigned32>*& connectionIndices,
                iQueryCallBack* queryCallBack
                ) const;
    void findPathAway(
                cQueryContext& qc, 
                const cInternalPosition& start, const cInternalPosition& awayFrom,
                tSigned32 radius,
                const cCompoundContextCollisionPreprocess& cccp,
                const cContextPathfindPreprocess& cpp,
                std::vector<cInternalPosition>& result,
                std::vector<tSigned32>*& connectionIndices,
                iQueryCallBack* queryCallBack
                ) const;

    const cMeshWithTrackedTransform<tMesh>* getPartitioned() const
    {
        return _partitioned;
    }

    static tSigned32 majorVersion();
    static tSigned32 minorVersion();
};

#endif
