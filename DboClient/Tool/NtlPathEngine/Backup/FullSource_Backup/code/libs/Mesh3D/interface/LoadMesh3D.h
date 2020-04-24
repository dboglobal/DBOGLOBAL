//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/ParseXML/interface/iXMLHandler.h"
#include "common/STL/vector.h"

class cMesh3DVertexLoader : public iXMLHandler
{
    tMesh_3D& _mesh;

public:

    cMesh3DVertexLoader(tMesh_3D& mesh);

    std::vector<tMesh_3D::vertex> _vertices;
    std::vector<float> _heights;

// iXMLHandler
    iXMLHandler* element(const char* el, const char *const* attr);
};


class cMesh3DLoader : public iXMLHandler
{
    cMesh3DVertexLoader _vertexLoader;
    tMesh_3D _meshResult;
    bool _reportedBadConnection;

public:

    cMesh3DLoader();
    void swapOutResultAndClear(tMesh_3D& swapOutTo);

// iXMLHandler
    iXMLHandler* element(const char* el, const char *const* attr);
};
