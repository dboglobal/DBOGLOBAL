
#pragma warning(disable : 4786)

#include "SimpleDOM.h"
#include "Error.h"
#include <stdlib.h>

using std::string;

void
cSimpleDOM::clear()
{
    _name = "";
    _attributes.clear();
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

long
cSimpleDOM::attributeAsLong(const std::string& attribute) const
{
    string value = getAttribute(attribute);
    assertD(!value.empty());
    char* ptr;
    long result = strtol(value.c_str(), &ptr, 10);
    assertD(*ptr == 0);
    return result;
}
long
cSimpleDOM::attributeAsLongWithDefault(const std::string& attribute, long defaultValue) const
{
    if(!hasAttribute(attribute))
    {
        return defaultValue;
    }
    string value = getAttribute(attribute);
    assertD(!value.empty());
    char* ptr;
    long result = strtol(value.c_str(), &ptr, 10);
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

long
cSimpleDOM::firstChildWithName(const std::string& name) const
{
    size_t i;
    for(i = 0; i < _children.size(); i++)
    {
        if(_children[i]._name == name)
        {
            return i;
        }
    }
    return -1;
}

cSimpleDOM&
cSimpleDOM::refFirstChildWithName(const std::string& name)
{
    long i = firstChildWithName(name);
    assertD(i >= 0);
    return _children[i];
}

const cSimpleDOM&
cSimpleDOM::refFirstChildWithName(const std::string& name) const
{
    long i = firstChildWithName(name);
    assertD(i >= 0);
    return _children[i];
}
