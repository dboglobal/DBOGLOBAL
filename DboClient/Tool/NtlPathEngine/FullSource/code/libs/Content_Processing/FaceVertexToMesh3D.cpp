//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "FaceVertexToMesh3D.h"
#include "VerticalEdgeCollapses.h"
#include "CollapsedVertexFilter.h"
#include "ConnectionsMap.h"
#include "ConnectingEdgeInfo.h"
#include "ExternalShapeInfo.h"
#include "libs/Mesh_Common/interface/Triangulate.h"
#include "common/Containers/BitVector.h"
#include "common/interface/Assert.h"
#include "common/interface/Error.h"
#include "common/interface/Attributes.h"
#include "common/TemplateMesh/External.h"
#include "common/AttributesBuilder.h"
#include "common/STL/utility.h"
#include "platform_common/SquareRoot.h"
#include "i_pathengine.h"

typedef tMesh_3D_FaceTracked tMesh;
typedef tFace_3D_FaceTracked tFace;
typedef tEdge_3D_FaceTracked tEdge;

static bool
FaceMarksPortal(const iFaceVertexMesh& mesh, tSigned32 face)
{
    return mesh.faceAttribute(face, PE_FaceAttribute_MarksPortal) == 1 || mesh.faceAttribute(face, PE_FaceAttribute_MarksTerrainPortal) == 1;
}
static bool
FaceMarksTerrainPortal(const iFaceVertexMesh& mesh, tSigned32 face)
{
    return mesh.faceAttribute(face, PE_FaceAttribute_MarksTerrainPortal) == 1;
}
static bool
FaceMarksExternalShape(const iFaceVertexMesh& mesh, tSigned32 face)
{
    return mesh.faceAttribute(face, PE_FaceAttribute_MarksExternalShape) == 1;
}
static bool
FaceMarksConnectingEdge(const iFaceVertexMesh& mesh, tSigned32 face)
{
    return mesh.faceAttribute(face, PE_FaceAttribute_MarksConnectingEdge) == 1;
}

static void
GetPortalVertexPairs(const iFaceVertexMesh& mesh,
                      const cVerticalEdgeCollapses& collapses,
                      std::vector<cVertexPair>& result,
                      cBitVector& terrainPortalFlags,
                      const char *const* errorAttributes
                      )
{
    assertD(result.empty());
    bool reportNonVerticalPortalMarkers = true;
    bool reportCollapsedPortalFaces = true;

    tSigned32 i;
    for(i = 0; i < mesh.faces(); ++i)
    {
        if(!FaceMarksPortal(mesh, i))
        {
            continue;
        }

        if(!collapses.faceWasCollapsed(i))
        {
			if(reportNonVerticalPortalMarkers)
			{
                cAttributesBuilder attributes;
                attributes.add(errorAttributes);
                attributes.add("faceIndex", i);
                Error("NonFatal", "A face is marked as a portal,"
                    " but does not contain a vertical edge."
                    " The face will be treated as a non-portal face."
					" (Note that only the first such face per mesh object is reported.)",
                    attributes.get());
				reportNonVerticalPortalMarkers = false;
			}
            continue;
        }

        tSigned32 v1 = collapses.getCollapsedIndex(mesh.vertexIndex(i, 0));
        tSigned32 v2 = collapses.getCollapsedIndex(mesh.vertexIndex(i, 1));
        if(v1 == v2)
        {
            v2 = collapses.getCollapsedIndex(mesh.vertexIndex(i, 2));
            if(v1 == v2)
            {
                if(reportCollapsedPortalFaces)
                {
                    cAttributesBuilder attributes;
                    attributes.add(errorAttributes);
                    attributes.add("faceIndex", i);
                    Error("NonFatal", "A face marked as a portal has zero size after approximation."
                        " The face will be treated as a non-portal face."
				        " (Note that only the first such face per mesh object is reported.)",
                        attributes.get());
			        reportCollapsedPortalFaces = false;
                }
                continue;
            }
        }

        assertD(mesh.vertexX(v1) != mesh.vertexX(v2) || mesh.vertexY(v1) != mesh.vertexY(v2)); // otherwise should have collapsed to the same vertex index
        cVertexPair vp(v1, v2);

        tSigned32 j;
        for(j = 0; j < SizeL(result); ++j)
        {
            if(result[j] == vp)
            {
                break;
            }
        }
        if(j == result.size())
        {
            result.push_back(vp);
            terrainPortalFlags.push_back(FaceMarksTerrainPortal(mesh, i));
        }
    }
}

