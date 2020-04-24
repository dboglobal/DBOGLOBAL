//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "MeshLOSPreprocess.h"
#include "libs/MeshPair/interface/TranslationToOverlapped.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "i_pathengine.h"

template <class MESH>
bool PickPointOnMesh(
        const std::vector<nLineCollidesWithTriangle::cTriPreprocess>& preprocess,
        tMesh_3D& mesh,
        const cTranslationToOverlapped& translation,
        const cVector3F& cameraPosition, const cVector3F& cameraTarget,
        cPosition& result
        )
{
    tSigned32 fIndex;
    nLineCollidesWithTriangle::cLinePreprocess line(cameraPosition, cameraTarget);
    float fractionAlong;
    if(!RayCollidesWithMesh(line, preprocess, fIndex, fractionAlong))
    {
        return false;
    }

    cVector3F approximatePoint = (cameraTarget - cameraPosition) * fractionAlong + cameraPosition;

    MESH::face f = mesh.faceAt(fIndex);
    tPoint p;
    tSigned32 fIndex2D;

    p = tPoint(static_cast<tSigned32>(approximatePoint.x()), static_cast<tSigned32>(approximatePoint.y()), WORLD_RANGE + 2);
    if(PointIsInFace<tMesh_3D>(f, p))
    {
        fIndex2D = translation.translate(f, p).index();
        result.x = p.getX();
        result.y = p.getY();
        result.cell = fIndex2D;
        return true;
    }
    
    p=tPoint(static_cast<tSigned32>(approximatePoint.x()) + 1, static_cast<tSigned32>(approximatePoint.y()), WORLD_RANGE + 2);
    if(PointIsInFace<tMesh_3D>(f, p))
    {
        fIndex2D = translation.translate(f, p).index();
        result.x = p.getX();
        result.y = p.getY();
        result.cell = fIndex2D;
        return true;
    }

    p=tPoint(static_cast<tSigned32>(approximatePoint.x()) - 1, static_cast<tSigned32>(approximatePoint.y()), WORLD_RANGE + 2);
    if(PointIsInFace<tMesh_3D>(f, p))
    {
        fIndex2D = translation.translate(f, p).index();
        result.x = p.getX();
        result.y = p.getY();
        result.cell = fIndex2D;
        return true;
    }

    p=tPoint(static_cast<tSigned32>(approximatePoint.x()), static_cast<tSigned32>(approximatePoint.y()) + 1,WORLD_RANGE + 2);
    if(PointIsInFace<tMesh_3D>(f, p))
    {
        fIndex2D = translation.translate(f, p).index();
        result.x = p.getX();
        result.y = p.getY();
        result.cell = fIndex2D;
        return true;
    }

    p=tPoint(static_cast<tSigned32>(approximatePoint.x()), static_cast<tSigned32>(approximatePoint.y()) - 1, WORLD_RANGE + 2);
    if(PointIsInFace<tMesh_3D>(f, p))
    {
        fIndex2D = translation.translate(f, p).index();
        result.x = p.getX();
        result.y = p.getY();
        result.cell = fIndex2D;
        return true;
    }

    return false;
}

