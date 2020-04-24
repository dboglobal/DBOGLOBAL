//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"
#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Geometry/interface/SegmentTree.h"
#include "common/STL/vector.h"
class cRangeTree;

class cMesh3DPartitioning
{
    cSegmentTree* _segmentTree;
    cRangeTree* _rangeTree;
    std::vector<tSigned32> _yRanges;

    void initRangeTree(const std::vector<cSegmentTreeElement>& segments);
    void filterByYRange(tSigned32 startY, tSigned32 endY, tSigned32* resultsBuffer, tSigned32& results) const;

public:

    template <class tMesh>
    cMesh3DPartitioning(tMesh& mesh)
    {
        std::vector<cSegmentTreeElement> segments;
        _yRanges.resize(mesh.faces() * 2);

        typename tMesh::face f = mesh.beginFace();
        do
        {
            if(f->isExternal())
            {
                continue;
            }

            typename tMesh::edge e = tMesh::beginEdge(f);
            tPoint bottomLeft = GetVertexPoint(e);
            tPoint topRight = GetVertexPoint(e);
            while(e.toNextInFace())
            {
                bottomLeft.updateAsMinimumBound(GetVertexPoint(e));
                topRight.updateAsMaximumBound(GetVertexPoint(e));
            }

            segments.push_back(cSegmentTreeElement());

            segments.back().start = bottomLeft.getX();
            segments.back().end = topRight.getX() + 1;
            segments.back().key = f.index();

            _yRanges[f.index() * 2] = bottomLeft.getY();
            _yRanges[f.index() * 2 + 1] = topRight.getY();
        }
        while((++f).valid());

        _segmentTree = new cSegmentTree(segments);

        initRangeTree(segments);
    }

    ~cMesh3DPartitioning();

    tSigned32 maximumResultsSize();
    tSigned32 queryFacesPotentiallyStabbed(const tPoint& p, tSigned32* resultsBuffer) const;
    tSigned32 queryFacesPotentiallyOverlapped(const tPoint& bottomLeft, const tPoint& topRight, tSigned32* resultsBuffer) const;
};