static bool
GetSingleConnection(
           const cConnectionsMap& connectionsMap,
           const cVertexPair& vertexPair,
           cFaceVertexEdge& edge,
           bool& direction
           )
{
    const tEdgeConnections& connections = connectionsMap.lookupConnections(vertexPair);
    const std::vector<cFaceVertexEdge>* v;
    bool otherEmpty;
    if(connections.first.empty())
    {
        v = &connections.second;
        otherEmpty = connections.first.empty();
        direction = true;
    }
    else
    {
        v = &connections.first;
        otherEmpty = connections.second.empty();
        direction = false;
    }
    if(v->size() != 1 || !otherEmpty)
    {
        cFaceVertexEdge result;
        result.clear();
        return false;
    }
    edge = v->front();
    return true;
}

static void
GetPortals(const iFaceVertexMesh& mesh,
           const cConnectionsMap& connectionsMap,
           const cConnectionsMap& connectionsMap_ExternalShapes,
           const std::vector<cVertexPair>& portalVertexPairs,
           const cBitVector& portalVertexPairsTerrainPortalFlags,
           std::vector<cFaceVertexEdge>& portals,
           cBitVector& portalTerrainPortalFlags,
           std::vector<cFaceVertexEdge>& externalShapeEdgesForPortals,
           const char *const* errorAttributes
           )
{
    bool reportBadPortals = true;
    tSigned32 i;
    for(i = 0; i < SizeL(portalVertexPairs); ++i)
    {
        cFaceVertexEdge e1, e2;
        bool direction1, direction2;
        if(!GetSingleConnection(connectionsMap, portalVertexPairs[i], e1, direction1)
         || !GetSingleConnection(connectionsMap_ExternalShapes, portalVertexPairs[i], e2, direction2)
         || direction1 != direction2
         )
        {
			if(reportBadPortals)
			{
                cAttributesBuilder attributes;
                attributes.add(errorAttributes);
                attributes.add("vertexIndex1", portalVertexPairs[i]._v1);
                attributes.add("vertexIndex2", portalVertexPairs[i]._v2);
                Error("NonFatal", "Badly formed portal."
                    " Portals must connect once to an internal ground mesh,"
                    " and once to an external shape,"
                    " with both connections being in the same direction."
                    " Badly formed portals will be skipped."
					" (Note that only the first such portal per mesh object is reported.)",
                    attributes.get());
				reportBadPortals = false;
			}
            continue;
        }
        portals.push_back(e1);
        portalTerrainPortalFlags.push_back(portalVertexPairsTerrainPortalFlags[i]);
        externalShapeEdgesForPortals.push_back(e2);
    }
}

static bool
ConnectionsGoAllAroundVertex(const iFaceVertexMesh& mesh, 
                             const cConnectionsMap& connectionsMap,
                             cFaceVertexEdge beforeEdge,
                             const cFaceVertexEdge& afterEdge
                             )
{
    while(1)
    {
        const tEdgeConnections& connections = connectionsMap.lookupConnections(beforeEdge.vp(mesh));
        if(connections.first.size() != 1
         || connections.second.size() != 1)
        {
          // note - returns false if there are non manifold edges around the vertex
            return false;
        }
        cFaceVertexEdge twin;
        if(connections.first.front() == beforeEdge)
        {
            twin = connections.second.front();
        }
        else
        {
            if(connections.second.front() != beforeEdge)
            {
              // connection was non-manifold and beforeEdge got disconnected
                return false;
            }
            twin = connections.first.front();
        }
        if(twin == afterEdge)
        {
            return true;
        }
        beforeEdge = twin.prev();
    }
}

