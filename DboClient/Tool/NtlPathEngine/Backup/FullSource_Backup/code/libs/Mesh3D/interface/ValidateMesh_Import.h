//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "common/TemplateMesh/Helper.h"
#include "common/interface/Error.h"
#include "common/AttributesBuilder.h"
#include "common/STL/vector.h"
#include "common/STL/utility.h"

template <class tMesh> bool
ValidateMesh_Import_AllowDeadEntries(
        tMesh &mesh,
        const std::vector<std::pair<tSigned32, tSigned32> >* faceMappings = 0
        )
{
    cAttributesBuilder attributes;
    typename tMesh::edge e;
    typename tMesh::face f;

    if(mesh.empty())
    {
        Error("NonFatal", "Mesh Validation: Mesh must contain at least one polygon. Validation fails.",attributes.get());
        return false;
    }

    // external faces must all be at end of face ordering
    // asserts are used here, because external faces are added after import
    f = mesh.beginFace();
    while(1)
    {
        assertD(f.valid());
        if(f->isExternal())
        {
            ++f;
            break;
        }
        ++f;
    }
    while(f.valid())
    {
        assertD(f->isExternal());
        ++f;
    }

    e=mesh.beginEdge();
    do
    {
        tSigned32 x,y;
        tPoint p=GetVertexPoint(e);
        x=p.getX();
        y=p.getY();

        if(x<-WORLD_RANGE || x>WORLD_RANGE || y<-WORLD_RANGE || y>WORLD_RANGE)
        {
            if(faceMappings)
            {
                tSigned32 indexIn3D = e.face().index();
                attributes.add("meshPart", (*faceMappings)[indexIn3D].first);
                attributes.add("faceIndex", (*faceMappings)[indexIn3D].second);
            }
            tPoint p=GetVertexPoint(e);
            attributes.add("x", p.getX());
            attributes.add("y", p.getY());
            attributes.add("allowed_range", WORLD_RANGE);
            Error("NonFatal", "Mesh Validation: Vertex is outside allowed range. Validation Fails.",attributes.get());
            return false;
        }
    }
    while(e.toNextInMesh());


    f=mesh.beginFace();
    do
    {
        if(f->isExternal())
            continue;

        e = tMesh::beginEdge(f);
        if(CountEdgesAround(e) != 3)
        {
            if(faceMappings)
            {
                tSigned32 indexIn3D = f.index();
                attributes.add("meshPart", (*faceMappings)[indexIn3D].first);
                attributes.add("faceIndex", (*faceMappings)[indexIn3D].second);
            }
            Error("NonFatal", "Mesh Validation: Face has number of edges not equal to 3. Validation Fails.",attributes.get());
            return false;
        }

        tLine line(GetVertexPoint(e),GetVertexPoint(e.next()));
        tPoint test=GetVertexPoint(e.prev());
        if(line.side(test)!=SIDE_RIGHT)
        {
            if(faceMappings)
            {
                tSigned32 indexIn3D = f.index();
                attributes.add("meshPart", (*faceMappings)[indexIn3D].first);
                attributes.add("faceIndex", (*faceMappings)[indexIn3D].second);
            }
            Error("NonFatal", "Mesh Validation: Face has zero area or is not clockwise (as seen from above). Validation Fails.",attributes.get());
            return false;
        }
    }
    while((++f).valid());

    return true;
}

template <class tMesh> bool
ValidateMesh_Import(
        tMesh &mesh,
        const std::vector<std::pair<tSigned32, tSigned32> >* faceMappings = 0
        )
{
    assertD(!mesh.hasDeadEntries());
    return ValidateMesh_Import_AllowDeadEntries(mesh, faceMappings);
}

template <class tMesh> bool
MeshContainsHeightDifferencesAtVertices(tMesh& mesh)
{
    typename tMesh::vertex v = mesh.beginVertex();
    do
    {
        typename tMesh::edge e = tMesh::firstEdge(v);
        float h = e->getZ();
        while(e.toNextAroundVertex())
        {
            if(e->getZ() != h)
            {
                return true;
            }
        }
    }
    while((++v).valid());
    return false;
}

