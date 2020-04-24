//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TOKENISED_XML_INPUT_STREAM_INCLUDED
#define TOKENISED_XML_INPUT_STREAM_INCLUDED

#include "common/interface/iXMLInputStream.h"
#include "common/AttributesBuilder.h"
#include <vector>

class cTokenisedXMLInputStream : public iXMLInputStream
{
    const char* _buffer;
    tUnsigned32 _bufferSize;
    tUnsigned32 _bufferPosition;

    std::vector<const char*> _elements;
    std::vector<const char*> _attributes;
    std::vector<int> _attributeTypes;
    tSigned32 _elementSize;
    tSigned32 _attributeSize;

    tSigned32 _nestedLevel;
    bool _bad;
    cAttributesBuilder _ab;

    bool skipString(bool& wasEmpty);
    bool readToken(tSigned32 tokenSize, tUnsigned32 &result);
    bool peekToken(tSigned32 tokenSize, tUnsigned32 &result);
    bool readValue(tSigned32& result);
    bool readValue(short& result);
    bool readValue(signed char& result);
    bool readValue(tUnsigned32& result);
    bool readValue(unsigned short& result);
    bool readValue(unsigned char& result);
    bool skipValue(tUnsigned32 size);

    bool skipAttributes();
    bool skipToCloseElement();

public:

    cTokenisedXMLInputStream(const char* buffer, tUnsigned32 bufferSize);

// iXMLInputStream

    const char *const* openElement(const char* element);
    void closeElement();
    const char* peekNextElement();
    const char *const* openNextElement();
    void skipNextElement();
    bool good();
    bool atEnd();
};

#endif