static bool
AttemptToResolveCutPortal(
                 const iFaceVertexMesh& mesh, cConnectionsMap& connectionsMap,
                 const cFaceVertexEdge& source,
                 const std::vector<cFaceVertexEdge>& targetCandidates
                 )
{                   
    if(ConnectionsGoAllAroundVertex(mesh, connectionsMap, source.prev(), targetCandidates[0].next())
        || ConnectionsGoAllAroundVertex(mesh, connectionsMap, targetCandidates[0].prev(), source.next())
        )
    {
        connectionsMap.disconnectSingle(targetCandidates[0], mesh);
        return true;
    }
    if(ConnectionsGoAllAroundVertex(mesh, connectionsMap, source.prev(), targetCandidates[1].next())
        || ConnectionsGoAllAroundVertex(mesh, connectionsMap, targetCandidates[1].prev(), source.next())
        )
    {
        connectionsMap.disconnectSingle(targetCandidates[1], mesh);
        return true;
    }
    return false;
}

float
ResolveRangeForConnectingEdge(const cCollapsedVertexFilter& mesh, const cFaceVertexEdge& edgeFV)
{
    float minZ, maxZ;
    mesh.vertexZRange(edgeFV.vertexIndex(mesh), minZ, maxZ);
    float range = maxZ - minZ;
    mesh.vertexZRange(edgeFV.next().vertexIndex(mesh), minZ, maxZ);
    float range2 = maxZ - minZ;
    if(range2 > range)
    {
        range = range2;
    }
    return range;
}

void
ResolveNonManifold(const iFaceVertexMesh& mesh,
                   const cBitVector& faceSkipFlags,
                   cConnectionsMap& connectionsMap,
                   const char *const* errorAttributes
                   )
{
    bool reportNonManifold = true;
    tSigned32 i;
    for(i = 0; i < mesh.faces(); ++i)
    {
        if(faceSkipFlags[i])
        {
            continue;
        }
        tSigned32 j;
        for(j = 0; j < 3; ++j)
        {
            cFaceVertexEdge edge(i, j);
            const tEdgeConnections& connections = connectionsMap.lookupConnections(edge.vp(mesh));
            if(connections.first.size() <= 1 && connections.second.size() <= 1)
            {
            // not non-manifold
                continue;
            }
            else if(connections.first.size() > 1 && connections.second.size() == 0)
            {
              // 'vertex reuse', with all edges connecting in the same direction
              // not really non-manifold, straightforward to resolve
                connectionsMap.disconnectAll(edge.vp(mesh));
                continue;
            }
            else if(connections.first.size() == 1 && connections.second.size() == 2)
            {            
              // 'cut portal' type non manifold
                if(AttemptToResolveCutPortal(mesh, connectionsMap, connections.first[0], connections.second))
                {
                    continue;
                }
            }
            else if(connections.first.size() == 2 && connections.second.size() == 1)
            {            
              // 'cut portal' type non manifold
                if(AttemptToResolveCutPortal(mesh, connectionsMap, connections.second[0], connections.first))
                {
                    continue;
                }
            }

            // doesnt match any template for non manifold resolution
			if(reportNonManifold)
			{
                cAttributesBuilder attributes;
                attributes.add(errorAttributes);
                attributes.add("faceIndex", i);
                Error("NonFatal", "Non-manifold connections at a vertex pair could not be resolved."
                    " All edges connecting to this vertex pair will be disconnected."
					" (Note that only the first such vertex pair per mesh object is reported.)",
                    attributes.get());
				reportNonManifold = false;
			}            
            connectionsMap.disconnectAll(edge.vp(mesh));
        }
    }
}

