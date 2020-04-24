//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh_Common/interface/iMeshTranslator.h"
#include "libs/Geometry/interface/SIOffset_PointLike.h"
#include "platform_common/PointMultiplication.h"
#include "common/TemplateMesh/MeshElement.h"

template <class tPointLike> tFace
TranslateToPartitioned(const iMeshTranslator<tMesh>& translator, tFace from, const tPointLike& p)
{
    cReverseSIOffset_PointLike<tPointLike> offsetPL(p);
    cMeshElement<tMesh> translatedEl = translator.translate(from, offsetPL);
    return translatedEl.asFace();
}
template <class tPointLike> tFace
TranslateFromPartitioned(const iMeshTranslator<tMesh>& translator, tFace from, const tPointLike& p)
{
    cReverseSIOffset_PointLike<tPointLike> offsetPL(p);
    cMeshElement<tMesh> translatedEl = translator.translateBack(from, offsetPL);
    return translatedEl.asFace();
}

inline cInternalPosition
TranslateToPartitioned(
        const iMeshTranslator<tMesh>& translator, 
        const cInternalPosition& position
        )
{
    return cInternalPosition(TranslateToPartitioned(translator, position.f, position.p), position.p);
}
