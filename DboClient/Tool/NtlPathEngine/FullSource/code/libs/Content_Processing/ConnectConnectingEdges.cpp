//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "ConnectConnectingEdges.h"
#include "ConnectingEdgeInfo.h"
#include "common/Containers/BitVector.h"
#include "common/interface/Error.h"

typedef tMesh_3D_FaceTracked tMesh;
typedef tFace_3D_FaceTracked tFace;
typedef tEdge_3D_FaceTracked tEdge;
typedef tVertex_3D_FaceTracked tVertex;

static void
AddBridgingTri(tMesh& mesh, tEdge edge, tEdge vertex)
{
    tMesh::tEdgeData edgeData[3];
    tMesh::tVertexData vertexData[3];
    vertexData[0] = *edge.next().vertex();
    vertexData[1] = *edge.vertex();
    vertexData[2] = *vertex.vertex();
    edgeData[0] = *edge.next();
    edgeData[1] = *edge;
    edgeData[2] = *vertex;
    tFace added = mesh.addFace(*edge.face(), 3, edgeData, vertexData);
    mesh.connectEdges(edge, tMesh::beginEdge(added));
}

static bool
BridgeEdges(tMesh& mesh, tEdge edge1, tEdge edge2)
{
    if(GetVertexPoint(edge1) == GetVertexPoint(edge2.next()) && GetVertexPoint(edge2) == GetVertexPoint(edge1.next()))
    {
      // edges already match exactly
        mesh.connectEdges(edge1, edge2);
        return true;
    }
    if(GetEdgeLine(edge1).side(GetVertexPoint(edge2)) == SIDE_LEFT
     && GetEdgeLine(edge2).side(GetVertexPoint(edge1)) == SIDE_LEFT
     )
    {
        AddBridgingTri(mesh, edge1, edge2);
        AddBridgingTri(mesh, edge2, edge1);
        mesh.connectEdges(edge1.twin().next(), edge2.twin().next());
        return true;
    }
    if(GetEdgeLine(edge1).side(GetVertexPoint(edge2.next())) == SIDE_LEFT
     && GetEdgeLine(edge2).side(GetVertexPoint(edge1.next())) == SIDE_LEFT
     )
    {
        AddBridgingTri(mesh, edge1, edge2.next());
        AddBridgingTri(mesh, edge2, edge2.next());
        mesh.connectEdges(edge1.twin().prev(), edge2.twin().prev());
        return true;
    }
    return false;
}

void
ConnectConnectingEdges(
        tMesh_3D_FaceTracked& baseMesh,
        const std::vector<cConnectingEdgeInfo*>& connectingEdges
        )
{
    cBitVector alreadyResolved(SizeL(connectingEdges), false);
    tSigned32 i;
    for(i = 0; i < SizeL(connectingEdges); ++i)
    {
        if(alreadyResolved[i])
        {
            continue;
        }
        cConnectingEdgeInfo& info = *connectingEdges[i];
        tSigned32 resolveRangeL = static_cast<tSigned32>(info._resolveRange);

        tPoint bottomLeft_Start = GetVertexPoint(info._edge);
        tPoint topRight_Start = bottomLeft_Start;
        bottomLeft_Start -= tPoint(resolveRangeL, resolveRangeL, resolveRangeL);
        topRight_Start += tPoint(resolveRangeL, resolveRangeL, resolveRangeL);

        tPoint bottomLeft_End = GetVertexPoint(info._edge.next());
        tPoint topRight_End = bottomLeft_End;
        bottomLeft_End -= tPoint(resolveRangeL, resolveRangeL, resolveRangeL);
        topRight_End += tPoint(resolveRangeL, resolveRangeL, resolveRangeL);

        float zStart = info._edge->_z - info._resolveRange;
        float zEnd = info._edge->_z + info._resolveRange;

        tSigned32 found = -1;
        tSigned32 j;
        for(j = i + 1; j < SizeL(connectingEdges); ++j)
        {
            if(GetVertexPoint(connectingEdges[j]->_edge.next()).insideBounds(bottomLeft_Start, topRight_Start)
             && GetVertexPoint(connectingEdges[j]->_edge).insideBounds(bottomLeft_End, topRight_End)
             && connectingEdges[j]->_edge->_z >= zStart && connectingEdges[j]->_edge->_z <= zEnd
             && connectingEdges[j]->_edge.next()->_z >= zStart && connectingEdges[j]->_edge.next()->_z <= zEnd
             )
            {
                if(found != -1)
                {
                    found = -1;
                    break;
                }
                found = j;
            }
        }
        if(found == -1 || connectingEdges[found]->_edge.twin().valid())
        {
            cAttributesBuilder attributes;
            attributes.add(info._errorAttributes.get());
            attributes.add("faceIndex", info._faceIndex);
            attributes.add("portalStartX", GetVertexPoint(info._edge).getX());
            attributes.add("portalStartY", GetVertexPoint(info._edge).getY());
            attributes.add("portalEndX", GetVertexPoint(info._edge.next()).getX());
            attributes.add("portalEndY", GetVertexPoint(info._edge.next()).getY());
	        if(found != -1)
			{
	            Error("NonFatal", "Failed to resolve target for a connecting edge."
		            " (A target edge was found, but has already been resolved to another edge.)"
			        " The edge will not be connected.",
				    attributes.get());
			}
			else if(j == connectingEdges.size())
            {
                Error("NonFatal", "Failed to resolve target for a connecting edge."
                    " (No target edge found within resolve range.)"
                    " The edge will not be connected.",
                    attributes.get());
            }
            else
            {
                Error("NonFatal", "Failed to resolve target for a connecting edge."
                    " (Multiple target edges found within resolve range.)"
                    " The edge will not be connected.",
                    attributes.get());
            }
            continue;
        }
        if(BridgeEdges(baseMesh, info._edge, connectingEdges[found]->_edge))
        {
            alreadyResolved.setBit(found, true);
        }
        else
        {
            cAttributesBuilder attributes;
            attributes.add(info._errorAttributes.get());
            attributes.add("faceIndex1", info._faceIndex);
            attributes.add("faceIndex2", connectingEdges[found]->_faceIndex);
            Error("NonFatal", "Failed to create a bridge for connecting edges."
                " The edges will not be connected.",
                attributes.get());
            continue;            
        }
    }
}