void
BuildMesh3D(const iFaceVertexMesh& collapsedFilter,
            const iFaceVertexMesh& faceVertex,
            cConnectionsMap connectionsMap,
            const cBitVector& skipFaceFlags,
            std::vector<tEdge>& edgeLookup,
            tMesh& result,
            tSigned32 part, std::vector<std::pair<tSigned32, tSigned32> >& faceMapping
            )
{
    tSigned32 i;

    tSigned32 sizeBefore = result.faces();
    assertD(sizeBefore == SizeL(faceMapping));

    faceMapping.resize(sizeBefore + faceVertex.faces());

    for(i = 0; i < faceVertex.faces(); ++i)
    {
        if(skipFaceFlags[i])
        {
            continue;
        }

        faceMapping[result.faces()] = std::pair<tSigned32, tSigned32>(part, i);

        tMesh::tEdgeData edgeData[3];
        tMesh::tVertexData vertexData[3];
        tSigned32 j;
        for(j = 0; j < 3; ++j)
        {
            tSigned32 v = faceVertex.vertexIndex(i, j);
            vertexData[j]._x = faceVertex.vertexX(v);
            vertexData[j]._y = faceVertex.vertexY(v);
            edgeData[j]._z = faceVertex.vertexZ(v);
        }
        tFace added = result.addFaceAtEnd(tMesh::tFaceData(), 3, edgeData, vertexData);

        tEdge e = tMesh::beginEdge(added);
        for(j = 0; j < 3; ++j, ++e)
        {
            cFaceVertexEdge edgeFV(i, j);
            cFaceVertexEdge twinFV = connectionsMap.twin(edgeFV, collapsedFilter);
            if(twinFV.valid() && twinFV._face < i)
            {
                tEdge twin = edgeLookup[twinFV._face * 3 + twinFV._indexInFace];
                result.connectEdges(twin, e);
            }
        }

        e = tMesh::beginEdge(added);
        for(j = 0; j < 3; ++j, ++e)
        {
            edgeLookup[i * 3 + j] = e;
        }
    }

    faceMapping.resize(result.faces());
}

class cDelaunayFilter3D
{
public:
    bool operator()(tEdge e)
    {
        if(e.face()->isExternal() || e.twin().face()->isExternal())
        {
            return true;
        }
        return false;
    }
};

