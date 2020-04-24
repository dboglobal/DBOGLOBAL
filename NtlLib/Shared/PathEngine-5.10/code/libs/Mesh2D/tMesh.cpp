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
#include "interface/tMesh.h"
#include "common/TemplateMesh/ElementIndexing.h"

cV::cV(const tPoint& p) :
 _elementOn3D(-1),
 _p(p)
{
}

tSigned32
cV::getTargetVertexIndex() const
{
    bMeshElement::eType type;
    tSigned32 subIndex;
    TypeAndSubIndexForElementIndex(_elementOn3D, type, subIndex);
    assertD(type == bMeshElement::VERTEX);
    return subIndex;
}

