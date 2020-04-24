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
#include "ConnectionsMap.h"
#include "VerticalEdgeCollapses.h"
#include "i_pathengine.h"

tSigned32
cFaceVertexEdge::vertexIndex(const iFaceVertexMesh& mesh) const
{
    return mesh.vertexIndex(_face, _indexInFace);
}

cConnectionsMap::cConnectionsMap(const iFaceVertexMesh& mesh) :
 _edgeIsConnected(mesh.faces() * 3, false)
{
}

void
cConnectionsMap::addFaceConnections(const iFaceVertexMesh& mesh, tSigned32 faceIndex)
{
    tSigned32 i;
    for(i = 0; i < 3; i++)
    {
        cFaceVertexEdge e(faceIndex, i);

        assertD(!_edgeIsConnected[e.index()]);
        _edgeIsConnected.setBit(e.index(), true);

        cVertexPair key = e.vp(mesh);
        if(e.ownsVP(mesh))
        {
            _map[key].first.push_back(e);
        }
        else
        {
            _map[key].second.push_back(e);
        }
    }
}

void
cConnectionsMap::disconnectAll(const cVertexPair& vp)
{
    tEdgeConnections& connections = _map[vp];
    tSigned32 i;
    for(i = 0; i < SizeL(connections.first); ++i)
    {
        assertD(_edgeIsConnected[connections.first[i].index()]);
        _edgeIsConnected.setBit(connections.first[i].index(), false);
    }
    connections.first.clear();
    for(i = 0; i < SizeL(connections.second); ++i)
    {
        assertD(_edgeIsConnected[connections.second[i].index()]);
        _edgeIsConnected.setBit(connections.second[i].index(), false);
    }
    connections.second.clear();
}

void
cConnectionsMap::disconnectSingle(const cFaceVertexEdge& toDisconnect, const iFaceVertexMesh& mesh)
{
    assertD(_edgeIsConnected[toDisconnect.index()]);
    _edgeIsConnected.setBit(toDisconnect.index(), false);
    tSigned32 i;
    tEdgeConnections& connections = _map[toDisconnect.vp(mesh)];
    if(toDisconnect.ownsVP(mesh))
    {
        for(i = 0; i < SizeL(connections.first); ++i)
        {
            if(connections.first[i] == toDisconnect)
            {
                connections.first[i] = connections.first.back();
                connections.first.pop_back();
                return;
            }
        }
    }
    else
    {
        for(i = 0; i < SizeL(connections.second); ++i)
        {
            if(connections.second[i] == toDisconnect)
            {
                connections.second[i] = connections.second.back();
                connections.second.pop_back();
                return;
            }
        }
    }
    invalid();
}

//const std::vector<cFaceVertexEdge>&
//cConnectionsMap::lookupReturnConnections(const cFaceVertexEdge& edge, const iFaceVertexMesh& mesh) const
//{
//    const tEdgeConnections& connections = lookupConnections(edge.vp(mesh));
//    tSigned32 i;
//    for(i = 0; i < connections.first.size(); ++i)
//    {
//        if(connections.first[i] == edge)
//        {
//            return connections.second;
//        }
//    }
//    for(i = 0; i < connections.second.size(); ++i)
//    {
//        if(connections.second[i] == edge)
//        {
//            return connections.first;
//        }
//    }
//  // the edge got disconnected from it's vertex pair
//    return _noConnectionsVector;
//}

bool
cConnectionsMap::connectionIsNonManifold(const cFaceVertexEdge& edge, const iFaceVertexMesh& mesh) const
{
    if(!_edgeIsConnected[edge.index()])
    {
        return false;
    }
    const tEdgeConnections& connections = lookupConnections(edge.vp(mesh));
    return connections.first.size() > 1 || connections.second.size() > 1;
}

cFaceVertexEdge
cConnectionsMap::twin(const cFaceVertexEdge& edge, const iFaceVertexMesh& mesh) const
{
    assertD(!connectionIsNonManifold(edge, mesh));
    if(!_edgeIsConnected[edge.index()])
    {
        return cFaceVertexEdge();
    }
    tEdgeConnections& connections = _map[edge.vp(mesh)];
    if(edge.ownsVP(mesh))
    {
        if(connections.second.empty())
        {
            return cFaceVertexEdge();
        }
        return connections.second.front();
    }
    else
    {
        if(connections.first.empty())
        {
            return cFaceVertexEdge();
        }
        return connections.first.front();
    }
}