void
ProcessBuildingConnections(
        const cCollapsedVertexFilter& collapsedFilter,
        const iFaceVertexMesh& faceVertex,
        cConnectionsMap& connectionsMap,
        const std::vector<cFaceVertexEdge>& portals,
        const cBitVector& portalTerrainPortalFlags,
        const std::vector<cFaceVertexEdge>& externalShapeEdgesForPortals,
        std::vector<tEdge>& edgeLookup,
        std::vector<cExternalShapeInfo*>& buildingConnections,
        const char *const* errorAttributes
        )
{
    bool reportNonManifold = true;

    std::vector<tSigned32> lookupByEdge(faceVertex.faces() * 3, -1);
    tSigned32 i;
    for(i = 0; i < SizeL(portals); ++i)
    {
        cFaceVertexEdge e = externalShapeEdgesForPortals[i];
        assertD(lookupByEdge[e._face * 3 + e._indexInFace] == -1);
        lookupByEdge[e._face * 3 + e._indexInFace] = i;
    }

    cBitVector faceProcessed(faceVertex.faces(), false);

    for(i = 0; i < SizeL(portals); ++i)
    {
        cFaceVertexEdge startEdgeFV = externalShapeEdgesForPortals[i];

        if(faceProcessed[startEdgeFV._face])
        {
            continue;
        }
        
        buildingConnections.push_back(new cExternalShapeInfo);
        tMesh& shapeMesh = buildingConnections.back()->_externalShape;

        std::vector<tSigned32> open;
        open.push_back(startEdgeFV._face);
        faceProcessed.setBit(startEdgeFV._face, true);
        do
        {
            tSigned32 f = open.back();
            open.pop_back();

            tMesh::tEdgeData edgeData[3];
            tMesh::tVertexData vertexData[3];
            tSigned32 j;
            for(j = 0; j < 3; ++j)
            {
                tSigned32 v = faceVertex.vertexIndex(f, j);
                vertexData[j]._x = faceVertex.vertexX(v);
                vertexData[j]._y = faceVertex.vertexY(v);
                edgeData[j]._z = faceVertex.vertexZ(v);
            }
            tFace added = shapeMesh.addFace(tMesh::tFaceData(), 3, edgeData, vertexData);

            tEdge e = tMesh::beginEdge(added);
            for(j = 0; j < 3; ++j, ++e)
            {
                cFaceVertexEdge edgeFV(f, j);
                edgeLookup[edgeFV.index()] = e;
                if(connectionsMap.connectionIsNonManifold(edgeFV, collapsedFilter))
                {
                    if(reportNonManifold)
                    {
                        cAttributesBuilder attributes;
                        attributes.add(errorAttributes);
                        attributes.add("faceIndex", f);
                        Error("NonFatal", "Non-manifold connection at a vertex pair in an external shape."
                            " All edges connecting to this vertex pair will be disconnected."
					        " (Note that only the first such vertex pair per mesh object is reported.)",
                            attributes.get());
				        reportNonManifold = false;
                    }
                    connectionsMap.disconnectAll(edgeFV.vp(collapsedFilter));
                }
                cFaceVertexEdge twinFV = connectionsMap.twin(edgeFV, collapsedFilter);
                if(twinFV.valid())
                {
                  // connect if the adjoining face has already been added
                    tEdge twin = edgeLookup[twinFV.index()];
                    if(twin.valid())
                    {
                        shapeMesh.connectEdges(twin, e);
                    }

                  // propagate the fill if the adjoining face has not already been processed (and is not on the open list)
                    if(!faceProcessed[twinFV._face])
                    {
                        open.push_back(twinFV._face);
                        faceProcessed.setBit(twinFV._face, true);
                    }
                }
            }
        }
        while(!open.empty());

        buildingConnections.back()->_shapeExternalFace = AddExternalFace(shapeMesh, edgeLookup[startEdgeFV.index()]);

      // check the shape has no holes
        tEdge e = shapeMesh.beginEdge();
        do
        {
            if(!e.twin().valid())
            {
                break;
            }
        }
        while(e.toNextInMesh());
        if(e.valid())
        {
            cAttributesBuilder attributes;
            attributes.add(errorAttributes);
            attributes.add("faceIndex", startEdgeFV._face);
            Error("NonFatal", "An external shape contains at least one hole, and will be skipped.", attributes.get());
            delete buildingConnections.back();
            buildingConnections.pop_back();
            continue;
        }

        buildingConnections.back()->_shapeFaceIndex = startEdgeFV._face;

        cFaceVertexEdge edgeFV = startEdgeFV;
        do
        {
            tSigned32 portalIndex = lookupByEdge[edgeFV.index()];
            
            if(portalIndex == -1)
            {
                buildingConnections.back()->_insidePortalEdges.push_back(tMesh::endEdge());
                buildingConnections.back()->_portalResolveRanges.push_back(0);
                buildingConnections.back()->_portalResolveRanges.push_back(0);
                buildingConnections.back()->_portalFaceIndices.push_back(-1);
                buildingConnections.back()->_portalResolvesToTerrain.push_back(false);
            }
            else
            {
                cFaceVertexEdge portalFV = portals[portalIndex];
                tEdge portalE = edgeLookup[portalFV.index()];
                buildingConnections.back()->_insidePortalEdges.push_back(portalE);
                float minZ, maxZ;
                collapsedFilter.vertexZRange(portalFV.vertexIndex(collapsedFilter), minZ, maxZ);
                buildingConnections.back()->_portalResolveRanges.push_back(minZ);
                buildingConnections.back()->_portalResolveRanges.push_back(maxZ);
                buildingConnections.back()->_portalFaceIndices.push_back(portalFV._face);
                buildingConnections.back()->_portalResolvesToTerrain.push_back(portalTerrainPortalFlags[portalIndex]);
            }
            buildingConnections.back()->_behindPortalEdges.push_back(tMesh::endEdge());
            buildingConnections.back()->_insideEdgeOnTransformed.push_back(tMesh::endEdge());

            ++edgeFV;
            while(connectionsMap.twin(edgeFV, collapsedFilter).valid())
            {
                edgeFV = connectionsMap.twin(edgeFV, collapsedFilter);
                ++edgeFV;
            }
        }
        while(edgeFV != startEdgeFV);

        
        RemoveInternalVertices(shapeMesh);
        //RemoveInlineEdgeVertices(mesh2D, FlaggedVertexFilter(preserveVertexFlags));
        cDelaunayFilter3D filter;
        //Delaunay(mesh2D, filter);
        SimplifyMesh(shapeMesh, filter);

        buildingConnections.back()->_errorAttributes.addCopies(errorAttributes);
    }

    for(i = 0; i < faceVertex.faces(); ++i)
    {
        if(FaceMarksExternalShape(faceVertex, i) && !faceProcessed[i])
        {
            cAttributesBuilder attributes;
            attributes.add(errorAttributes);
            attributes.add("faceIndex", i);
            Error("NonFatal", "At least one face is marked as an external shape but doesnt connect to a (valid) portal."
					" (Reported once per mesh object.)",
                    attributes.get());
            break;
        }
    }
}

