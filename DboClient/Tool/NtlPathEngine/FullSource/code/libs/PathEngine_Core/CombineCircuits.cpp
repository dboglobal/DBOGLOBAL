//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "CombineCircuits.h"
#include "ElementInFaceLookup.h"
#include "ConnectNewCuts.h"
#include "interface/ObstacleInfo.h"
#include "interface/LocalisedConvexShape.h"
#include "interface/LocalisedConvexShapeSet.h"
#include "interface/OverlappingPolygon.h"
#include "interface/QueryContext.h"
#include "libs/Mesh2D/interface/AddInternalVertices.h"
#include "libs/Mesh2D/interface/TranslateToPartitioned.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Mesh_Common/interface/MeshWithTrackedTransform.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "common/TemplateMesh/ValidateMeshConnections.h"
#include "common/Containers/FlagVector.h"
#include "common/STL/list.h"

static void
AddInternalVertices(cMeshWithTrackedTransform<tMesh>& mesh,
                  const std::vector<cObstacleInfo>& obstacles,
                  const int frequency)
{
    assertD(frequency > 0);
    int frequencyCount = 0;
    tSigned32 i;
    for(i = 0; i != SizeL(obstacles); ++i)
    {
        if(obstacles[i]._costToTraverse != -1.f)
        {
            continue;
        }
        if(frequencyCount == 0)
        {
            tFace f = obstacles[i]._position.f;
            tPoint p = obstacles[i]._position.p;
            AddInternalVertex(mesh, f, p);
            frequencyCount = frequency;
        }
        frequencyCount--;
    }
}

void
CombineCircuits_WithInternalVertices_KeepPartitioned(
        cQueryContext& qc,
        tMesh& mesh,
        tMeshCircuits& edgeExpansionPieces,
        const std::vector<cObstacleInfo>& baseObstacles,
        const std::vector<cConvexPoly>& baseObstacleShapes,
        const cConvexPoly& expansionShape,
        tMeshCircuits& result,
        cMeshWithTrackedTransform<tMesh>*& partitioned
        )
{
    assertD(result.empty());
    std::list<tEdge> processedEdges;
    std::list<tEdge> processedVertices;
    tMeshCutSequence edgeExpansions;

    {
        cCutRegistry cutRegistry(mesh.faces(), mesh.edges());

        tSigned32 count = 0;
        tMeshCircuits::iterator i;
        for(i = edgeExpansionPieces.begin(); i != edgeExpansionPieces.end(); ++i)
        {
            SortOutSelfCrossings(**i);
//            cutRegistry.validateElements();
            ConnectExternalEdgeExpansion(cutRegistry, **i);
            count++;
        }
        cutRegistry.removeCutsThatArentInternal();

        cutRegistry.stripAsCuts(edgeExpansions);
//        JoinInlineCorners(result);
    }


    partitioned = new cMeshWithTrackedTransform<tMesh>(&mesh);
    AddInternalVertices(*partitioned, baseObstacles, 4);
    partitioned->consolidate();

    RemapCutSequence(edgeExpansions, *partitioned);

    tSigned32 edgesInPartitioned = 0;
    {
        tEdge e = partitioned->mesh().beginEdge();
        do
        {
            edgesInPartitioned++;
        }
        while(e.toNextInMesh());
    }

    cCutRegistry cutRegistry(partitioned->faces(), edgesInPartitioned);

    cutRegistry.addCuts(edgeExpansions);

    cFlagVector faceFlagsBuffer(partitioned->faces());
    std::vector<tFace> faceBuffer(partitioned->faces());

    tSigned32 i;
    for(i = 0; i != SizeL(baseObstacles); ++i)
    {
        const cObstacleInfo& info = baseObstacles[i];
        if(info._costToTraverse != -1.f)
        {
            continue;
        }

        tFace faceOnPartitioned;
        {
            cPoint_PointLike pl(info._position.p);
            cMeshElement<tMesh> el = ElementForPointInOverlappingFace<tMesh>(info._position.f, info._position.p);
            cMeshElement<tMesh> remappedEl = partitioned->translate(el, pl);
            faceOnPartitioned = TranslateToOwned_SI(remappedEl, info._position.p);
        }

        cInternalPosition positionOnPartitioned(faceOnPartitioned, info._position.p);
        cConvexPoly placedShape;
        baseObstacleShapes[i].offset(info._position.p, placedShape);
        cConvexPoly combinedShape;
        placedShape.minkowskiExpand(expansionShape, combinedShape);
        cLocalisedConvexShape lcs(combinedShape, positionOnPartitioned, &faceBuffer.front(), faceFlagsBuffer);

        tMeshCircuits circuitsToAdd;
        {
            std::list<tFace> containedExternalFaces;
            CircuitsForOverlappingPolygon(
                    faceOnPartitioned,
                    combinedShape,
                    containedExternalFaces,
                    circuitsToAdd);
        }

        ConnectBaseObstacleExpansion(cutRegistry, circuitsToAdd, lcs, faceFlagsBuffer);

        cutRegistry.removeCutsThatArentInternal();
    }
    cutRegistry.stripAsCircuits(result);
//    JoinInlineCorners(result);

    cReversedMeshTranslator<tMesh> reversed(partitioned);
    RemapCircuits(result, reversed);
}

