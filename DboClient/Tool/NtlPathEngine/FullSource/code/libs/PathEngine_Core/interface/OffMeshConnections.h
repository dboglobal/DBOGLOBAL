//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef OFF_MESH_CONNECTIONS_INCLUDED
#define OFF_MESH_CONNECTIONS_INCLUDED

#include "libs/Mesh2D/interface/InternalPosition.h"
#include "common/STL/vector.h"
#include "common/STL/utility.h"

class iXMLOutputStream;
class iXMLInputStream;

class cOffMeshConnection
{
public:

    tSigned32 fromEndPoint, toEndPoint;
    tSigned32 penalty;
};

class cOffMeshConnections
{
    std::vector<cInternalPosition> _endPoints;
    std::vector<cOffMeshConnection> _connections;
    std::vector<std::vector<tSigned32> > _connectionsPerEndPoint;

    // prevent copying and assignment
    cOffMeshConnections(const cOffMeshConnections&);
    const cOffMeshConnections& operator=(const cOffMeshConnections&);

public:

    cOffMeshConnections()
    {
    }
//    cOffMeshConnections(iXMLInputStream& is, const cMeshIndex<tMesh>& index);

    void save(iXMLOutputStream& os) const;

    void clear()
    {
        _endPoints.clear();
        _connections.clear();
        _connectionsPerEndPoint.clear();
    }
    bool empty() const
    {
        return _endPoints.empty();
    }

    tSigned32 addEndPoint(const cInternalPosition&);
    tSigned32 addConnection(tSigned32 from, tSigned32 to, tSigned32 penalty);

    tSigned32 lookupConnection(tSigned32 from, tSigned32 to) const;

    tSigned32 numberOfEndPoints() const
    {
        return static_cast<tSigned32>(_endPoints.size());
    }
    tSigned32 numberOfConnections() const
    {
        return static_cast<tSigned32>(_connections.size());
    }
    tSigned32 connectionsFromEndPoint(tSigned32 from) const
    {
        return SizeL(_connectionsPerEndPoint[from]);
    }
    void connectionFromEndPoint(tSigned32 from, tSigned32 index, tSigned32& to, tSigned32& penalty) const
    {
        const cOffMeshConnection& c = _connections[_connectionsPerEndPoint[from][index]];
        to = c.toEndPoint;
        penalty = c.penalty;
    }

    const cInternalPosition& refEndPoint(tSigned32 index) const
    {
        return _endPoints[index];
    }

    const std::vector<cInternalPosition>& refEndPoints() const
    {
        return _endPoints;
    }

    const cOffMeshConnection& refConnection(tSigned32 index) const
    {
        return _connections[index];
    }
};

#endif