void
ProcessConnectingEdges(
        const cCollapsedVertexFilter& mesh,
        const cConnectionsMap& connectionsMap,
        const std::vector<tEdge>& edgeLookup,
        std::vector<cConnectingEdgeInfo*>& connectingEdges,
        const char *const* errorAttributes
        )
{
    bool reportNonVerticalMarkers = true;

    std::vector<cVertexPair> vertexPairs;

    tSigned32 i;
    for(i = 0; i < mesh.faces(); ++i)
    {
        if(!FaceMarksConnectingEdge(mesh, i))
        {
            continue;
        }

        if(!mesh.faceWasCollapsed(i))
        {
			if(reportNonVerticalMarkers)
			{
                cAttributesBuilder attributes;
                attributes.add(errorAttributes);
                attributes.add("faceIndex", i);
                Error("NonFatal", "A face is marked as a connecting edge,"
                    " but does not contain a vertical edge."
                    " The face will be treated as if it was not marked as a connecting edge."
					" (Note that only the first such face per mesh object is reported.)",
                    attributes.get());
				reportNonVerticalMarkers = false;
			}
            continue;
        }

        tSigned32 v1 = mesh.getCollapsedIndex(i, 0);
        tSigned32 v2 = mesh.getCollapsedIndex(i, 1);
        if(v1 == v2)
        {
            v2 = mesh.getCollapsedIndex(i, 2);
            assertD(v1 != v2);
        }
        cVertexPair vp(v1, v2);

        tSigned32 j;
        for(j = 0; j < SizeL(vertexPairs); ++j)
        {
            if(vertexPairs[j] == vp)
            {
                break;
            }
        }
        if(j == vertexPairs.size())
        {
            vertexPairs.push_back(vp);
        }
    }

    bool reportBadlyFormed = true;
    for(i = 0; i < SizeL(vertexPairs); ++i)
    {
        const tEdgeConnections& connections = connectionsMap.lookupConnections(vertexPairs[i]);
        const std::vector<cFaceVertexEdge>* v;
        bool otherEmpty;
        if(connections.first.empty())
        {
            v = &connections.second;
            otherEmpty = connections.first.empty();
        }
        else
        {
            v = &connections.first;
            otherEmpty = connections.second.empty();
        }
        if(v->size() != 1
         || !otherEmpty
         || FaceMarksExternalShape(mesh, v->front()._face)
         )
        {
			if(reportBadlyFormed)
			{
                cAttributesBuilder attributes;
                attributes.add(errorAttributes);
                attributes.add("vertexIndex1", vertexPairs[i]._v1);
                attributes.add("vertexIndex2", vertexPairs[i]._v2);
                Error("NonFatal", "Badly formed connecting edge marker."
                    " The marker should connect to a single mesh edge."
                    " Badly formed markers will be skipped."
					" (Note that only the first such marker per mesh object is reported.)",
                    attributes.get());
				reportBadlyFormed = false;
			}
            continue;
        }        

        connectingEdges.push_back(new cConnectingEdgeInfo);

        cFaceVertexEdge edgeFV = v->front();
        connectingEdges.back()->_edge = edgeLookup[edgeFV.index()];
        connectingEdges.back()->_resolveRange = ResolveRangeForConnectingEdge(mesh, edgeFV);
        connectingEdges.back()->_errorAttributes.addCopies(errorAttributes);
        connectingEdges.back()->_faceIndex = v->front()._face;
    }
}

