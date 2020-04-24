//**********************************************************************
//
// Copyright (c) 2002-2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

// this didnt make any noticeable difference over default setting (in VC 6)
//#pragma inline_depth(100)

#include "platform_common/Header.h"
#include "interface/PathfindPreprocess.h"
#include "CallBackGoalConditions.h"
#include "ElementInFaceLookup_Header.h"
#include "AStarHeap.h"
#include "SilhouetteRegion.h"
#include "PathAwayTargets.h"
#include "CombinedCornerProvider.h"
#include "VisibilityGraph.h"
#include "SmallConvexSplit.h"
#include "CombinedStaticCircuits.h"
#include "PushPathAroundObstacles.h"
#include "BoundedLineCollisionTester.h"
#include "ElementLookupCollisionTester.h"
#include "Graph_StaticComponent.h"
#include "Graph_NoContextDynamicComponent.h"
#include "Graph_DynamicComponent.h"
#include "Graph_SingleTarget.h"
#include "Graph.h"
#include "NullCornerProvider.h"
#include "StaticLineCollisionTester.h"
#include "interface/CombinedCollisionTesters.h"
#include "interface/CollisionPreprocess.h"
#include "interface/LineCollision.h"
#include "interface/iCollisionTester.h"
#include "interface/QueryContext.h"
#include "interface/AStarHeapPool.h"
#include "interface/LocalisedConvexShape.h"
#include "interface/CollisionAgainstShapeSet.h"
#include "interface/CornerProvider.h"
#include "interface/CompoundContextCollisionPreprocess.h"
#include "interface/ContextPathfindPreprocess.h"
#include "interface/ManagedSoftObstacleExpansions.h"
#include "interface/EdgeExpansion.h"
#include "libs/Mesh2D/interface/AddInternalVertices.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/TranslateToPartitioned.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh_Common/interface/MeshTraversal_Common.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Mesh_Common/interface/Triangulate.h"
#include "libs/Mesh_Common/interface/JoinFacesAtVertex.h"
#include "libs/Mesh_Common/interface/MeshWithTrackedTransform.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Instrumentation/interface/MemoryReport.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "platform_common/PointMultiplication.h"
#include "common/STL_Helpers.h"
#include "common/AttributesBuilder.h"
#include "common/Containers/FlagVector.h"
#include "common/interface/Assert.h"
#include "common/interface/Error.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/interface/iXMLInputStream.h"
#include "common/interface/Attributes.h"
#include "common/STL/algorithm.h"

void
cPathfindPreprocess::processAttributes(
        const char *const* attributes,
        const char *const* collisionPreprocessAttributes
        )
{
    _attributesBuilder = new cAttributesBuilder;
    {
        const char* value = GetValueForAttribute(attributes, "split_with_circumference_below");
        _smallConvexWereSplit = false;
        if(value)
        {
            _smallConvexWereSplit = true;
            _smallConvexMaximumCircumference = ValueAsLong(value);
            _attributesBuilder->addCopies("split_with_circumference_below", value);
        }
        else
        {
//            const char *const* cpAttributes = _cp->getAttributes();
//            const char* value = GetValueForAttribute(cpAttributes, "split_with_circumference_below");
            const char* value = GetValueForAttribute(collisionPreprocessAttributes, "split_with_circumference_below");
            if(value)
            {
                _smallConvexWereSplit = true;
                _smallConvexMaximumCircumference = ValueAsLong(value);
                _attributesBuilder->addCopies("split_with_circumference_below", value);
            }
        }
    }
    _attributesBuilder->finalise();
}

