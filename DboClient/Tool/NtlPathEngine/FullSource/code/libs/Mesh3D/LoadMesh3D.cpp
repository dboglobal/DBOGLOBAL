//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/LoadMesh3D.h"
#include "platform_common/CoordinateRanges.h"
#include "common/AttributesBuilder.h"
#include "common/interface/Attributes.h"
#include "common/interface/Error.h"
#include "common/TemplateMesh/ValidateMeshConnections.h"

cMesh3DVertexLoader::cMesh3DVertexLoader(tMesh_3D& mesh) :
 _mesh(mesh)
{
}

iXMLHandler*
cMesh3DVertexLoader::element(const char* el, const char *const* attr)
{
    if(strcmp(el, "vert") == 0)
    {
        tSigned32 x = ValueAsLong(attr, "x");
        tSigned32 y = ValueAsLong(attr, "y");
        _vertices.push_back(_mesh.addUnconnectedVertex(tMesh_3D::tVertexData(x, y)));
        _heights.push_back(ValueAsPersistentFloatWithDefault(attr, "z", 0.f));
    }
    return 0;
}

cMesh3DLoader::cMesh3DLoader() :
 _vertexLoader(_meshResult)
{
    _reportedBadConnection = false;
}
void
cMesh3DLoader::swapOutResultAndClear(tMesh_3D& swapOutTo)
{
    tEdge_3D e = _meshResult.beginEdge();
    while(e.valid())
    {
        _meshResult.initialiseEdgeVertexIfUninitialised(e);
        e.toNextInMesh();
    }
    _meshResult.eraseUninitialisedVertices();
    _meshResult.fixEdgesForAnyMultiplyAssignedVerts();
    assertD(ValidateMeshConnections(_meshResult));
    assertD(swapOutTo.empty());
    _meshResult.swap(swapOutTo);
    _reportedBadConnection = false;
}

static tEdge_3D
GetConnectingEdge(tEdge_3D target, tFace_3D f)
{
    tEdge_3D e = tMesh_3D::beginEdge(f);
    do
    {
        if(e.vertex() == target.next().vertex()
         && e.next().vertex() == target.vertex()
         )
        {
            return e;
        }
    }
    while(e.toNextInFace());
    return e;
}

iXMLHandler*
cMesh3DLoader::element(const char* el, const char *const* attr)
{
    if(getDepth() == 0)
    {
        if(strcmp(el, "verts") == 0)
        {
            return &_vertexLoader;
        }
        if(strcmp(el, "tris") == 0)
        {
            return this;
        }
        return 0;
    }
    if(strcmp(el, "tri") != 0)
    {
        return 0;
    }

    tMesh_3D::tEdgeData edgeData[3];
    tVertex_3D vertices[3];
    tSigned32 connections[3];
    tSigned32 v;

    v = ValueAsLong(attr, "edge0StartVert");
    vertices[0] = _vertexLoader._vertices[v];
    edgeData[0]._z = ValueAsPersistentFloatWithDefault(attr, "edge0StartZ", _vertexLoader._heights[v]);
    connections[0] = ValueAsLongWithDefault(attr, "edge0Connection", -1);

    v = ValueAsLong(attr, "edge1StartVert");
    vertices[1] = _vertexLoader._vertices[v];
    edgeData[1]._z = ValueAsPersistentFloatWithDefault(attr, "edge1StartZ", _vertexLoader._heights[v]);
    connections[1] = ValueAsLongWithDefault(attr, "edge1Connection", -1);

    v = ValueAsLong(attr, "edge2StartVert");
    vertices[2] = _vertexLoader._vertices[v];
    edgeData[2]._z = ValueAsPersistentFloatWithDefault(attr, "edge2StartZ", _vertexLoader._heights[v]);
    connections[2] = ValueAsLongWithDefault(attr, "edge2Connection", -1);

    tFace_3D f = _meshResult.addFaceAtEnd_LeaveVertsUninitialised(tMesh_3D::tFaceData(), 3, edgeData, vertices);
    tSigned32 i;
    tEdge_3D e = tMesh_3D::beginEdge(f);
    for(i = 0; i != 3; ++i, ++e)
    {
        tSigned32 connection = connections[i];
        if(connection < -1 || connection >= _meshResult.faces() - 1)
        {
            if(!_reportedBadConnection)
            {
                cAttributesBuilder attributes;
                attributes.add("errorcode", "10");
                attributes.add("polygonindex", _meshResult.faces() - 1);
                attributes.add("edgeindexinpolygon", i);
                attributes.add("connection", connection);
                Error("NonFatal","Loading mesh from xml: bad value for polygon connection, ignored."
                    " Only one connection error reported per mesh.",
                    attributes.get());
                _reportedBadConnection = true;
            }
        }
        else if(connection != -1)
        {
            tEdge_3D connectTo = GetConnectingEdge(e, _meshResult.faceAt(connection));
            if(!connectTo.valid())
            {
                if(!_reportedBadConnection)
                {
                    cAttributesBuilder attributes;
                    attributes.add("polygonindex", _meshResult.faces() - 1);
                    attributes.add("edgeindexinpolygon", i);
                    attributes.add("connection", connection);
                    Error("NonFatal", "Could not make connection,"
                        " connection is not reciprocated or vertices do not match across the connection."
                        " This connection will be skipped."
                        " Only one connection error reported per mesh.",
                        attributes.get());
                    _reportedBadConnection = true;
                }
            }
            else
            {
                _meshResult.connectEdges_LeaveVertsUninitialised(e, connectTo);
            }
        }
    }
    return 0;
}