static float
FaceIsAboveMaxSlope(const iFaceVertexMesh& mesh, tSigned32 fIndex, float maxSlope)
{
    float normalX, normalY, normalZ;
    normalX = 0.f;
    normalY = 0.f;
    normalZ = 0.f;
    int i;
    tSigned32 v[5];
    v[0] = mesh.vertexIndex(fIndex, 0);
    v[1] = mesh.vertexIndex(fIndex, 1);
    v[2] = mesh.vertexIndex(fIndex, 2);
    v[3] = mesh.vertexIndex(fIndex, 0);
    v[4] = mesh.vertexIndex(fIndex, 1);
    for(i = 0; i < 3; i++)
    {
        float e1X, e1Y, e1Z;
        e1X = static_cast<float>(mesh.vertexX(v[i]) - mesh.vertexX(v[i + 1]));
        e1Y = static_cast<float>(mesh.vertexY(v[i]) - mesh.vertexY(v[i + 1]));
        e1Z = mesh.vertexZ(v[i]) - mesh.vertexZ(v[i + 1]);
        float e2X, e2Y, e2Z;
        e2X = static_cast<float>(mesh.vertexX(v[i + 2]) - mesh.vertexX(v[i + 1]));
        e2Y = static_cast<float>(mesh.vertexY(v[i + 2]) - mesh.vertexY(v[i + 1]));
        e2Z = mesh.vertexZ(v[i + 2]) - mesh.vertexZ(v[i + 1]);
        float vertexNormalX = e1Y * e2Z - e1Z * e2Y; 
        float vertexNormalY = e1Z * e2X - e1X * e2Z; 
        float vertexNormalZ = e1X * e2Y - e1Y * e2X;
        normalX += vertexNormalX; 
        normalY += vertexNormalY; 
        normalZ += vertexNormalZ; 
    }
    float horizontal = SquareRoot(normalX * normalX + normalY * normalY);
    if(normalZ == 0.f)
    {
        return true;
    }
    if(normalZ < 0.f)
    {
        normalZ = -normalZ;
    }
    return horizontal / normalZ > maxSlope;
}
static float
FaceIsVertical(const iFaceVertexMesh& mesh, tSigned32 fIndex)
{
    float normalX, normalY, normalZ;
    normalX = 0.f;
    normalY = 0.f;
    normalZ = 0.f;
    int i;
    tSigned32 v[5];
    v[0] = mesh.vertexIndex(fIndex, 0);
    v[1] = mesh.vertexIndex(fIndex, 1);
    v[2] = mesh.vertexIndex(fIndex, 2);
    v[3] = mesh.vertexIndex(fIndex, 0);
    v[4] = mesh.vertexIndex(fIndex, 1);
    for(i = 0; i < 3; i++)
    {
        float e1X, e1Y, e1Z;
        e1X = static_cast<float>(mesh.vertexX(v[i]) - mesh.vertexX(v[i + 1]));
        e1Y = static_cast<float>(mesh.vertexY(v[i]) - mesh.vertexY(v[i + 1]));
        e1Z = mesh.vertexZ(v[i]) - mesh.vertexZ(v[i + 1]);
        float e2X, e2Y, e2Z;
        e2X = static_cast<float>(mesh.vertexX(v[i + 2]) - mesh.vertexX(v[i + 1]));
        e2Y = static_cast<float>(mesh.vertexY(v[i + 2]) - mesh.vertexY(v[i + 1]));
        e2Z = mesh.vertexZ(v[i + 2]) - mesh.vertexZ(v[i + 1]);
        float vertexNormalX = e1Y * e2Z - e1Z * e2Y; 
        float vertexNormalY = e1Z * e2X - e1X * e2Z; 
        float vertexNormalZ = e1X * e2Y - e1Y * e2X;
        normalX += vertexNormalX; 
        normalY += vertexNormalY; 
        normalZ += vertexNormalZ; 
    }
    float horizontal = SquareRoot(normalX * normalX + normalY * normalY);
    if(normalZ == 0.f)
    {
        return true;
    }
    return false;
}