void
cPathfindPreprocess::buildSmallConvexSplitAndCombinedCircuits(
        cQueryContext& qc,
        bool keepPartitioned,
        bool justGeneratePartitioned,
        cCombinedStaticCircuits*& combinedCircuits
        )
{
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess small convex split");
        _smallConvexSplit = 0;
        if(_smallConvexWereSplit)
        {
            _smallConvexSplit = new cSmallConvexSplit(
                    qc,
                    _baseObstacleExpansions->refShapeSet(),
                    _edgeExpansion->refLineTester(),
                    _smallConvexMaximumCircumference
                    );
        }
    }
    if(_smallConvexWereSplit)
    {
        combinedCircuits = new cCombinedStaticCircuits(
                qc,
                _edgeExpansion->refEdgeExpansion(),
                _smallConvexSplit->refNonSmallConvex(),
                keepPartitioned,
                justGeneratePartitioned
                );
    }
    else
    {
        combinedCircuits = new cCombinedStaticCircuits(
                qc,
                _edgeExpansion->refEdgeExpansion(),
                _baseObstacleExpansions->refShapeSet(),
                keepPartitioned,
                justGeneratePartitioned
                );
    }
}

void
cPathfindPreprocess::loadSmallConvexSplit(
        cQueryContext& qc,
        iXMLInputStream& is
        )
{
    const char* nextElement = is.peekNextElement();
    _smallConvexSplit = 0;
    if(strcmp(nextElement, "smallConvexSplit") == 0)
    {
        assertD(_smallConvexWereSplit);
        is.openNextElement();
//        _smallConvexSplit = new cSmallConvexSplit(is, qc, _baseObstacleExpansions->refShapeSet());
        _smallConvexSplit = new cSmallConvexSplit(is, qc, _baseObstacleExpansions->refShapeSet());
        is.closeElement();
    }
    else
    {
        assertD(!_smallConvexWereSplit);
    }
}

void
cPathfindPreprocess::setupCollisionTesters(cQueryContext& qc)
{
    if(_smallConvexWereSplit)
    {
        _baseLineTester_NonSmallConvex = new cStaticLineCollisionTester(
                _edgeExpansion->refLineTester(),
                &_smallConvexSplit->refNonSmallConvex()
                );
    }
    else
    {
        _baseLineTester_NonSmallConvex = new cStaticLineCollisionTester(
                _edgeExpansion->refLineTester(),
                &_baseObstacleExpansions->refShapeSet()
                );
    }
    if(_smallConvexWereSplit)
    {
        _basePointTester_NonSmallConvex = new cCombinedPointCollisionTester(_edgeExpansion->refPointTester(), _smallConvexSplit->refNonSmallConvex());
    }
    else
    {
        _basePointTester_NonSmallConvex = new cCombinedPointCollisionTester(_edgeExpansion->refPointTester(), _baseObstacleExpansions->refShapeSet());
    }
}

cMeshWithTrackedTransform<tMesh>*
cPathfindPreprocess::generateSilhouetteRegionsPartition(
        cQueryContext& qc,
        const cCombinedStaticCircuits& combinedCircuits
        ) const
{
    const tMeshCircuits& circuits = *combinedCircuits.getCircuits_NotThreadSafe();

    tMesh& mesh = qc.refMesh();

    cMeshWithTrackedTransform<tMesh>* partitioned = new cMeshWithTrackedTransform<tMesh>(&mesh);
    if(qc.meshHasInternalVertices())
    {
        std::vector<tVertex> toRemove;
        tVertex v = partitioned->beginVertex();
        do
        {
            if(VertexIsExternal(tMesh::firstEdge(v)))
            {
                continue;
            }
            tPoint p = GetVertexPoint(v);
            tFace f = TranslateToOwned_SI<tMesh>(v, p);
            f = mesh.faceAt(f.index());
            if(refPointTester_NonSmallConvex().testPointCollision(qc, f, p))
            {
                continue;
            }
            toRemove.push_back(v);
        }
        while((++v).valid());
    
        tSigned32 i;
        for(i = 0; i != SizeL(toRemove); ++i)
        {
            JoinFacesAtVertex_KeepConvex(*partitioned, toRemove[i]);
        }
    }
    else
    {
        AddInternalVerticesInsideObstructions(*partitioned, circuits, 30);
    }
    partitioned->consolidate();
    
    return partitioned;
}

