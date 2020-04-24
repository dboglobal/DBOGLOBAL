//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "interface/tMesh.h"

//cInternalPosition::cInternalPosition(tFace f) :
// f(f),
// p(0, 0, 0)
//{
//    assertD(!f.valid());
//}

bool
cInternalPosition::valid() const
{
    return f.valid();
}

void
cInternalPosition::setInvalid()
{
    f = tMesh::endFace();
}

tSigned32
cInternalPosition::cell() const
{
    if(f.valid())
    {
        return f.index();
    }
    return -1;
}
