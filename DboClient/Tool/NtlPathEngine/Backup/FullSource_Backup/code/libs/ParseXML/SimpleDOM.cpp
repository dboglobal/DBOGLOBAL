//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/SimpleDOM.h"
#include "common/interface/Error.h"
#include "common/interface/Assert.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/CLib/stdlib.h"
#include "common/STL/sstream.h"

using std::string;


cSimpleDOM::~cSimpleDOM()
{
    clear();
}

void
cSimpleDOM::setName(const std::string& value)
{
    _name = value;
}
void
cSimpleDOM::appendChild(cSimpleDOM* toAppend)
{
    _children.push_back(toAppend);
}
void
cSimpleDOM::replaceChild(tSigned32 index, cSimpleDOM* replaceWith)
{
    delete _children[index];
    _children[index] = replaceWith;
}
void
cSimpleDOM::setAttribute(const char* name, const char* value)
{
    _attributes[name] = value;
}
void
cSimpleDOM::setAttribute(const char* name, tSigned32 value)
{
    std::ostringstream oss;
    oss << value;
    _attributes[name] = oss.str();
}

const std::string&
cSimpleDOM::name() const
{
    return _name;
}
const std::vector<cSimpleDOM*>&
cSimpleDOM::children() const
{
    return _children;
}

void
cSimpleDOM::clear()
{
    _name = "";
    _attributes.clear();
    size_t i;
    for(i = 0; i < _children.size(); ++i)
    {
        delete _children[i];
    }
    _children.clear();
}

bool
cSimpleDOM::hasAttribute(const std::string& attribute) const
{
    return _attributes.find(attribute) != _attributes.end();
}

std::string
cSimpleDOM::getAttribute(const std::string& attribute) const
{
    std::map<std::string, std::string>::const_iterator i = _attributes.find(attribute);
    if(i == _attributes.end())
    {
        return "";
    }
    return i->second;
}

tSigned32
cSimpleDOM::attributeAsLong(const std::string& attribute) const
{
    string value = getAttribute(attribute);
    assertD(!value.empty());
    char* ptr;
    tSigned32 result = strtol(value.c_str(), &ptr, 10);
    assertD(*ptr == 0);
    return result;
}
tSigned32
cSimpleDOM::attributeAsLongWithDefault(const std::string& attribute, tSigned32 defaultValue) const
{
    if(!hasAttribute(attribute))
    {
        return defaultValue;
    }
    string value = getAttribute(attribute);
    assertD(!value.empty());
    char* ptr;
    tSigned32 result = strtol(value.c_str(), &ptr, 10);
    assertD(*ptr == 0);
    return result;
}
float
cSimpleDOM::attributeAsFloat(const std::string& attribute) const
{
    string value = getAttribute(attribute);
    assertD(!value.empty());
    char* ptr;
    float result = static_cast<float>(strtod(value.c_str(), &ptr));
    assertD(*ptr == 0);
    return result;
}
float
cSimpleDOM::attributeAsFloatWithDefault(const std::string& attribute, float defaultValue) const
{
    if(!hasAttribute(attribute))
    {
        return defaultValue;
    }
    string value = getAttribute(attribute);
    assertD(!value.empty());
    char* ptr;
    float result = static_cast<float>(strtod(value.c_str(), &ptr));
    assertD(*ptr == 0);
    return result;
}
bool
cSimpleDOM::attributeAsBool(const std::string& attribute) const
{
    string value = getAttribute(attribute);
    if(value == "true")
    {
        return true;
    }
    assertD(value == "false");
    return false;
}
bool
cSimpleDOM::attributeAsBoolWithDefault(const std::string& attribute, bool defaultValue) const
{
    if(!hasAttribute(attribute))
    {
        return defaultValue;
    }
    return attributeAsBool(attribute);
}

tSigned32
cSimpleDOM::firstChildWithName(const std::string& name) const
{
    tSigned32 i;
    for(i = 0; i < SizeL(_children); i++)
    {
        if(_children[i]->_name == name)
        {
            return i;
        }
    }
    return -1;
}

cSimpleDOM&
cSimpleDOM::refFirstChildWithName(const std::string& name)
{
    tSigned32 i = firstChildWithName(name);
    assertD(i >= 0);
    return *_children[i];
}

const cSimpleDOM&
cSimpleDOM::refFirstChildWithName(const std::string& name) const
{
    tSigned32 i = firstChildWithName(name);
    assertD(i >= 0);
    return *_children[i];
}

void
cSimpleDOM::save(iXMLOutputStream& xos) const
{
    xos.openElement(name().c_str());
    std::map<std::string, std::string>::const_iterator it = _attributes.begin();
    while(it != _attributes.end())
    {
        xos.addAttribute(it->first.c_str(), it->second.c_str());
    }
    for(tSigned32 i = 0; i != SizeL(_children); ++i)
    {
        _children[i]->save(xos);
    }
    xos.closeElement(name().c_str());
}
