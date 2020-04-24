//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "MeshLOSPreprocess.h"
#include "LineCollidesWithTriangle.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"

void
GenerateMeshLOSPreprocess(tMesh_3D& mesh, std::vector<nLineCollidesWithTriangle::cTriPreprocess>& result)
{
    result.clear();
    tFace_3D f = mesh.beginFace();
    while(f.valid() && !f->isExternal())
    {
        tEdge_3D e = tMesh_3D::beginEdge(f);
        cVector3F v1 = GetVertexPoint3F(e);
        cVector3F v2 = GetVertexPoint3F(e.next());
        cVector3F v3 = GetVertexPoint3F(e.next().next());
        result.push_back(nLineCollidesWithTriangle::cTriPreprocess(v1, v2, v3));
        ++f;
    }
    while(f.valid())
    {
        assertD(f->isExternal());
        ++f;
    }
}

bool
LineCollidesWithMesh(const nLineCollidesWithTriangle::cLinePreprocess& line, const std::vector<nLineCollidesWithTriangle::cTriPreprocess>& meshPreprocess, tSigned32& faceIndex)
{
    size_t i;
    for(i = 0; i < meshPreprocess.size(); i++)
    {
        if(nLineCollidesWithTriangle::lineCollidesWithTriangle(meshPreprocess[i], line))
        {
            faceIndex = static_cast<tSigned32>(i);
            return true;
        }
    }
    return false;
}

bool
RayCollidesWithMesh(const nLineCollidesWithTriangle::cLinePreprocess& line, const std::vector<nLineCollidesWithTriangle::cTriPreprocess>& meshPreprocess, tSigned32& faceIndex, float& fractionAlong)
{
    size_t i;
    faceIndex = -1;
    for(i = 0; i < meshPreprocess.size(); i++)
    {
        float candidateFraction;
        if(nLineCollidesWithTriangle::rayCollidesWithTriangle(meshPreprocess[i], line, candidateFraction))
        {
            if(faceIndex == -1 || candidateFraction < fractionAlong)
            {
                faceIndex = static_cast<tSigned32>(i);
                fractionAlong = candidateFraction;
            }
        }
    }
    return faceIndex != -1;
}
void
GetFacesObstructingLine(const nLineCollidesWithTriangle::cLinePreprocess& line,
                         const std::vector<nLineCollidesWithTriangle::cTriPreprocess>& meshPreprocess,
                         std::vector<tSigned32>& appendTo)
{
    size_t i;
    for(i = 0; i < meshPreprocess.size(); i++)
    {
        if(nLineCollidesWithTriangle::lineCollidesWithTriangle(meshPreprocess[i], line))
        {
            appendTo.push_back(static_cast<tSigned32>(i));
        }
    }
}

