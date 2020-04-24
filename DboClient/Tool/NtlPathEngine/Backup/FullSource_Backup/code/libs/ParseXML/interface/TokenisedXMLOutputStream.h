//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TOKENISEDXMLOUTPUTSTREAM_HAS_BEEN_INCLUDED
#define TOKENISEDXMLOUTPUTSTREAM_HAS_BEEN_INCLUDED

#include "common/interface/Assert.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/STL/string.h"
#include "common/STL/vector.h"

class iOutputStream;

class cElementAndAttributeEnumerator : public iXMLOutputStream
{
    friend class cTokenisedXMLOutputStream;

    std::vector<std::string> _elements;
    std::vector<std::string> _attributes;
    std::vector<char> _types;
    tSigned32 _indent;

    void AddOrUpdateAttributeAndType(const char* attribute, char type);

public:

    cElementAndAttributeEnumerator()
    {
        _indent = 0;
    }
    ~cElementAndAttributeEnumerator()
    {
        assertD(_indent == 0);
    }

    void writeTo(iOutputStream& os);

    void openElement(const char* element);
    void addAttribute(const char* attribute, const char* value);
    void addAttribute(const char* attribute, tSigned32 value);
    void addAttribute(const char* attribute, tUnsigned32 value);
    void closeElement(const char* element)
    {
        assertD(_indent > 0);
        _indent--;
    }
};

class cTokenisedXMLOutputStream : public iXMLOutputStream
{
    const cElementAndAttributeEnumerator& _enumerator;
    iOutputStream& _os;
    bool _attributeListClosed;
public:

    cTokenisedXMLOutputStream(const cElementAndAttributeEnumerator& enumerator, iOutputStream& os) :
      _enumerator(enumerator),
      _os(os)
    {
          _attributeListClosed = true;
    }

    void openElement(const char* element);
    void addAttribute(const char* attribute, const char* value);
    void addAttribute(const char* attribute, tSigned32 value);
    void addAttribute(const char* attribute, tUnsigned32 value);
    void closeElement(const char* element);
};

#endif
