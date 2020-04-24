//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef C_SHAPE_INCLUDED
#define C_SHAPE_INCLUDED

#include "common/interface/Assert.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "i_pathengine.h"
#include "common/Containers/ReplacementList.h"
#include "platform_common/ThreadSafeHandleTarget.h"

class iAPIObjectWrapper;

class cShape : public iShape, public bThreadSafeHandleTarget
{
    cConvexPoly _shape;
    cList_NoSize<cShape*>::iterator _heldNode;
    tSigned32 _preprocessIndex;

public:

    iAPIObjectWrapper* _wrapper;

    cShape(const cConvexPoly& cs, cList_NoSize<cShape*>::iterator heldNode);
    ~cShape();

// iShape

    void destroy();
    tSigned32 size() const;
    void vertex(tSigned32 index, tSigned32& x, tSigned32& y) const;
    void operator delete(void* p) {::operator delete(p);}

//====================

    void declareWillBePreprocessed();
    tSigned32 getPreprocessIndex() const
    {
        return _preprocessIndex;
    }

    const cConvexPoly& refCS() const
    {
        return _shape;
    }
};

inline cShape* FromI(iShape* ptr)
{
    return static_cast<cShape*>(ptr);
}

#endif
