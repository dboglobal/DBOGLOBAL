//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D_DEMO__SOLID_OBJECTS_INCLUDED
#define CONTENT_PROCESSING_3D_DEMO__SOLID_OBJECTS_INCLUDED

#include "i_pathengine.h"
#include "common/STL/vector.h"

class cSolidObjects : public iSolidObjects
{
public:

    // avoid vectors of vectors wherever possible in performance critical code,
    // but it's ok to use this here
    std::vector<std::vector<tSigned32> > _data;

// interface to iSolidObjects

    tSigned32 numberOfConvexSolids() const
    {
        return static_cast<tSigned32>(_data.size());
    }
    tSigned32 convexSolid_Points(tSigned32 convexSolidIndex) const
    {
        return static_cast<tSigned32>(_data[convexSolidIndex].size()) / 3;
    }
    void
    convexSolid_Point(tSigned32 convexSolidIndex, tSigned32 pointIndex, tSigned32& x, tSigned32& y, tSigned32& z) const
    {
        x = _data[convexSolidIndex][pointIndex * 3];
        y = _data[convexSolidIndex][pointIndex * 3 + 1];
        z = _data[convexSolidIndex][pointIndex * 3 + 2];
    }
    tSigned32 convexSolid_Attribute(tSigned32 convexSolidIndex, tSigned32 attributeIndex) const
    {
        return -1;
    }
};

#endif
