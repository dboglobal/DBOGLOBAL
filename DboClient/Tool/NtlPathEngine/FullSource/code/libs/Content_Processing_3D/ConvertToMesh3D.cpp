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
#include "ConvertToMesh3D.h"
#include "tMesh_Dynamic.h"
#include "libs/Mesh3D/interface/PlaneEquation.h"
#include "libs/Mesh3D/interface/ValidateMesh_Import.h"
#include "common/TemplateMesh/External.h"
#include "common/interface/Assert.h"

void
ConvertToMesh3D(const cBSPVertices& vertices, tMesh_Dynamic& from, tMesh_3D& to)
{
    assertD(to.empty());
    assertD(!from.empty());

    to.copyStructure(from);

    tVertex_Dynamic fromV = from.beginVertex();
    tVertex_3D toV = to.beginVertex();
    do
    {
        const cDynamicPoint3& p = fromV->dynamicPoint(vertices);
        toV->_x = p.x().asLong();
        assertD(toV->_x == p.x());
        toV->_y = p.y().asLong();
        assertD(toV->_y == p.y());
        ++toV;
    }
    while((++fromV).valid());

    tEdge_Dynamic fromE = from.beginEdge();
    tEdge_3D toE = to.beginEdge();
    do
    {
        toE->_z = static_cast<float>(fromE->_approximateZ);
        toE.toNextInMesh();
    }
    while(fromE.toNextInMesh());

    to.consolidate(); 
    AddExternalFaces(to);
    to.consolidate(); 
    assertD(ValidateMesh_Import(to));
    CalculatePlaneEquations(to);
}

