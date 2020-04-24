//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "SceneObject.h"
#include "common/interface/Attributes.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/STL/sstream.h"

bSceneObject::bSceneObject(const char *const* attributes)
{
    _name = GetValueForAttribute(attributes, "name");
    assertR(!_name.empty());
    _type = GetValueForAttribute(attributes, "type");
    assertR(!_type.empty());
    _rotation = ValueAsFloat(attributes, "rotation");
    _scale = ValueAsFloat(attributes, "scale");
    _translationX = ValueAsLong(attributes, "x");
    _translationY = ValueAsLong(attributes, "y");
    _translationZ = ValueAsLong(attributes, "z");
}

void
bSceneObject::addAttributes(iXMLOutputStream& os)
{
    os.addAttribute("name", _name.c_str());
    os.addAttribute("type", _type.c_str());
    {
        std::ostringstream oss;
        oss << _rotation;
        os.addAttribute("rotation", oss.str().c_str());
    }
    {
        std::ostringstream oss;
        oss << _scale;
        os.addAttribute("scale", oss.str().c_str());
    }
    os.addAttribute("x", _translationX);
    os.addAttribute("y", _translationY);
    os.addAttribute("z", _translationZ);
}

void
bSceneObject::addRotation(float r)
{
    _rotation += r;
    invalidateCachedData();
}

void
bSceneObject::addScale(float toAdd, float minClamp, float maxClamp)
{
    _scale += toAdd;
    if(_scale < minClamp)
    {
        _scale = minClamp;
    }
    if(_scale > maxClamp)
    {
        _scale = maxClamp;
    }
    invalidateCachedData();
}

void
bSceneObject::addTranslation(tSigned32 x, tSigned32 y, tSigned32 z, tSigned32 range)
{
    _translationX += x;
    if(_translationX < -range)
    {
        _translationX = range;
    }
    if(_translationX > range)
    {
        _translationX = range;
    }
    _translationY += y;
    if(_translationY < -range)
    {
        _translationY = range;
    }
    if(_translationY > range)
    {
        _translationY = range;
    }
    _translationZ += z;
    if(_translationZ < -range)
    {
        _translationZ = range;
    }
    if(_translationZ > range)
    {
        _translationZ = range;
    }
    invalidateCachedData();
}
