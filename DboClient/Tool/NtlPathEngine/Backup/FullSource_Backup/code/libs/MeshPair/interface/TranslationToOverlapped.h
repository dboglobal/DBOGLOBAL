//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TRANSLATETOOVERLAPPED_INCLUDED
#define TRANSLATETOOVERLAPPED_INCLUDED

#include "libs/MeshPair/interface/TranslationBSP.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/tLine_Header.h"
#include "common/STL/vector.h"
#include "common/STL/utility.h"

template <class T> class cMeshElement;
class iPointLike;
class cFacesOverlapped;
class cTranslationBSP;

class cTranslationToOverlapped
{
    tMesh_3D* _mesh3D;
    tMesh* _mesh2D;
    //..... optimise - look at replacing by cCollapsedVectorVector
    std::vector<std::vector<tSigned32> > _facesOverlappedBy2D;
    std::vector<std::vector<tSigned32> > _facesOverlappedBy3D;
    std::vector<cTranslationBSP*> _bsps;

    void processFace(tFace f, cFacesOverlapped& facesOverlapped);

public:

    cTranslationToOverlapped() {}
    cTranslationToOverlapped(
            tMesh_3D& mesh3D,
            tMesh& mesh2D,
            bool useBSPs
            );
    ~cTranslationToOverlapped();

    void swap(cTranslationToOverlapped& rhs)
    {
        _facesOverlappedBy2D.swap(rhs._facesOverlappedBy2D);
        _facesOverlappedBy3D.swap(rhs._facesOverlappedBy3D);
        std::swap(_mesh3D, rhs._mesh3D);
        std::swap(_mesh2D, rhs._mesh2D);
    }

    template <class tPointLike>
    tFace translate(tFace_3D f, tPointLike& p) const
    {
        tSigned32 i;
        for(i = 0; i != _facesOverlappedBy3D[f.index()].size(); ++i)
        {
            tFace result = _mesh2D->faceAt(_facesOverlappedBy3D[f.index()][i]);
            if(PointIsInFace<tMesh>(result, p))
            {
                return result;
            }
        }
        invalid();
        return tFace();
    }
    template <class tPointLike>
    tFace_3D translate(tFace f, tPointLike& p) const
    {
        if(!_bsps.empty())
        {
            tSigned32 resultIndex = _bsps[f.index()]->query(p);
            return _mesh3D->faceAt(resultIndex);
        }
        tSigned32 i;
        for(i = 0; i != _facesOverlappedBy2D[f.index()].size(); ++i)
        {
            tFace_3D result = _mesh3D->faceAt(_facesOverlappedBy2D[f.index()][i]);
            if(PointIsInFace<tMesh_3D>(result, p))
            {
                return result;
            }
        }
        invalid();
        return tFace_3D();
    }

    tEdge translateEntry(tEdge_3D entryEdge, const tLine& entryLine) const;
    tEdge_3D translateEntry(tEdge entryEdge, const tLine& entryLine) const;

    template <class tPointLike> cMeshElement<tMesh_3D>
    translateElement(cMeshElement<tMesh> el, const tPointLike& p) const
    {
        tFace f;
        if(el.isFace())
        {
            f = el.asFace();
        }
        else if(el.isEdge())
        {
            f = el.asEdge().face();
            if(f->isExternal())
            {
                f = el.asEdge().twin().face();
            }
        }
        else
        {
            f = tMesh::firstEdge(el.asVertex()).face();
            if(f->isExternal())
            {
                f = tMesh::firstEdge(el.asVertex()).twin().face();
            }
        }
        tSigned32 i;
        for(i = 0; i != _facesOverlappedBy2D[f.index()].size(); ++i)
        {
            tFace_3D f3D = _mesh3D->faceAt(_facesOverlappedBy2D[f.index()][i]);
            cMeshElement<tMesh_3D> result = ElementForPointInOverlappingFace<tMesh_3D>(f3D, p);
            if(result.valid())
            {
                return result;
            }
        }
        invalid();
        return tFace_3D();
    }

    tSigned32 numberOfFacesOverlapped(tFace f) const;
    tSigned32 numberOfFacesOverlapped(tSigned32 f2DIndex) const;
    tSigned32 iterateInSetOfFacesOverlapped(tSigned32 f2DIndex, tSigned32 i) const;
};

#endif
