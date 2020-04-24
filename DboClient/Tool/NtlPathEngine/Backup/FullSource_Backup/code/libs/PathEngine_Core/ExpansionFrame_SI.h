//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EXPANSION_FRAME_SI_DEFINED
#define EXPANSION_FRAME_SI_DEFINED

#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "common/STL/vector.h"

class cConvexPoly;

class cExpansionFrame_SI
{
public:
    enum eStartType
    {
        STARTS_INSIDE,
        MESH_ENTRY,
        BLOCKED,
    };
private:

    tSigned32 _sections;
    std::vector<tLine> _spokes;
    std::vector<std::vector<tEdge> > _baseSequence;
    eStartType _startType, _endType;
    tEdge _startTypeInfo, _endTypeInfo;

    bool sectionIsRadial(tSigned32 section) const;

public:

     // a reference to expandBy is retained by the constructors for the lifetime of the frame
    cExpansionFrame_SI(tFace convexExternalFace, const cConvexPoly& expandBy);
    cExpansionFrame_SI(tEdge first, tEdge last, const cConvexPoly& expandBy);

    void getStartInfo(eStartType& type, tEdge& info) const
    {
        type = _startType;
        info = _startTypeInfo;
    }
    void getEndInfo(eStartType& type, tEdge& info) const
    {
        type = _endType;
        info = _endTypeInfo;
    }

    tSigned32 size() const
    {
        return _sections;
    }

    tFace startFace() const
    {
        return _startTypeInfo.face();
    }
    tSigned32 baseSequenceSize(tSigned32 section) const
    {
        return SizeL(_baseSequence[section]);
    }
    tEdge baseSequenceEdge(tSigned32 section, tSigned32 indexInSection) const
    {
        if(indexInSection >= baseSequenceSize(section))
        {
            assertD(indexInSection == baseSequenceSize(section));
            return tMesh::endEdge();
        }
        return _baseSequence[section][indexInSection];
    }

    tLine boundary(tSigned32 section) const
    {
        return tLine(_spokes[section].end(), _spokes[section + 1].end());
    }
    tLine base(tSigned32 section) const
    {
        assertD(!sectionIsRadial(section));
        return tLine(_spokes[section].start(), _spokes[section + 1].start());
    }
    tLine sectionStart(tSigned32 section) const
    {
        return _spokes[section];
    }
    tLine sectionEnd(tSigned32 section) const
    {
        return _spokes[section + 1];
    }
    tLine expandLine(tSigned32 section, const tPoint& p) const;
    eSide sense(tSigned32 section, const tLine& line) const;
    eSide sense(tSigned32 section, tEdge edge) const;
    bool isBefore(tSigned32 section, const tPoint& lhs, const tPoint& rhs) const;

    enum eTraverseResult
    {
        ENDS,
        EXITS,
        CROSSES_TO_NEXT,
    };

    eTraverseResult traverse(tSigned32 section, tEdge e) const;
    eTraverseResult reverseTraverse(tSigned32 section, tEdge e) const;
};

#endif