void
cPathfindPreprocess::createCostCalculator()
{
    _staticCostCalculator.setTarget1(&_softObstacleExpansions->refCostCalculator());
    _staticCostCalculator.setTarget2Vector(&_staticMesh3DCostCalculators);
}

void
cPathfindPreprocess::createStaticCorners(
        cQueryContext& qc,
        const cCornerProvider& mesh3DCorners,
        const cCombinedStaticCircuits& combinedCircuits
        )
{
    tSigned32 i;
    _staticCorners = new cCornerProvider(&qc.refMesh());
    for(i = 0; i < combinedCircuits.numberOfCornerElements(); ++i)
    {
        const cCircuitElement& el = combinedCircuits.refCornerElement(i);
        _staticCorners->push_back(
                    el.faceCrossed(0),
                    el.cornerPoint(), el.getPrev()->refLine().axis(), el.refLine().axis());
    }
    _numberOfHardCorners = _staticCorners->size();
    _softObstacleExpansions->addCornersTo(qc, *_staticCorners);

    _staticCorners->add(mesh3DCorners);
    _staticCorners->shrinkToFit();
}

void
cPathfindPreprocess::saveElementCorners(iXMLOutputStream& os) const
{
    _staticCorners->save(_numberOfHardCorners, os);
}

void
cPathfindPreprocess::createStaticCornersFromPersistence(
        cQueryContext& qc,
        iXMLInputStream& is,
        const cCornerProvider& mesh3DCorners
        )
{
    _staticCorners = new cCornerProvider(&qc.refMesh());
    is.openElement("elementCorners");
    _staticCorners->load(is);
    is.closeElement();
    _numberOfHardCorners = _staticCorners->size();
    _softObstacleExpansions->addCornersTo(qc, *_staticCorners);
    _staticCorners->add(mesh3DCorners);
    _staticCorners->shrinkToFit();
}

void
cPathfindPreprocess::buildEndPointObstructed(cQueryContext& qc)
{
    assertD(SizeL(_endPointObstructed) == _offMeshConnections.numberOfEndPoints());
    tSigned32 i;
    for(i = 0; i != _offMeshConnections.numberOfEndPoints(); ++i)
    {
        cInternalPosition endPointOnOriginal = _offMeshConnections.refEndPoints()[i];
        _endPointObstructed.setBit(
                i,
                _edgeExpansion->refPointTester().testPointCollision(qc, endPointOnOriginal.f, endPointOnOriginal.p)
                 || _baseObstacleExpansions->refShapeSet().testPointCollision(qc, endPointOnOriginal.f, endPointOnOriginal.p)
                );
    }
}

