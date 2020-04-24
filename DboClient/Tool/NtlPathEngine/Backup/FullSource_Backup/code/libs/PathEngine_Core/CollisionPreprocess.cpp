//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/CollisionPreprocess.h"
#include "ElementLookupCollisionTester.h"
#include "interface/CombinedCollisionTesters.h"
#include "interface/ObstacleInfo.h"
#include "interface/LocalisedConvexShapeSet.h"
#include "interface/LocalisedConvexShape.h"
#include "interface/CollisionAgainstShapeSet.h"
#include "interface/QueryContext.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh_Common/interface/MeshWithTrackedTransform.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "libs/Instrumentation/interface/MemoryReport.h"
#include "common/AttributesBuilder.h"
#include "common/interface/Attributes.h"

void
cCollisionPreprocess::processAttributes(const char *const* attributes)
{
    _attributesBuilder = new cAttributesBuilder;
    {
        const char* value = GetValueForAttribute(attributes, "split_with_circumference_below");
        if(value)
        {
//....
//            Error("Warning", "Attribute 'split_with_circumference_below' for collision preprocess generation is depreciated. Pass this attribute in for pathfind preprocess generation instead.");
            _attributesBuilder->addCopies("split_with_circumference_below", value);
        }
    }
    _attributesBuilder->finalise();
}

void
cCollisionPreprocess::buildBaseObstacleShapes(
        cQueryContext& qc,
        const std::vector<cObstacleInfo>& baseObstacles,
        const std::vector<cConvexPoly>& baseObstacleShapes,
        const cConvexPoly& expandShape
        )
{
    _baseObstacleExpansions = new cManagedShapeSet(qc.getNumberOfInternalFaces());
    tSigned32 i;
    for(i = 0; i != SizeL(baseObstacles); ++i)
    {
        const cObstacleInfo& info = baseObstacles[i];
        if(info._costToTraverse != -1.f)
        {
            continue;
        }
        cConvexPoly placedShape;
        baseObstacleShapes[i].offset(info._position.p, placedShape);
        cConvexPoly combinedShape;
        placedShape.minkowskiExpand(expandShape, combinedShape);
        cLocalisedConvexShape* lcs = new cLocalisedConvexShape(qc, combinedShape, info._position);
        _baseObstacleExpansions->push_back(lcs);
    }
    _baseObstacleExpansions->setCompleted();
}

void
cCollisionPreprocess::constructorCommon(
        cQueryContext& qc,
        const std::vector<cObstacleInfo>& baseObstacles,
        const std::vector<cConvexPoly>& baseObstacleShapes,
        const cConvexPoly& expandShape,
        const char *const* attributes
        )
{
    processAttributes(attributes);
    buildBaseObstacleShapes(qc, baseObstacles, baseObstacleShapes, expandShape);
    _baseLineTester_All = new cCombinedLineCollisionTester;
    _baseLineTester_All->setTargets(&_edgeExpansion->refLineTester(), &_baseObstacleExpansions->refShapeSet());
    _basePointTester_All = new cCombinedPointCollisionTester(_edgeExpansion->refPointTester(), _baseObstacleExpansions->refShapeSet());
}

cCollisionPreprocess::cCollisionPreprocess(
            cQueryContext& qc,
            cMeshWithTrackedTransform<tMesh>* mesh2D_NoInternalVertices,
            const std::vector<cObstacleInfo>& baseObstacles,
            const std::vector<cConvexPoly>& baseObstacleShapes,
            const cConvexPoly& expandShape,
            const char *const* attributes
            )
{
    _edgeExpansion = new cEdgeExpansion(qc, mesh2D_NoInternalVertices, expandShape);
    constructorCommon(qc, baseObstacles, baseObstacleShapes, expandShape, attributes);
}

cCollisionPreprocess::cCollisionPreprocess(
        cQueryContext& qc,
        iXMLInputStream& is,
        const std::vector<cObstacleInfo>& baseObstacles,
        const std::vector<cConvexPoly>& baseObstacleShapes,
        const cConvexPoly& expandShape,
        const char *const* attributes
        )
{
    _edgeExpansion = new cEdgeExpansion(qc, is);
    constructorCommon(qc, baseObstacles, baseObstacleShapes, expandShape, attributes);
}

cCollisionPreprocess::~cCollisionPreprocess()
{
    MEMORY_REPORT_SCOPE("cCollisionPreprocess - destructor");
    delete _attributesBuilder;
    delete _baseLineTester_All;
    delete _basePointTester_All;
#ifdef MEMORY_REPORTS_ON
    _edgeExpansion = 0;
    _baseObstacleExpansions = 0;
#endif
}

void
cCollisionPreprocess::save(iXMLOutputStream& os) const
{
    _edgeExpansion->save(os);
}

const char *const*
cCollisionPreprocess::getAttributes() const
{
    return _attributesBuilder->get();
}

tSigned32
cCollisionPreprocess::getElementCountRequirement() const
{
    return _edgeExpansion->getElementCountRequirement();
}
const tMeshCutSequence&
cCollisionPreprocess::refEdgeExpansion() const
{
    return _edgeExpansion->refEdgeExpansion();
}
//const std::vector<std::vector<cElementInFaceEntry> >&
//cCollisionPreprocess::refEdgeExpansionLookup() const
//{
//    return _edgeExpansion->refEdgeExpansionLookup();
//}

void
cCollisionPreprocess::firstCollision(cQueryContext& qc, tFace startF, const tLine& line, bool& collided, cAxisFraction& distanceAlong, tLine& collidingLine) const
{
    if(line.start() == line.end())
    {
        return;
    }
    _edgeExpansion->firstCollision(qc, startF, line, collided, distanceAlong, collidingLine);
    tSigned32 collidingAgent;
    FirstCollision_AgainstShapeSet(
        _baseObstacleExpansions->refShapeSet(),
        startF, line, collided, distanceAlong, collidingLine, collidingAgent
        );
}

void
cCollisionPreprocess::firstCollision(
        cQueryContext& qc, tFace startF,
        const tLine& line, const cAxisFraction& startFraction, const cAxisFraction& endFraction,
        bool& collided, cAxisFraction& distanceAlong, tLine& collidingLine
        ) const
{
    if(line.start() == line.end())
    {
        return;
    }
    _edgeExpansion->firstCollision(qc, startF, line, startFraction, endFraction, collided, distanceAlong, collidingLine);
    tSigned32 collidingAgent;
    FirstCollision_AgainstShapeSet(
        _baseObstacleExpansions->refShapeSet(),
        startF,
        line, startFraction, endFraction,
        collided, distanceAlong, collidingLine, collidingAgent
        );
}

const cElementLookupCollisionTester&
cCollisionPreprocess::refLineTester_Edges() const
{
    return _edgeExpansion->refLineTester();
}
const cElementLookupCollisionTester&
cCollisionPreprocess::refPointTester_Edges() const
{
    return _edgeExpansion->refPointTester();
}

tSigned32
cCollisionPreprocess::majorVersion()
{
    return 2;
}
tSigned32
cCollisionPreprocess::minorVersion()
{
    return 0;
}
