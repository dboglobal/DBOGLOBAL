//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/interface/Assert.h"
#include "common/STL/utility.h"

class cVertexPair
{
public:

    cVertexPair(tSigned32 v1, tSigned32 v2)
    {
        assertD(v1 >= 0);
        assertD(v2 >= 0);
        assertD(v1 != v2);
        if(v1 > v2)
        {
            _v1 = v1;
            _v2 = v2;
        }
        else
        {
            _v1 = v2;
            _v2 = v1;
        }
    }

    bool operator==(const cVertexPair& rhs) const
    {
        return _v1 == rhs._v1 && _v2 == rhs._v2;
    }
    bool operator<(const cVertexPair& rhs) const
    {
        if(_v1 != rhs._v1)
        {
            return _v1 < rhs._v1;
        }
        return _v2 < rhs._v2;
    }

    tSigned32 _v1, _v2;
};

