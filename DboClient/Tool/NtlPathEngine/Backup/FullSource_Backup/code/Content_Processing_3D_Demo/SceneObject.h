//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D_DEMO__SCENE_OBJECT_INCLUDED
#define CONTENT_PROCESSING_3D_DEMO__SCENE_OBJECT_INCLUDED

#include "common/STL/string.h"

class iXMLOutputStream;

class bSceneObject
{
protected:

    std::string _name;
    std::string _type;
    float _rotation;
    float _scale;
    tSigned32 _translationX, _translationY, _translationZ;

    virtual void invalidateCachedData() = 0;

public:

    bSceneObject(const char *const* attributes);

    void addAttributes(iXMLOutputStream& os);

    void addRotation(float r);
    void addScale(float toAdd, float minClamp, float maxClamp);
    void addTranslation(tSigned32 x, tSigned32 y, tSigned32 z, tSigned32 range);
};

#endif
