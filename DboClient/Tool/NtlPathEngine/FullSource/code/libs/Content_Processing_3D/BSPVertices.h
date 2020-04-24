//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BSP_VERTICES_INCLUDED
#define BSP_VERTICES_INCLUDED

#include "DynamicPoint3.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "libs/Geometry/interface/tPoint.h"
#include "common/STL/vector.h"

class cBSPVertices
{
    std::vector<tPoint3> _originalPoints;
    std::vector<cDynamicPoint3> _dynamicPoints;
 
public:

    void
    swap(cBSPVertices& rhs)
    {
        _originalPoints.swap(rhs._originalPoints);
        _dynamicPoints.swap(rhs._dynamicPoints);
    }

    tSigned32
    addOriginal(const tPoint3& p)
    {
        assertD(_dynamicPoints.size() == _originalPoints.size());
        tSigned32 index = SizeL(_originalPoints);
        _originalPoints.push_back(p);
        _dynamicPoints.push_back(p);
        return index;
    }

    tSigned32
    addDynamic(const cDynamicPoint3& p)
    {
        tSigned32 index = SizeL(_dynamicPoints);
        _dynamicPoints.push_back(p);
        return index;
    }

    bool
    isOriginal(tSigned32 index) const
    {
        return index < SizeL(_originalPoints);
    }
    const
    tPoint3& lookupOriginal(tSigned32 index) const
    {
        return _originalPoints[index];
    }
    const
    cDynamicPoint3& lookupDynamic(tSigned32 index) const
    {
        return _dynamicPoints[index];
    }
    const tPoint lookupApproximated2D(tSigned32 index) const;

    void approximateX();
    void approximateY();
    void approximateZ();
};

#endif