cPathfindPreprocess::cPathfindPreprocess(
        cQueryContext& qc,
        cHandle<cEdgeExpansion> edgeExpansion,
        cHandle<cManagedShapeSet> baseObstacleExpansions,
        const char *const* collisionPreprocessAttributes,
        const cOffMeshConnections& offMeshConnections,
        cHandle<cManagedSoftObstacleExpansions> softObstacleExpansions,
        const cCornerProvider& mesh3DCorners,
        const std::vector<iMovementCostCalculator*>& mesh3DCostCalculators,
        const char *const* attributes,
        bool keepPartitioned,
        bool justGeneratePartitioned
        ) :
 _edgeExpansion(edgeExpansion),
 _baseObstacleExpansions(baseObstacleExpansions),
 _staticMesh3DCostCalculators(mesh3DCostCalculators),
 _offMeshConnections(offMeshConnections),
 _endPointObstructed(offMeshConnections.numberOfEndPoints())
{
    _softObstacleExpansions = softObstacleExpansions;

    buildEndPointObstructed(qc);
    _keptCombinedCircuits = 0;

    processAttributes(attributes, collisionPreprocessAttributes);
    bool keepCircuitCombinationPartitioned = (GetValueForAttribute(attributes, "keep_circuit_combination_partitioned") != 0);
    bool justGenerateCircuitCombinationPartitioned = (GetValueForAttribute(attributes, "just_generate_circuit_combination_partitioned") != 0);

    buildSmallConvexSplitAndCombinedCircuits(qc, keepCircuitCombinationPartitioned, justGenerateCircuitCombinationPartitioned, _keptCombinedCircuits);
    if(justGenerateCircuitCombinationPartitioned)
    {
        assertD(keepCircuitCombinationPartitioned);
        _graphStaticComponent = 0;
        return;
    }
    setupCollisionTesters(qc);

    qc.addElementCountRequirement(_keptCombinedCircuits->elementCount());

    createCostCalculator();
    createStaticCorners(qc, mesh3DCorners, *_keptCombinedCircuits);

    _partitioned = generateSilhouetteRegionsPartition(qc, *_keptCombinedCircuits);

    if(justGeneratePartitioned)
    {
        if(!keepCircuitCombinationPartitioned)
        {
            delete _keptCombinedCircuits;
            _keptCombinedCircuits = 0;
        }
        assertD(keepPartitioned);
        _graphStaticComponent = 0;
        return;
    }

    _graphStaticComponent = new cGraph_StaticComponent(
            qc,
            _offMeshConnections,
            *_staticCorners, _numberOfHardCorners,
            *_baseLineTester_NonSmallConvex,
            refPointTester_NonSmallConvex(),
            &_staticCostCalculator
            );
    assertR(_partitioned);
    assertR(_keptCombinedCircuits);
    cSilhouetteLookup silhouetteLookup(
            qc,
            *_partitioned,
            *_keptCombinedCircuits,
            *_graphStaticComponent,
            _numberOfHardCorners
            );
    if(!keepCircuitCombinationPartitioned)
    {
        delete _keptCombinedCircuits;
        _keptCombinedCircuits = 0;
    }
    if(!keepPartitioned)
    {
        delete _partitioned;
        _partitioned = 0;
    }
    _graphStaticComponent->buildVisibilityGraph(qc, silhouetteLookup);
 }

cPathfindPreprocess::cPathfindPreprocess(
        cQueryContext& qc,
        cHandle<cEdgeExpansion> edgeExpansion,
        cHandle<cManagedShapeSet> baseObstacleExpansions,
        const char *const* collisionPreprocessAttributes,
        const cOffMeshConnections& offMeshConnections,
        cHandle<cManagedSoftObstacleExpansions> softObstacleExpansions,
        const cCornerProvider& mesh3DCorners,
        const std::vector<iMovementCostCalculator*>& mesh3DCostCalculators,
        iXMLInputStream& is,
        const char *const* attributes        
        ) :
 _edgeExpansion(edgeExpansion),
 _baseObstacleExpansions(baseObstacleExpansions),
 _staticMesh3DCostCalculators(mesh3DCostCalculators),
 _offMeshConnections(offMeshConnections),
 _endPointObstructed(offMeshConnections.numberOfEndPoints())
{
    _softObstacleExpansions = softObstacleExpansions;

    buildEndPointObstructed(qc);
    _keptCombinedCircuits = 0;

    processAttributes(attributes, collisionPreprocessAttributes);
    loadSmallConvexSplit(qc, is);
    setupCollisionTesters(qc);

    createCostCalculator();
    createStaticCornersFromPersistence(qc, is, mesh3DCorners);

    _partitioned = 0;

    _graphStaticComponent = new cGraph_StaticComponent(
            qc,
            _offMeshConnections,
            *_staticCorners, _numberOfHardCorners,
            *_baseLineTester_NonSmallConvex,
            refPointTester_NonSmallConvex(),
            &_staticCostCalculator,
            is
            );
 }

