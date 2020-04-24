//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/ExtractConnectingEdgeAndTerrainPortalInfo.h"
#include "VerticalEdgeCollapses.h"
#include "CollapsedVertexFilter.h"
#include "FaceVertexEdge.h"
#include "FaceVertexToMesh3D.h"
#include "i_pathengine.h"

void
ExtractConnectingEdgeAndTerrainPortalInfo(
        const iFaceVertexMesh& ground,
        std::vector<tSigned32>& connectingEdgeVertices,
        std::vector<float>& connectingEdgeResolveRanges,
        std::vector<tSigned32>& terrainPortalInfo
        )
{
    cBitVector faceSkipFlags(ground.faces(), false);
    cVerticalEdgeCollapses collapses(ground, faceSkipFlags);
    cCollapsedVertexFilter collapsedFilter(ground, collapses);

    cBitVector collapsedVertexIsOnConnectingEdge(collapsedFilter.vertices(), false);
    cBitVector collapsedVertexIsOnTerrainPortal(collapsedFilter.vertices(), false);

    for(tSigned32 i = 0; i != ground.faces(); ++i)
    {
        if(ground.faceAttribute(i, PE_FaceAttribute_MarksConnectingEdge) == 1)
        {
            for(tSigned32 j = 0; j != 3; ++j)
            {
                tSigned32 v = collapses.getCollapsedIndex(ground.vertexIndex(i, j));
                collapsedVertexIsOnConnectingEdge.setBit(v, true);
            }
        }
        else if(ground.faceAttribute(i, PE_FaceAttribute_MarksTerrainPortal) == 1)
        {
            for(tSigned32 j = 0; j != 3; ++j)
            {
                tSigned32 v = collapses.getCollapsedIndex(ground.vertexIndex(i, j));
                collapsedVertexIsOnTerrainPortal.setBit(v, true);
            }
        }
    }

    for(tSigned32 i = 0; i != collapsedFilter.faces(); ++i)
    {
        if(collapsedFilter.faceWasCollapsed(i))
        {
            continue;
        }
        if(ground.faceAttribute(i, PE_FaceAttribute_MarksExternalShape) == 1)
        {
            continue;
        }
        for(tSigned32 j = 0; j != 3; ++j)
        {
            cFaceVertexEdge edgeFV(i, j);
            tSigned32 v = edgeFV.vertexIndex(collapsedFilter);
            tSigned32 nextV = edgeFV.next().vertexIndex(collapsedFilter);
            if(collapsedVertexIsOnConnectingEdge[v] && collapsedVertexIsOnConnectingEdge[nextV])
            {
                connectingEdgeVertices.push_back(edgeFV.vertexIndex(ground));
                connectingEdgeVertices.push_back(edgeFV.next().vertexIndex(ground));
                connectingEdgeResolveRanges.push_back(ResolveRangeForConnectingEdge(collapsedFilter, edgeFV));
            }
            else if(collapsedVertexIsOnTerrainPortal[v] && collapsedVertexIsOnTerrainPortal[nextV])
            {
                terrainPortalInfo.push_back(i);
                terrainPortalInfo.push_back(j);
            }
        }        
    }
}
