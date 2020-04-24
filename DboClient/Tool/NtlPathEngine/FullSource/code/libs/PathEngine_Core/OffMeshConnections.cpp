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
#include "interface/OffMeshConnections.h"
#include "libs/Mesh2D/interface/Mesh2DPersistence.h"
#include "common/interface/Attributes.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/interface/iXMLInputStream.h"

// ** not currently used **
//cOffMeshConnections::cOffMeshConnections(iXMLInputStream& is, const cMeshIndex<tMesh>& index)
//{
//    const char* next;
//    is.openElement("endPoints");
//    next = is.peekNextElement();
//    while(next && strcmp(next, "endPoint") == 0)
//    {
//        const char *const* attributes = is.openNextElement();
//        const char* value = GetValueForAttribute(attributes, "position");
//        _endPoints.push_back(PositionFromString(index, value));
//        is.closeElement();
//        next = is.peekNextElement();
//    }
//    is.closeElement();
//    is.openElement("connections");
//    next = is.peekNextElement();
//    while(next && strcmp(next, "connection") == 0)
//    {
//        _connections.push_back(cOffMeshConnection());
//        const char *const* attributes = is.openNextElement();
//        _connections.back().fromEndPoint = ValueAsLong(attributes, "from");
//        _connections.back().toEndPoint = ValueAsLong(attributes, "to");
//        _connections.back().penalty = ValueAsLong(attributes, "penalty");
//        is.closeElement();
//        next = is.peekNextElement();
//    }
//    is.closeElement();
//
//  // _connectionsPerEndPoint is rebuilt
//    _connectionsPerEndPoint.resize(_endPoints.size());
//    tSigned32 i;
//    for(i = 0; i != SizeL(_connections); ++i)
//    {
//        _connectionsPerEndPoint[_connections[i].fromEndPoint].push_back(i);
//    }
//}

void
cOffMeshConnections::save(iXMLOutputStream& os) const
{
    os.openElement("endPoints");
    tSigned32 i;
    for(i = 0; i != SizeL(_endPoints); ++i)
    {
        os.openElement("endPoint");
        std::string asString = PositionToString(_endPoints[i]);
        os.addAttribute("position", asString.c_str());
        os.closeElement("endPoint");
    }
    os.closeElement("endPoints");
    os.openElement("connections");
    for(i = 0; i != SizeL(_connections); ++i)
    {
        os.openElement("connection");
        os.addAttribute("from", _connections[i].fromEndPoint);
        os.addAttribute("to", _connections[i].toEndPoint);
        os.addAttribute("penalty", _connections[i].penalty);
        os.closeElement("connection");
    }
    os.closeElement("connections");
}

tSigned32
cOffMeshConnections::addEndPoint(const cInternalPosition& position)
{
    tSigned32 index = static_cast<tSigned32>(_endPoints.size());
    _endPoints.push_back(position);
    _connectionsPerEndPoint.push_back(std::vector<tSigned32>());
    return index;
}

tSigned32
cOffMeshConnections::addConnection(tSigned32 from, tSigned32 to, tSigned32 penalty)
{
    tSigned32 index = static_cast<tSigned32>(_connections.size());
    _connections.push_back(cOffMeshConnection());
    _connections.back().fromEndPoint = from;
    _connections.back().toEndPoint = to;
    _connections.back().penalty = penalty;
    _connectionsPerEndPoint[from].push_back(index);
    return index;
}

tSigned32
cOffMeshConnections::lookupConnection(tSigned32 from, tSigned32 to) const
{
    const std::vector<tSigned32>& v = _connectionsPerEndPoint[from];
    size_t i;
    for(i = 0; i < v.size(); i++)
    {
        const cOffMeshConnection& connection = _connections[v[i]];
        if(connection.toEndPoint == to)
        {
            return v[i];
        }
    }
    return -1;
}