void
CombineCircuits_WithInternalVertices(
        cQueryContext& qc,
        tMesh& mesh, 
        tMeshCircuits& edgeExpansionPieces,
        const std::vector<cObstacleInfo>& baseObstacles,
        const std::vector<cConvexPoly>& baseObstacleShapes,
        const cConvexPoly& expansionShape,
        tMeshCircuits& result)
{
    cMeshWithTrackedTransform<tMesh>* partitioned;
    CombineCircuits_WithInternalVertices_KeepPartitioned(
        qc,
        mesh, 
        edgeExpansionPieces,
        baseObstacles, baseObstacleShapes,
        expansionShape,
        result,
        partitioned);
    delete partitioned;
}

void
ConnectBaseObstacleCircuits(
        cQueryContext& qc,
        tMeshCutSequence& edgeExpansion,
        const cLocalisedConvexShapeSet& baseObstacleExpansions,
        tMeshCircuits& result,
        bool keepPartitioned,
        bool justGeneratePartitioned,
        cMeshWithTrackedTransform<tMesh>*& partitioned
        )
{
    assertF(ValidateMeshConnections(qc.refMesh()));
    partitioned = new cMeshWithTrackedTransform<tMesh>(&qc.refMesh());
    {
        tSigned32 i;
        for(i = 0; i < baseObstacleExpansions.size(); i += 4)
        {
            cInternalPosition p = baseObstacleExpansions[i].centre();
            AddInternalVertex(*partitioned, p.f, p.p);
        }
    }
    assertF(ValidateMeshConnections(partitioned->mesh()));
    partitioned->consolidate(); // note that this interferes with external face order

    if(justGeneratePartitioned)
    {
        return;
    }

    RemapCutSequence(edgeExpansion, *partitioned);

    cCutRegistry cutRegistry(partitioned->faces(), partitioned->edges());
    cutRegistry.addCuts(edgeExpansion);

    cFlagVector faceFlagsBuffer(partitioned->faces());
    std::vector<tFace> faceBuffer(partitioned->faces());

    tSigned32 i;
    for(i = 0; i != baseObstacleExpansions.size(); ++i)
    {
        const cLocalisedConvexShape& s = baseObstacleExpansions[i];
        cInternalPosition pPartitioned = TranslateToPartitioned(*partitioned, s.centre());

        cLocalisedConvexShape* lcs = new cLocalisedConvexShape(s.poly(), pPartitioned, &faceBuffer.front(), faceFlagsBuffer);

        //..... optimise - build circuits at same time as building lcs?
        //..... optimise - if the shape exits the mesh then use a faster circuit building method

        tMeshCircuits circuitsToAdd;
        {
            std::list<tFace> containedExternalFaces;
            CircuitsForOverlappingPolygon(
                    pPartitioned.f,
                    s.poly(),
                    containedExternalFaces,
                    circuitsToAdd);
        }

        ConnectBaseObstacleExpansion(cutRegistry, circuitsToAdd, *lcs, faceFlagsBuffer);

        delete lcs;

        cutRegistry.removeCutsThatArentInternal();
    }
    cutRegistry.stripAsCircuits(result);
//    JoinInlineCorners(result);

    cReversedMeshTranslator<tMesh> reversed(partitioned);
    RemapCircuits(result, reversed);

    if(!keepPartitioned)
    {
        delete partitioned;
        partitioned = 0;
    }
}
