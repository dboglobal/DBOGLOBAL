//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_ITERRAINCALLBACK_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_ITERRAINCALLBACK_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iTerrainCallBack_Wrapper : public iTerrainCallBack, public iAPIObjectWrapper
{
public:

    typedef iTerrainCallBack tWrapped;

	iTerrainCallBack* _target;

    static tSigned32 interfaceIndex()
    {
        return 8;
    }

    float
    getHeightAt(tSigned32 x, tSigned32 y);

    float
    getHeightAtWithPrecision(tSigned32 x, tSigned32 y, float precisionX, float precisionY);

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
