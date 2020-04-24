//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh3D/interface/LoadMesh3D.h"
#include "libs/ParseXML/interface/iXMLHandler.h"
#include "common/STL/vector.h"

class cAdditionalVerticesLoader : public iXMLHandler
{
public:

    std::vector<tSigned32> _targetElements;
    std::vector<tPoint> _points;

// iXMLHandler
    iXMLHandler* element(const char* el, const char *const* attr);
};

class cMappingTo2DLoader : public iXMLHandler
{
    tMesh_3D& _mesh3D;
    tMesh _mesh2DResult;
    cAdditionalVerticesLoader _additionalVertices;
    bool _reportedBadConnection;
    std::vector<tFace> _faces;
    std::vector<tMesh::tVertexData> _vertexData;
    std::vector<tSigned32> _connections;
    std::vector<tMesh::tEdgeData> _edgeData;

public:

    cMappingTo2DLoader(
            tMesh_3D& mesh3D //.. should be const reference
            );

    void swapOutResultAndClear(tMesh& swapOutTo);

// iXMLHandler
    iXMLHandler* element(const char* el, const char *const* attr);
    void elementCompleted(iXMLHandler*);
};

class cMesh3DAndMappingLoader : public iXMLHandler
{
    tMesh_3D _mesh3D;
    tMesh _mesh2D;
    cMesh3DLoader _mesh3DLoader;
    cMappingTo2DLoader _mappingLoader;

public:

    cMesh3DAndMappingLoader();

    // if no mapping was present then mesh2D will be empty
    void swapOutResultAndClear(
        tMesh_3D& mesh3D,
        tMesh& mesh2D
        );

// iXMLHandler
    iXMLHandler* element(const char* el, const char *const* attr);
    void elementCompleted(iXMLHandler* handlerCompleted);
};
