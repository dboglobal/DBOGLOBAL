//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/ParseXML/interface/iXMLHandler.h"
#include "common/STL/vector.h"

class cCutLoader : public iXMLHandler
{
    tMesh& _mesh;
    bool _unconnected;
    bool _loops;
    bool _startCapped;
    tLine _startCap;
    tEdge _entry_SI;

    tFace _startFace;
    std::vector<tLine> _lines;
    std::vector<tLine> _intersects;
	std::vector<char> _hasIntersect;

    cHandle<cMeshCut> _result;

public:
    cCutLoader(tMesh& mesh);

    cHandle<cMeshCut> getResult();

// interface for iXMLHandler

    void init(const char *const* attributes);
    iXMLHandler* element(const char* el, const char *const* attributes);
    void completed();

};

class cCircuitsLoader : public iXMLHandler
{
    cCutLoader _cutLoader;
    cHandle<tMeshCircuits> _result;

public:
    cCircuitsLoader(tMesh& mesh);

    cHandle<tMeshCircuits> getResult();

// interface for iXMLHandler

    void init(const char *const* attributes);
    iXMLHandler* element(const char* el, const char *const* attributes);
    void elementCompleted(iXMLHandler* elementHandler);

};
