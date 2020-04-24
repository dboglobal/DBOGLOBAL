//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/Mesh3DCorners.h"
#include "libs/Mesh_Common/interface/Mesh.h"
#include "libs/Mesh_Common/interface/MeshTraversal_Common.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/MeshPair/interface/TranslationToOverlapped.h"
#include "libs/PathEngine_Core/interface/CornerProvider.h"
#include "common/TemplateMesh/MeshElement.h"


template <class tEdgePredicate> void
AddCorners(
        tMesh_3D& mesh3D,
        const cTranslationToOverlapped& translation,
        tEdgePredicate predicate,
        cCornerProvider& addTo)
{
    tVertex_3D v = mesh3D.beginVertex();
    do
    {
        assertD(!tMesh_3D::lastEdge(v).valid()); // mesh must be fully connected
        tEdge_3D e1 = tMesh_3D::firstEdge(v);
        if(VertexIsExternal(e1))
        {
            continue;
        }
        do
        {
            if(!predicate(e1))
            {
                continue;
            }
            tPoint axis1 = GetEdgeAxis(e1);
            tEdge_3D e2 = e1.prev();
//            while(!predicate.faceAbortsCorner(e2.face()) && !predicate(e2) && axis1.side(GetEdgeAxis(e2)) == SIDE_LEFT)
            while(!predicate(e2) && axis1.side(GetEdgeAxis(e2)) == SIDE_LEFT)
            {
                e2 = e2.twin().prev();
            }
//            if(predicate.faceAbortsCorner(e2.face()))
//            {
//                continue;
//            }
            if(!predicate(e2))
            {
              // concave corner
                continue;
            }
            if(axis1.side(GetEdgeAxis(e2)) != SIDE_LEFT)
            {
                continue;
            }
            // convex corner
            cMeshElement<tMesh_3D> el3D(v);
            tPoint p = GetVertexPoint(v);
            tFace_3D f3D = TranslateToOwned_SI<tMesh_3D>(el3D, p);
            tFace f = translation.translate(f3D, p);
            addTo.push_back(f, p, GetEdgeAxis(e2), GetEdgeAxis(e1));
        }
        while(e1.toNextAroundVertex());
    }
    while((++v).valid());
}

//class cHardCornersPredicate
//{
//    const std::vector<tSigned32>& _surfaceTypes;
//    const std::vector<float>& _costs;
//
//public:
//
//    cHardCornersPredicate(
//            const std::vector<tSigned32>& surfaceTypes,
//            const std::vector<float>& costs) :
//     _surfaceTypes(surfaceTypes),
//     _costs(costs)
//    {
//    }
//
//    bool operator()(tEdge_3D e)
//    {
//        return _costs[_surfaceTypes[e.face().index()]] == -1.f
//         && _costs[_surfaceTypes[e.twin().face().index()]] != -1.f;
//    }
//    bool faceAbortsCorner(tFace_3D f)
//    {
//        return false;
//    }
//};
//
//void
//AddMesh3DHardCorners(
//        tMesh_3D& mesh3D,
//        const cTranslationToOverlapped& translation,
//        const std::vector<tSigned32>& surfaceTypes,
//        const std::vector<float>& costs,
//        cCornerProvider& addTo)
//{
//    cHardCornersPredicate predicate(surfaceTypes, costs);
//    AddCorners(mesh3D, translation, predicate, addTo);
//}

class cSoftCornersPredicate
{
    const std::vector<tSigned32>& _surfaceTypes;
    const std::vector<float>& _costs;
    const std::vector<tPoint>& _costDirections;

public:

    cSoftCornersPredicate(
            const std::vector<tSigned32>& surfaceTypes,
            const std::vector<float>& costs,
            const std::vector<tPoint>& costDirections) :
     _surfaceTypes(surfaceTypes),
     _costs(costs),
     _costDirections(costDirections)
    {
    }

    bool operator()(tEdge_3D e)
    {
        tSigned32 t = _surfaceTypes[e.face().index()];
        tSigned32 tTwin = _surfaceTypes[e.twin().face().index()];
        if(_costs[t] == -1.f && _costs[tTwin] != -1.f)
        {
            return true;
        }
        if(_costs[t] != -1.f && _costs[tTwin] != -1.f && _costs[t] > _costs[tTwin])
        {
            return true;
        }
        //... optimise - look at working out a more specific test for direction boundaries
        if(_costs[t] != 0.f && _costDirections[t] != _costDirections[tTwin])
        {
            return true;
        }
        return false;
    }
//    bool faceAbortsCorner(tFace_3D f)
//    {
//        return _costs[_surfaceTypes[f.index()]] == -1.f;
//    }
};

void
AddMesh3DSoftCorners(
        tMesh_3D& mesh3D,
        const cTranslationToOverlapped& translation,
        const std::vector<tSigned32>& surfaceTypes,
        const std::vector<float>& costs,
        const std::vector<tPoint>& costDirections,
        cCornerProvider& addTo)
{
    cSoftCornersPredicate predicate(surfaceTypes, costs, costDirections);
    AddCorners(mesh3D, translation, predicate, addTo);
}
