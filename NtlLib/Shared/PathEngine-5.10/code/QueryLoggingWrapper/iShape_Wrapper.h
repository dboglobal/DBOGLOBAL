//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_ISHAPE_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_ISHAPE_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iShape_Wrapper : public iShape, public iAPIObjectWrapper
{
public:

    typedef iShape tWrapped;

	iShape* _target;

    static tSigned32 interfaceIndex()
    {
        return 1;
    }

    void
    destroy();

    tSigned32
    size() const;

    void
    vertex(tSigned32 index, tSigned32& x, tSigned32& y) const;

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
