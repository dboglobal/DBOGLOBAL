//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "ValidateVolume.h"
#include "GetOriginAndNormal.h"
#include "Plane.h"
#include "SideOfPlane.h"
#include "tMesh_Simple3D.h"
#include "common/TemplateMesh/Helper.h"
#include "common/interface/Error.h"

//.. remove repetition of this
static bool
VertexConnectsToFace(tFace_Simple3D f, tVertex_Simple3D v)
{
    tEdge_Simple3D e = tMesh_Simple3D::beginEdge(f);
    do
    {
        if(e.vertex() == v)
        {
            return true;
        }
    }
    while(e.toNextInFace());
    return false;
}

bool
ValidateVolume(tMesh_Simple3D& volume)
{
    tFace_Simple3D f;

    if(volume.faces() < 4)
    {
        Error("NonFatal", "Volume Validation: Volume must contain at least four polygons. Validation fails.", 0);
        return false;
    }

    {
        tEdge_Simple3D e = volume.beginEdge();
        do
        {
            if(!e.twin().valid())
            {
                Error("NonFatal", "Volume Validation: Edge has no twin. Validation fails.", 0);
                return false;
            }
        }
        while(e.toNextInMesh());
    }

    {
        tEdge_Simple3D e = volume.beginEdge();
        do
        {
            if(GetVertexPoint3(e) == GetVertexPoint3(e.next()))
            {
                Error("NonFatal", "Volume Validation: Edge has zero length. Validation fails.", 0);
                return false;
            }
        }
        while(e.toNextInMesh());
    }

    f = volume.beginFace();
    do
    {
        if(CountEdgesInFace(volume, f) < 3)
        {
            Error("NonFatal", "Volume Validation: Face has less than 3 sides. Validation fails.", 0);
            return false;
        }

        cPlane plane;
        GetOriginAndNormal(f, plane);

        tVertex_Simple3D v = volume.beginVertex();
        do
        {
            eSideOfPlane side = SideOfPlane(plane, GetVertexPoint3(v));

            if(side == IN_FRONT)
            {
                Error("NonFatal", "Volume Validation: Volume is not convex. Validation fails.", 0);
                return false;
            }

            if(VertexConnectsToFace(f, v))
            {
                if(side != ON_PLANE)
                {
                    Error("NonFatal", "Volume Validation: A vertex on a face is not exactly on the plane of that face. Validation fails.", 0);
                    return false;
                }
            }
            else
            {
                if(side != BEHIND)
                {
                    Error("NonFatal", "Volume Validation: A vertex not on a face is on the plane of that face. Validation fails.", 0);
                    return false;
                }
            }
        }
        while((++v).valid());
    }
    while((++f).valid());

    return true;
}

