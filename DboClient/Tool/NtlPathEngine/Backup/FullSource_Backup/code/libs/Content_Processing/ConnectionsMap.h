//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/interface/Assert.h"
#include "FaceVertexEdge.h"
#include "common/Containers/BitVector.h"
#include "common/STL/utility.h"
#include "common/STL/vector.h"
#include "common/STL/map.h"

typedef std::pair<std::vector<cFaceVertexEdge>, std::vector<cFaceVertexEdge> > tEdgeConnections;

class cConnectionsMap
{    
    std::vector<cFaceVertexEdge> _noConnectionsVector;
    cBitVector _edgeIsConnected;
    mutable std::map<cVertexPair, tEdgeConnections> _map;

public:

    cConnectionsMap(const iFaceVertexMesh& mesh);

    void addFaceConnections(const iFaceVertexMesh& mesh, tSigned32 faceIndex);

    void disconnectAll(const cVertexPair& vp);
    void disconnectSingle(const cFaceVertexEdge& toDisconnect, const iFaceVertexMesh& mesh);
    
    const tEdgeConnections& lookupConnections(const cVertexPair& vp) const
    {
        return _map[vp];
    }
//    const std::vector<cFaceVertexEdge>& lookupReturnConnections(const cFaceVertexEdge& edge, const iFaceVertexMesh& mesh) const;

    bool connectionIsNonManifold(const cFaceVertexEdge& edge, const iFaceVertexMesh& mesh) const;
    cFaceVertexEdge twin(const cFaceVertexEdge& edge, const iFaceVertexMesh& mesh) const;
};
