//**********************************************************************
//
// Copyright (c) 2002-2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "PointsOfVisibilityState.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/tPoint.h"
#include "platform_common/CoordinateRanges.h"
#include "common/interface/Assert.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"

class cInternalPosition;
class cQueryContext;
class cLocalisedConvexShapeSet;
class cEdgeExpansion;

class cPathAwayTargets
{
    tPoint _bottomLeft, _topRight;
    std::vector<tPoint> _pointTargets;
    tMeshCircuits _boundCircuits;
    std::list<tFace> _containedExternalFaces;

public:

    class cInitParams
    {
        friend class cPathAwayTargets;
        
        cQueryContext& _qc;
        const cInternalPosition& _centre;
        tSigned32 _radius;
        const cEdgeExpansion& _edgeExpansion;
        const cLocalisedConvexShapeSet& _expansions;

    public:
        
        cInitParams(
                cQueryContext& qc,
                const cInternalPosition& centre, tSigned32 radius,
                const cEdgeExpansion& edgeExpansion,
                const cLocalisedConvexShapeSet& expansions
                ) :
         _qc(qc),
         _centre(centre),
         _radius(radius),
         _edgeExpansion(edgeExpansion),
         _expansions(expansions)
        {
        }
    };

    cPathAwayTargets(const cInitParams& params);

    //cPathAwayTargets(
    //        cQueryContext& qc,
    //        const cInternalPosition& centre, tSigned32 radius,
    //        const cEdgeExpansion& edgeExpansion,
    //        const cLocalisedConvexShapeSet& expansions
    //        );

    bool pointIsInside(const tPoint &p) const
    {
        if(p.getX()<=_bottomLeft.getX())
            return false;
        if(p.getX()>=_topRight.getX())
            return false;
        if(p.getY()<=_bottomLeft.getY())
            return false;
        if(p.getY()>=_topRight.getY())
            return false;
        return true;
    }
    bool positionIsInside(const cInternalPosition& p) const;

    tSigned32
    getNumberOfTargets() const
    {
        return SizeL(_pointTargets) + 4;
    }
    tPoint
    getTargetPoint(tSigned32 index, const tPoint& parent) const
    {
        assertD(pointIsInside(parent));
        if(index < SizeL(_pointTargets))
        {
            return _pointTargets[index];
        }
        index -= SizeL(_pointTargets);
        switch(index)
        {
        default:
            invalid();
        case 0: 
            return tPoint(parent.getX(), _topRight.getY(), WORLD_RANGE + 1);
        case 1:
            return tPoint(_topRight.getX(), parent.getY(), WORLD_RANGE + 1);
        case 2:
            return tPoint(parent.getX(), _bottomLeft.getY(), WORLD_RANGE + 1);
        case 3:
            return tPoint(_bottomLeft.getX(), parent.getY(), WORLD_RANGE + 1);
        }
    }
    tUnsigned32
    minimumCostToGoal(const tPoint &p) const
    {
        tUnsigned32 result=p.getX()-_bottomLeft.getX();
        tUnsigned32 candidate;
        candidate=_topRight.getX()-p.getX();
        if(candidate<result)
            result=candidate;
        candidate=p.getY()-_bottomLeft.getY();
        if(candidate<result)
            result=candidate;
        candidate=_topRight.getY()-p.getY();
        if(candidate<result)
            result=candidate;
        return result;
    }

// following just for new graph setup

    cInternalPosition
    getTargetPosition(tSigned32 index, const cInternalPosition& parent) const;

    bool
    nonTargetStateIsDisabled(cPointsOfVisibilityState state, bool stateIsCorner, const tPoint& p) const
    {
        if((stateIsCorner || state.approachEndPointZone() == EPZ_ARRIVING)
         && !pointIsInside(p)
         )
        {
            return true;
        }
        return false;
    }
    bool
    nonTargetStateIsGoal(const tPoint& p) const
    {
        return !pointIsInside(p);
    }
    static bool
    stateCanPotentiallyConnectToTarget(cPointsOfVisibilityState state)
    {
        return true;
    }
    static bool
    hasDynamicTargets()
    {
        return true;
    }
};