cPathfindPreprocess::~cPathfindPreprocess()
{
    MEMORY_REPORT_SCOPE("cPathfindPreprocess - destructor");
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - graph static component");
        delete _graphStaticComponent;
    }
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - attributes builder");
        delete _attributesBuilder;
    }
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - small convex split");
        delete _smallConvexSplit;
    }
    delete _keptCombinedCircuits;
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - base collision testers");
        delete _baseLineTester_NonSmallConvex;
        delete _basePointTester_NonSmallConvex;
    }
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - partition");
        delete _partitioned;
    }
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - static corners");
        delete _staticCorners;
    }
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - static cost calculators");
        tSigned32 i;
        for(i = 0; i < SizeL(_staticMesh3DCostCalculators); ++i)
        {
            delete _staticMesh3DCostCalculators[i];
        }
    }
#ifdef MEMORY_REPORTS_ON
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - edge expansion");
        _edgeExpansion = 0;
    }
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - base obstacle expansions");
        _baseObstacleExpansions = 0;
    }
    _softObstacleExpansions = 0;
    _endPointObstructed.completelyClear();
#endif
}

void
cPathfindPreprocess::save(iXMLOutputStream& os) const
{
    if(_smallConvexSplit)
    {
        os.openElement("smallConvexSplit");
        _smallConvexSplit->save(os);
        os.closeElement("smallConvexSplit");
    }
//    os.openElement("combinedStaticCircuits");
//    _combinedCircuits->save(os);
//    os.closeElement("combinedStaticCircuits");

    os.openElement("elementCorners");
    saveElementCorners(os);
    os.closeElement("elementCorners");

    _graphStaticComponent->save(os);
}

const char *const*
cPathfindPreprocess::getAttributes() const
{
    return _attributesBuilder->get();
}

