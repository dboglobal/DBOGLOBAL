//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_ICOLLISIONINFO_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_ICOLLISIONINFO_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iCollisionInfo_Wrapper : public iCollisionInfo, public iAPIObjectWrapper
{
public:

    typedef iCollisionInfo tWrapped;

	iCollisionInfo* _target;

    static tSigned32 interfaceIndex()
    {
        return 7;
    }

    void
    destroy();

    void
    getCollidingLine(tSigned32* coords) const;

    iAgent*
    getCollidingAgent() const;

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