void
FaceVertexToMesh3D(const iFaceVertexMesh& faceVertex,
                   bool filterBySlope, float maximumSlope,
                   tMesh& result,
                   tSigned32 part, std::vector<std::pair<tSigned32, tSigned32> >& faceMapping,
                   std::vector<cExternalShapeInfo*>& buildingConnections,
                   std::vector<cConnectingEdgeInfo*>& connectingEdges,
                   const char *const* errorAttributes
                   )
{ 
    bool reportedVerticalFace = false;
    cBitVector faceSkipFlags(faceVertex.faces(), false);
    tSigned32 i;
    {
        cVerticalEdgeCollapses collapsesBeforeFilteringBySlope(faceVertex, faceSkipFlags);
        for(i = 0; i < faceVertex.faces(); i++)
        {
            if(collapsesBeforeFilteringBySlope.faceWasCollapsed(i))
            {
                continue;
            }
            if(FaceMarksPortal(faceVertex, i) || FaceMarksConnectingEdge(faceVertex, i))
            {
                // errors are reported for these cases elsewhere
                // the faces are then treated as normal ground
                continue;
            }

            if(filterBySlope)
            {
                if(FaceIsAboveMaxSlope(faceVertex, i, maximumSlope))
                {
                    faceSkipFlags.setBit(i, true);
                }
            }
            else if(FaceIsVertical(faceVertex, i))
            {
                if(!reportedVerticalFace)
                {
                    reportedVerticalFace = true;
                    cAttributesBuilder attributes;
                    attributes.add(errorAttributes);
                    attributes.add("faceIndex", i);
                    Error("NonFatal", "Vertical face does not have vertical edges, and will not be collapsed."
                            " The resulting mesh will not connect across this face."
					        " (Note that only the first such face per mesh object is reported.)",
                            attributes.get());
                }
                faceSkipFlags.setBit(i, true);
            }
        }
    }
    cVerticalEdgeCollapses collapses(faceVertex, faceSkipFlags);

    std::vector<cVertexPair> portalVertexPairs;
    cBitVector portalVertexPairsTerrainPortalFlags;
    GetPortalVertexPairs(faceVertex, collapses, portalVertexPairs, portalVertexPairsTerrainPortalFlags, errorAttributes);
    
    cCollapsedVertexFilter collapsedFilter(faceVertex, collapses);
    cConnectionsMap connectionsMap(collapsedFilter);
    cConnectionsMap connectionsMap_ExternalShapes(collapsedFilter);

    for(i = 0; i < faceVertex.faces(); i++)
    {
        if(collapses.faceWasCollapsed(i))
        {
            faceSkipFlags.setBit(i, true);
        }
    }

    for(i = 0; i < faceVertex.faces(); ++i)
    {
        if(!faceSkipFlags[i])
        {
            if(FaceMarksExternalShape(collapsedFilter, i))
            {
                connectionsMap_ExternalShapes.addFaceConnections(collapsedFilter, i);
            }
            else
            {
                connectionsMap.addFaceConnections(collapsedFilter, i);
            }
        }
    }

    std::vector<cFaceVertexEdge> portals;
    cBitVector portalTerrainPortalFlags;
    std::vector<cFaceVertexEdge> externalShapeEdgesForPortals;
    GetPortals(faceVertex, connectionsMap, connectionsMap_ExternalShapes, portalVertexPairs, portalVertexPairsTerrainPortalFlags, portals, portalTerrainPortalFlags, externalShapeEdgesForPortals, errorAttributes);
    assertD(externalShapeEdgesForPortals.size() == portals.size());

    for(i = 0; i < faceVertex.faces(); i++)
    {
        if(FaceMarksExternalShape(faceVertex, i))
        {
            faceSkipFlags.setBit(i, true);
        }
    }

    ResolveNonManifold(collapsedFilter, faceSkipFlags, connectionsMap, errorAttributes);

    std::vector<tEdge> edgeLookup(faceVertex.faces() * 3, tMesh::endEdge());

    BuildMesh3D(
            collapsedFilter, faceVertex, connectionsMap, faceSkipFlags,
            edgeLookup,
            result,
            part, faceMapping
            );

    ProcessBuildingConnections(
            collapsedFilter, faceVertex,
            connectionsMap_ExternalShapes,
            portals, portalTerrainPortalFlags, externalShapeEdgesForPortals,
            edgeLookup,
            buildingConnections,
            errorAttributes
            );

    ProcessConnectingEdges(collapsedFilter, connectionsMap, edgeLookup, connectingEdges, errorAttributes);
}
