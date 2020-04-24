//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D_DEMO__SOLID_OBJECT_INCLUDED
#define CONTENT_PROCESSING_3D_DEMO__SOLID_OBJECT_INCLUDED

#include "SceneObject.h"
#include "SolidObjects.h"
#include "libs/Geometry/interface/tPoint3_Header.h"
#include "common/STL/vector.h"

class cSolidObjects;
class cMeshRenderInfo;

class cSolidObject : public bSceneObject
{
    std::vector<std::vector<tSigned32> > _operands;
    double _cosOfRotation, _sinOfRotation;
    cSolidObjects _cachedTransformedOperands;
    cMeshRenderInfo* _cachedRenderInfo;

    void transformOperand(tSigned32 i);

    void invalidateCachedData();
    void generateCachedDataIfNotValid();

public:

    cSolidObject(const char *const* attributes);
    ~cSolidObject();

    void renderEdges();
    void renderFaces();
    void addTransformedOperands(cSolidObjects& solidObjects);
    void addAllPoints(std::vector<tPoint3>& addTo);
};

#endif
