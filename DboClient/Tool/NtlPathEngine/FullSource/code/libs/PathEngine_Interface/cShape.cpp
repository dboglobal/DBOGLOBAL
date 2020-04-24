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
#include "interface/cShape.h"
#include "interface/cPathEngine.h"
#include "common/interface/Assert.h"
#include "interface/Singletons.h"
#include "iAPIObjectWrapper.h"

cShape::cShape(const cConvexPoly& cs, cList_NoSize<cShape*>::iterator heldNode) :
 _heldNode(heldNode)
{
    _wrapper = 0;
    incRef();
    cs.normalise(_shape);
    _preprocessIndex = -1;
}

cShape::~cShape()
{
    assertD(!_heldNode.valid());
}

// called externally
void
cShape::destroy()
{
    if(_wrapper) {_wrapper->destroy_APIObjectWrapper();}
    INTERFACE_ASSERT(_heldNode.valid());
    nSingletons::pathEngine().lockHeldShapes();
    _heldNode.eraseAndSetInvalid();
    nSingletons::pathEngine().unlockHeldShapes();
    decRef();
}

tSigned32
cShape::size() const
{
    return _shape.sides();
}
void
cShape::vertex(tSigned32 index, tSigned32 &x, tSigned32 &y) const
{
    tPoint v = _shape.vertex(index);
    x = v.getX();
    y = v.getY();
}

void
cShape::declareWillBePreprocessed()
{
    if(_preprocessIndex == -1)
    {
        _preprocessIndex = nSingletons::pathEngine().allocateIndexForPreprocessShape(_shape);
    }
}
