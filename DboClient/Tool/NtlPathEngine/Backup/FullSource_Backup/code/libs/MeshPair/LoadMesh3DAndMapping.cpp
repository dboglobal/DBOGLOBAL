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
#include "interface/LoadMesh3DAndMapping.h"
#include "common/TemplateMesh/External.h"
#include "common/TemplateMesh/ElementIndexing.h"
#include "common/interface/Attributes.h"
#include "common/interface/Error.h"
#include "common/AttributesBuilder.h"

iXMLHandler*
cAdditionalVerticesLoader::element(const char* el, const char *const* attr)
{
    if(strcmp(el, "vert") == 0)
    {
        _targetElements.push_back(ValueAsLong(attr, "targetElement"));
        tSigned32 x = ValueAsLong(attr, "x");
        tSigned32 y = ValueAsLong(attr, "y");
        _points.push_back(tPoint(x, y, WORLD_RANGE));
    }
    return 0;
}

cMappingTo2DLoader::cMappingTo2DLoader(tMesh_3D& mesh3D) :
 _mesh3D(mesh3D)
{
    _reportedBadConnection = false;
}

void
cMappingTo2DLoader::swapOutResultAndClear(tMesh& swapOutTo)
{
    AddExternalFaces(_mesh2DResult);
    assertD(swapOutTo.empty());
    swapOutTo.swap(_mesh2DResult);
}

iXMLHandler*
cMappingTo2DLoader::element(const char* el, const char *const* attr)
{
    if(getDepth() == 0)
    {
        if(strcmp(el, "verts") == 0)
        {
            return &_additionalVertices;
        }
        if(strcmp(el, "poly") == 0)
        {
            assertD(_vertexData.empty());
            return this;
        }
        return 0;
    }
    if(strcmp(el, "edge") != 0)
    {
        return 0;
    }
    tSigned32 v = ValueAsLong(attr, "startVert");
    if(v < _mesh3D.vertices())
    {
        _vertexData.push_back(tMesh::tVertexData());
        _vertexData.back()._p = GetVertexPoint(_mesh3D.vertAt(v));
        _vertexData.back().setElementOn3DIndex(ElementIndexForTypeAndSubIndex(bMeshElement::VERTEX, v));
    }
    else
    {
        tSigned32 v2 = v - _mesh3D.vertices();
        _vertexData.push_back(tMesh::tVertexData());
        _vertexData.back()._p = _additionalVertices._points[v2];
        _vertexData.back().setElementOn3DIndex(_additionalVertices._targetElements[v2]);
    }
    _connections.push_back(ValueAsLongWithDefault(attr, "connection", -1));
    return 0;
}

static tEdge
GetConnectingEdge(tEdge target, tFace f)
{
    tEdge e = tMesh::beginEdge(f);
    do
    {
        if(*e.vertex() == *target.next().vertex()
         && *e.next().vertex() == *target.vertex()
         )
        {
            return e;
        }
    }
    while(e.toNextInFace());
    return e;
}
void
cMappingTo2DLoader::elementCompleted(iXMLHandler* handlerCompleted)
{
    if(getDepth() != 0)
    {
        return;
    }
    if(handlerCompleted != this)
    {
      // completed loading verts
        return;
    }

    assertD(!_vertexData.empty());
    tSigned32 edges = SizeL(_vertexData);
    _edgeData.resize(edges);

    tFace f = _mesh2DResult.addFaceAtEnd(tMesh::tFaceData(), edges, &_edgeData.front(), &_vertexData.front());

    tSigned32 i;
    tEdge e = tMesh::beginEdge(f);
    for(i = 0; i != edges; ++i, ++e)
    {
        tSigned32 connection = _connections[i];
        if(connection < -1 || connection >= SizeL(_faces))
        {
            if(!_reportedBadConnection)
            {
                cAttributesBuilder attributes;
                attributes.add("errorcode", "10");
                attributes.add("polygonindex", SizeL(_faces));
                attributes.add("edgeindexinpolygon", i);
                attributes.add("connection", connection);
                Error("NonFatal","Loading mapping to 2d from xml: bad value for polygon connection, ignored."
                    " Only one connection error reported per mesh.",
                    attributes.get());
                _reportedBadConnection = true;
            }
        }
        else if(connection != -1)
        {
            tEdge connectTo = GetConnectingEdge(e, _faces[connection]);
            if(!connectTo.valid())
            {
                if(!_reportedBadConnection)
                {
                    cAttributesBuilder attributes;
                    attributes.add("polygonindex", SizeL(_faces));
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
                _mesh2DResult.connectEdges(e, connectTo);
            }
        }
    }
    _faces.push_back(f);

    _vertexData.clear();
    _connections.clear();
}

cMesh3DAndMappingLoader::cMesh3DAndMappingLoader() :
 _mappingLoader(_mesh3D)
{
}

void
cMesh3DAndMappingLoader::swapOutResultAndClear(
        tMesh_3D& mesh3D,
        tMesh& mesh2D
        )
{
    assertD(mesh3D.empty());
    _mesh3D.swap(mesh3D);
    assertD(mesh2D.empty());
    _mesh2D.swap(mesh2D);
}

iXMLHandler*
cMesh3DAndMappingLoader::element(const char* el, const char *const* attr)
{
    if(strcmp(el, "mesh3D") == 0)
    {
        return &_mesh3DLoader;
    }
    if(strcmp(el, "mappingTo2D") == 0)
    {
        assertD(!_mesh3D.empty());
        return &_mappingLoader;
    }
    return 0;
}
void
cMesh3DAndMappingLoader::elementCompleted(iXMLHandler* completedHandler)
{
    if(completedHandler == &_mesh3DLoader)
    {
        _mesh3DLoader.swapOutResultAndClear(_mesh3D);
        _mesh3D.consolidate();
        _mesh3D.normaliseInternalVertices();
        AddExternalFaces(_mesh3D);
        _mesh3D.consolidate();
    }
    else
    if(completedHandler == &_mappingLoader)
    {
        _mappingLoader.swapOutResultAndClear(_mesh2D);
    }
}
