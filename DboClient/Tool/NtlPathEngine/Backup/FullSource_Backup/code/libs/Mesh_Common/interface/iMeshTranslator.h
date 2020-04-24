//**********************************************************************
//
// Copyright (c) 2003-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_MESH_TRANSLATOR_HAS_BEEN_INCLUDED
#define I_MESH_TRANSLATOR_HAS_BEEN_INCLUDED

#include "common/TemplateMesh/MeshElement.h"
#include "libs/Geometry/interface/tPoint_Header.h"

class iPointLike;

template <class tMesh>
class iMeshTranslator
{
public:
    virtual ~iMeshTranslator() {};
    virtual cMeshElement<tMesh> translate(const cMeshElement<tMesh>& el, const iPointLike& p) const = 0;
    virtual cMeshElement<tMesh> translateBack(const cMeshElement<tMesh>& el, const iPointLike& p) const = 0;
};

template <class tMesh>
class cReversedMeshTranslator : public iMeshTranslator<tMesh>
{
    const iMeshTranslator<tMesh>* _base;

public:

    cReversedMeshTranslator(const iMeshTranslator<tMesh>* base) :
      _base(base)
    {
    }

    cMeshElement<tMesh> translate(const cMeshElement<tMesh>& el, const iPointLike& p) const
    {
        return _base->translateBack(el, p);
    }
    cMeshElement<tMesh> translateBack(const cMeshElement<tMesh>& el, const iPointLike& p) const
    {
        return _base->translate(el, p);
    }
};

#endif
