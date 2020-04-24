//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_IPATH_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_IPATH_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iPath_Wrapper : public iPath, public iAPIObjectWrapper
{
public:

    typedef iPath tWrapped;

	iPath* _target;

    static tSigned32 interfaceIndex()
    {
        return 6;
    }

    void
    destroy();

    tSigned32
    size() const;

    cPosition
    position(tSigned32 pointIndex) const;

    tSigned32
    connectionIndex(tSigned32 segmentIndex) const;

    const cPosition*
    getPositionArray() const;

    const tSigned32*
    getConnectionIndexArray() const;

    tUnsigned32
    getLength() const;

    iCollisionInfo*
    advanceAlong(iShape* shape, float distance, iCollisionContext* context, float& precisionX, float& precisionY);

    void
    renderOnGround(tSigned32 originX, tSigned32 originY, iRender3DLinesCallBack& callBack) const;

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
