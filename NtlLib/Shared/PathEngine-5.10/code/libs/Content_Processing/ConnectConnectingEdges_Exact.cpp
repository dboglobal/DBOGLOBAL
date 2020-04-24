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
#include "ConnectConnectingEdges_Exact.h"
#include "ConnectingEdgeInfo.h"
#include "common/Containers/BitVector.h"
#include "common/interface/Error.h"

typedef tMesh_3D_FaceTracked tMesh;
typedef tFace_3D_FaceTracked tFace;
typedef tEdge_3D_FaceTracked tEdge;
typedef tVertex_3D_FaceTracked tVertex;

void
ConnectConnectingEdges_Exact(
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
        tPoint startP = GetVertexPoint(info._edge);
        tPoint endP = GetVertexPoint(info._edge.next());

        tSigned32 found = -1;
        tSigned32 j;
        for(j = i + 1; j < SizeL(connectingEdges); ++j)
        {
            if(GetVertexPoint(connectingEdges[j]->_edge.next()) == startP
             && GetVertexPoint(connectingEdges[j]->_edge) == endP
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
        if(found == -1)
        {
            continue;
        }
		if(connectingEdges[found]->_edge.twin().valid())
		{
            cAttributesBuilder attributes;
            attributes.add(info._errorAttributes.get());
            attributes.add("faceIndex", info._faceIndex);
            attributes.add("portalStartX", GetVertexPoint(info._edge).getX());
            attributes.add("portalStartY", GetVertexPoint(info._edge).getY());
            attributes.add("portalEndX", GetVertexPoint(info._edge.next()).getX());
            attributes.add("portalEndY", GetVertexPoint(info._edge.next()).getY());
            Error("NonFatal", "Failed to resolve target for a connecting edge."
                " (A target edge was found, but has already been resolved to another edge.)"
                " The edge will not be connected.",
                attributes.get());
			continue;
		}
        baseMesh.connectEdges(info._edge, connectingEdges[found]->_edge);
        alreadyResolved.setBit(found, true);
    }
}