bool
cPathfindPreprocess::testCollision_SmallConvex(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const
{
    if(from == to)
    {
        return false;
    }
    if(!_smallConvexWereSplit)
    {
        return false;
    }
    PROFILER_SCOPE("cPathfindPreprocess::testCollision_SmallConvex");
    tFace startF = _smallConvexSplit->translateToPartitioned(from.f, from.p);
    tFace endF = _smallConvexSplit->translateToPartitioned(to.f, to.p);
    return _smallConvexSplit->refSmallConvexOnPartitioned().testCollision(qc, cInternalPosition(startF, from.p), cInternalPosition(endF, to.p));
}
bool
cPathfindPreprocess::testPointCollision_SmallConvex(cQueryContext& qc, tFace face, const tPoint& point) const
{
    if(!_smallConvexWereSplit)
    {
        return false;
    }
    tFace faceOnPartitioned = _smallConvexSplit->translateToPartitioned(face, point);
    return PointCollision_AgainstShapeSet(_smallConvexSplit->refSmallConvexOnPartitioned(), cInternalPosition(faceOnPartitioned, point));
}

const iLineCollisionTester&
cPathfindPreprocess::refLineTester_NonSmallConvex() const
{
    return *_baseLineTester_NonSmallConvex;
}
const cCombinedPointCollisionTester&
cPathfindPreprocess::refPointTester_NonSmallConvex() const
{
    return *_basePointTester_NonSmallConvex;
}

void
cPathfindPreprocess::pushPathAroundSmallConvex(
        const std::vector<cInternalPosition>& path,
        const std::vector<tSigned32>* connectionIndices,
        std::vector<cInternalPosition>& result,
        std::vector<tSigned32>* connectionIndicesResult
        ) const
{
    assertD(_smallConvexWereSplit);
    nPushPathAroundObstacles::pushPathAroundSmallConvex(
        *_edgeExpansion,
        _smallConvexSplit->refNonSmallConvex(),
        _smallConvexSplit->getPartitioned(),
        _smallConvexSplit->refSmallConvexOnPartitioned(),
        0,
        path,
        connectionIndices,
        result,
        connectionIndicesResult
        );
}

void
cPathfindPreprocess::pushPathAroundSmallConvex(
        const std::vector<cInternalPosition>& path,
        const std::vector<tSigned32>* connectionIndices,
        std::vector<cInternalPosition>& result,
        std::vector<tSigned32>* connectionIndicesResult,
        const cLocalisedConvexShapeSet& dynamicExpansions
        ) const
{
    assertD(_smallConvexWereSplit);
    nPushPathAroundObstacles::pushPathAroundSmallConvex(
        *_edgeExpansion,
        _smallConvexSplit->refNonSmallConvex(),
        _smallConvexSplit->getPartitioned(),
        _smallConvexSplit->refSmallConvexOnPartitioned(),
        &dynamicExpansions,
        path,
        connectionIndices,
        result,
        connectionIndicesResult
        );
}

void
cPathfindPreprocess::getSmallConvexPotentiallyOverlapped(const cLocalisedConvexShape& shape, std::vector<tSigned32>& appendTo) const
{
    if(!_smallConvexWereSplit)
    {
        return;
    }
    GetShapesPotentiallyOverlappedBy(_smallConvexSplit->refSmallConvexOnOriginal(), shape, appendTo);
}


const cLocalisedConvexShapeSet*
cPathfindPreprocess::getSmallConvexOnOriginal() const
{
    return &_smallConvexSplit->refSmallConvexOnOriginal();
}
const cLocalisedConvexShapeSet*
cPathfindPreprocess::getNonSmallConvex() const
{
    return &_smallConvexSplit->refNonSmallConvex();
}
const cMeshWithTrackedTransform<tMesh>*
cPathfindPreprocess::getSmallConvexPartitioned() const
{
    return _smallConvexSplit->getPartitioned();
}
const cMeshWithTrackedTransform<tMesh>&
cPathfindPreprocess::refCircuitCombinationPartitioned()
{
    assertD(_keptCombinedCircuits);
    return _keptCombinedCircuits->refCircuitCombinationPartitioned();
}
bool
cPathfindPreprocess::smallConvexWereSplit() const
{
    return _smallConvexWereSplit;
}

static void
FilterZeroLengthPathSegments(
        std::vector<cInternalPosition>& result,
        std::vector<tSigned32>*& connectionIndices
        )
{
    if(!connectionIndices)
    {
      // note that this early out means that zero length connection from start to goal would not be filtered
        return;
    }
    std::vector<tSigned32>& indices = *connectionIndices;
    tSigned32 i;
    for(i = 0; i + 1 < SizeL(result); ++i)
    {
        if(result[i] == result[i + 1] && indices[i] == -1)
        {
            break;
        }
    }
    if(i + 1 == result.size())
    {
      // no segments need to be filtered
        return;
    }
    tSigned32 copyTo = i;
    // skip first filtered segment
    ++i;
    // scan for any further segments to be skipped
    for(; i + 1 < SizeL(result); ++i)
    {
        if(result[i] == result[i + 1] && indices[i] == -1)
        {
          // filter this segment
        }
        else
        {
            result[copyTo] = result[i];
            indices[copyTo] = indices[i];
            copyTo++;
        }
    }
    // end point of path
    result[copyTo] = result[i];
    indices.resize(copyTo);
    ++copyTo;
    result.resize(copyTo);
}

template <class STATESPACE, class ASTAR> void
PathFromAStar(
        STATESPACE& graph, ASTAR& searchHeap,
        cPointsOfVisibilityState goalState,
        std::vector<cInternalPosition>& result,
        std::vector<tSigned32>*& connectionIndices
        )
{
    assertD(connectionIndices == 0);
    if(!goalState.valid())
    {
        result.resize(0);
        return;
    }
    tSigned32 points = searchHeap.getNumberOfPathPointsTo(goalState);
    result.resize(points);

// fill in connection indices and all but last point

    cPointsOfVisibilityState state = goalState;
    cPointsOfVisibilityState parent = searchHeap.getParentState(state);
    tSigned32 i = points - 2;
    while(parent.valid())
    {
        tSigned32 connectionIndex = graph.getConnectionIndex(parent, state);
        state = parent;
        if(connectionIndex != -1)
        {
            if(!connectionIndices)
            {
                connectionIndices = new std::vector<tSigned32>(points - 1, -1);
            }
            (*connectionIndices)[i] = connectionIndex;
        }
        result[i] = graph.position(state.pointIndex());
        --i;
        parent = searchHeap.getParentState(state);
    }

// fill in last point

    if(points > 1)
    {
        result[points - 1] = graph.position(goalState.pointIndex(), result[points - 2]);
    }
    else
    {
        result[points - 1] = graph.position(goalState.pointIndex());
    }

// filter zero length segments

    FilterZeroLengthPathSegments(result, connectionIndices);
}

typedef cGraph<cGraph_NoContextDynamicComponent, cGraph_SingleTarget> tNoContextGraph;
typedef cGraph<cGraph_DynamicComponent, cGraph_SingleTarget> tWithContextGraph;
typedef cGraph<cGraph_NoContextDynamicComponent, cPathAwayTargets> tNoContextGraphAway;
typedef cGraph<cGraph_DynamicComponent, cPathAwayTargets> tWithContextGraphAway;

void
cPathfindPreprocess::findShortestPath(cQueryContext& qc, const cInternalPosition& start, const cInternalPosition& end,
                std::vector<cInternalPosition>& result,
                std::vector<tSigned32>*& connectionIndices,
                iQueryCallBack* queryCallBack
                ) const
{
    assertD(result.empty());
    assertD(connectionIndices == 0);
    if(start == end)
    {
        result.push_back(start);
        return;
    }
    PROFILER_SCOPE("cPathfindPreprocess::findShortestPath no obstacles, vector");

    cGraph_NoContextDynamicComponent dynamicComponent;
     
    tNoContextGraph graph(
            qc,
            *_graphStaticComponent, 
            dynamicComponent,
            start,
            cGraph_SingleTarget::cInitParams(*_graphStaticComponent, end)
            );

    cAStarLock lock(qc._heapPool);
    cPointsOfVisibilityState endState;
    if(queryCallBack)
    {
        cCallBackGoalConditions<tNoContextGraph> goalConditions(graph, *queryCallBack);
        endState = lock._heap.search(graph, goalConditions, graph.startState());
    }
    else
    {
        endState = lock._heap.search(graph, graph, graph.startState());
    }
    PathFromAStar(graph, lock._heap, endState, result, connectionIndices);
}

void
cPathfindPreprocess::findShortestPath(
                cQueryContext& qc, 
                const cInternalPosition& start,
                const cInternalPosition& end,
                const cCompoundContextCollisionPreprocess& cccp,
                const cContextPathfindPreprocess& cpp,
                std::vector<cInternalPosition>& result,
                std::vector<tSigned32>*& connectionIndices,
                iQueryCallBack* queryCallBack
                ) const
{
    assertD(result.empty());
    if(start == end)
    {
        result.push_back(start);
        return;
    }
    if(cpp.boundsValid() && !start.p.insideOrOnBounds(cpp.boundsMin(), cpp.boundsMax()))
    {
        result.push_back(start);
        return;
    }
    PROFILER_SCOPE("cPathfindPreprocess::findShortestPath obstacles, vector");

    cGraph_DynamicComponent dynamicComponent(cpp, false);

    tWithContextGraph graph(
            qc,
            *_graphStaticComponent, 
            dynamicComponent,
            start,
            cGraph_SingleTarget::cInitParams(*_graphStaticComponent, end)
            );

    cAStarLock lock(qc._heapPool);
    cPointsOfVisibilityState endState;
    if(queryCallBack)
    {
        cCallBackGoalConditions<tWithContextGraph> goalConditions(graph, *queryCallBack);
        endState = lock._heap.search(graph, goalConditions, graph.startState());
    }
    else
    {
        endState = lock._heap.search(graph, graph, graph.startState());
    }
    PathFromAStar(graph, lock._heap, endState, result, connectionIndices);
}

void
cPathfindPreprocess::findPathAway(
                cQueryContext& qc, 
                const cInternalPosition& start,
                const cInternalPosition& awayFrom,
                tSigned32 radius,
                std::vector<cInternalPosition>& result,
                std::vector<tSigned32>*& connectionIndices,
                iQueryCallBack* queryCallBack
                ) const
{
    assertD(result.empty());
    {
        tPoint p = start.p;
        if( p.getX() <= awayFrom.p.getX() - radius
            ||
            p.getX() >= awayFrom.p.getX() + radius
            ||
            p.getY() <= awayFrom.p.getY() - radius
            ||
            p.getY() >= awayFrom.p.getY() + radius)
        {
            result.push_back(start);
            return;
        }
    }

    cPathAwayTargets pathAwayTargets(cPathAwayTargets::cInitParams(
            qc, awayFrom, radius,
            *_edgeExpansion,
            _baseObstacleExpansions->refShapeSet()
            ));
    
    if(!pathAwayTargets.positionIsInside(start))
    {
        result.push_back(start);
        return;
    }

    cGraph_NoContextDynamicComponent dynamicComponent;
     
    tNoContextGraphAway graph(
            qc,
            *_graphStaticComponent, 
            dynamicComponent,
            start,
            cPathAwayTargets::cInitParams(
                qc, awayFrom, radius,
                *_edgeExpansion,
                _baseObstacleExpansions->refShapeSet()
                )
            );

    cAStarLock lock(qc._heapPool);
    cPointsOfVisibilityState endState;
    if(queryCallBack)
    {
        cCallBackGoalConditions<tNoContextGraphAway> goalConditions(graph, *queryCallBack);
        endState = lock._heap.search(graph, goalConditions, graph.startState());
    }
    else
    {
        endState = lock._heap.search(graph, graph, graph.startState());
    }
    PathFromAStar(graph, lock._heap, endState, result, connectionIndices);
}


void
cPathfindPreprocess::findPathAway(
                cQueryContext& qc, 
                const cInternalPosition& start,
                const cInternalPosition& awayFrom,
                tSigned32 radius,
                const cCompoundContextCollisionPreprocess& cccp,
                const cContextPathfindPreprocess& cpp,
                std::vector<cInternalPosition>& result,
                std::vector<tSigned32>*& connectionIndices,
                iQueryCallBack* queryCallBack
                ) const
{
    assertD(result.empty());
    {
        tPoint p = start.p;
        if( p.getX() <= awayFrom.p.getX() - radius
            ||
            p.getX() >= awayFrom.p.getX() + radius
            ||
            p.getY() <= awayFrom.p.getY() - radius
            ||
            p.getY() >= awayFrom.p.getY() + radius)
        {
            result.push_back(start);
            return;
        }
    }
    if(cpp.boundsValid() && !start.p.insideOrOnBounds(cpp.boundsMin(), cpp.boundsMax()))
    {
        result.push_back(start);
        return;
    }

    cPathAwayTargets pathAwayTargets(cPathAwayTargets::cInitParams(
            qc, awayFrom, radius,
            *_edgeExpansion,
            _baseObstacleExpansions->refShapeSet()
            ));
    
    if(!pathAwayTargets.positionIsInside(start))
    {
        result.push_back(start);
        return;
    }

    cGraph_DynamicComponent dynamicComponent(cpp, false);

    tWithContextGraphAway graph(
            qc,
            *_graphStaticComponent, 
            dynamicComponent,
            start,
            cPathAwayTargets::cInitParams(
                qc, awayFrom, radius,
                *_edgeExpansion,
                _baseObstacleExpansions->refShapeSet()
                )
            );

    cAStarLock lock(qc._heapPool);
    cPointsOfVisibilityState endState;
    if(queryCallBack)
    {
        cCallBackGoalConditions<tWithContextGraphAway> goalConditions(graph, *queryCallBack);
        endState = lock._heap.search(graph, goalConditions, graph.startState());
    }
    else
    {
        endState = lock._heap.search(graph, graph, graph.startState());
    }
    PathFromAStar(graph, lock._heap, endState, result, connectionIndices);
}

tSigned32
cPathfindPreprocess::majorVersion()
{
    return 4;
}
tSigned32
cPathfindPreprocess::minorVersion()
{
    return 0;
}
