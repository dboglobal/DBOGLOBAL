//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/STL/string.h"
#include "common/STL/map.h"
#include "common/STL/vector.h"

class iXMLOutputStream;

class cSimpleDOM
{
    std::string _name;
    std::map<std::string, std::string> _attributes;
    std::vector<cSimpleDOM*> _children;

public:

    ~cSimpleDOM();

    void setName(const std::string& value);
    void appendChild(cSimpleDOM* toAppend);
    void replaceChild(tSigned32 index, cSimpleDOM* replaceWith);
    void setAttribute(const char* name, const char* value);
    void setAttribute(const char* name, tSigned32 value);

    const std::string& name() const;
    const std::vector<cSimpleDOM*>& children() const;

    void clear();
    bool hasAttribute(const std::string& attribute) const;
    std::string getAttribute(const std::string& attribute) const;

    tSigned32 attributeAsLong(const std::string& attribute) const;
    tSigned32 attributeAsLongWithDefault(const std::string& attribute, tSigned32 defaultValue) const;
    float attributeAsFloat(const std::string& attribute) const;
    float attributeAsFloatWithDefault(const std::string& attribute, float defaultValue) const;
    bool attributeAsBool(const std::string& attribute) const;
    bool attributeAsBoolWithDefault(const std::string& attribute, bool defaultValue) const;

    tSigned32 firstChildWithName(const std::string& name) const;
    cSimpleDOM& refFirstChildWithName(const std::string& name);
    const cSimpleDOM& refFirstChildWithName(const std::string& name) const;

    void save(iXMLOutputStream& xos) const;
};
