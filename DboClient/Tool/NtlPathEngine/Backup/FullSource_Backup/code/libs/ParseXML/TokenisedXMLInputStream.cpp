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
#include "interface/TokenisedXMLInputStream.h"
#include "common/interface/Error.h"

static void
ParseError(const char* description)
{
    cAttributesBuilder attributes;
    attributes.add("errorcode","13");
    std::string s="Error parsing tokenised XML, ";
    s += description;
    Error("NonFatal", s.c_str(), attributes.get());
}

bool
cTokenisedXMLInputStream::skipString(bool& wasEmpty)
{
    tUnsigned32 startPosition = _bufferPosition;
    while(_bufferPosition < _bufferSize && _buffer[_bufferPosition] != 0)
    {
        _bufferPosition++;
    }
    if(_bufferPosition == _bufferSize)
    {
        return false;
    }
    _bufferPosition++;
    wasEmpty = (_buffer[startPosition] == 0);
    return true;
}

bool
cTokenisedXMLInputStream::readToken(tSigned32 tokenSize, tUnsigned32 &result)
{
    if(_bufferPosition == _bufferSize)
    {
        return false;
    }
    if(tokenSize == 1)
    {
        result = _buffer[_bufferPosition++];
    }
    else
    {
        assertD(tokenSize == 2);
        if(_bufferPosition + 1 == _bufferSize)
        {
            return false;
        }
        result = *(const char*)(_buffer + _bufferPosition + 1);
        result <<= 8;
        result += *(const unsigned char*)(_buffer + _bufferPosition);
        _bufferPosition += 2;
    }
    return true;
}

bool
cTokenisedXMLInputStream::peekToken(tSigned32 tokenSize, tUnsigned32 &result)
{
    if(_bufferPosition == _bufferSize)
    {
        return false;
    }
    if(tokenSize == 1)
    {
        result = _buffer[_bufferPosition];
    }
    else
    {
        assertD(tokenSize == 2);
        if(_bufferPosition + 1 == _bufferSize)
        {
            return false;
        }
        result = *(const signed char*)(_buffer + _bufferPosition + 1);
        result <<= 8;
        result += *(const unsigned char*)(_buffer + _bufferPosition);
    }
    return true;
}

bool
cTokenisedXMLInputStream::readValue(tSigned32& result)
{
    if(_bufferSize < 4)
    {
        return false;
    }
    if(_bufferSize - 4 < _bufferPosition)
    {
        return false;
    }
    result = *(const signed char*)(_buffer + _bufferPosition + 3);
    result <<= 8;
    result += *(const unsigned char*)(_buffer + _bufferPosition + 2);
    result <<= 8;
    result += *(const unsigned char*)(_buffer + _bufferPosition + 1);
    result <<= 8;
    result += *(const unsigned char*)(_buffer + _bufferPosition);
    _bufferPosition += sizeof(result);
    return true;
}

bool
cTokenisedXMLInputStream::readValue(short& result)
{
    if(_bufferSize < 2)
    {
        return false;
    }
    if(_bufferSize - 2 < _bufferPosition)
    {
        return false;
    }
    result = *(const signed char*)(_buffer + _bufferPosition + 1);
    result <<= 8;
    result += *(const unsigned char*)(_buffer + _bufferPosition);
    _bufferPosition += sizeof(result);
    return true;
}

bool
cTokenisedXMLInputStream::readValue(signed char& result)
{
    if(_bufferSize < 1)
    {
        return false;
    }
    if(_bufferSize - 1 < _bufferPosition)
    {
        return false;
    }
    result = *(const signed char*)(_buffer + _bufferPosition);
    _bufferPosition += sizeof(result);
    return true;
}

bool
cTokenisedXMLInputStream::readValue(tUnsigned32& result)
{
    if(_bufferSize < 4)
    {
        return false;
    }
    if(_bufferSize - 4 < _bufferPosition)
    {
        return false;
    }
    result = *(const unsigned char*)(_buffer + _bufferPosition + 3);
    result <<= 8;
    result += *(const unsigned char*)(_buffer + _bufferPosition + 2);
    result <<= 8;
    result += *(const unsigned char*)(_buffer + _bufferPosition + 1);
    result <<= 8;
    result += *(const unsigned char*)(_buffer + _bufferPosition);
    _bufferPosition += sizeof(result);
    return true;
}

bool
cTokenisedXMLInputStream::readValue(unsigned short& result)
{
    if(_bufferSize < 2)
    {
        return false;
    }
    if(_bufferSize - 2 < _bufferPosition)
    {
        return false;
    }
    result = *(const unsigned char*)(_buffer + _bufferPosition + 1);
    result <<= 8;
    result += *(const unsigned char*)(_buffer + _bufferPosition);
    _bufferPosition += sizeof(result);
    return true;
}

bool
cTokenisedXMLInputStream::readValue(unsigned char& result)
{
    if(_bufferSize < 1)
    {
        return false;
    }
    if(_bufferSize - 1 < _bufferPosition)
    {
        return false;
    }
    result = *(const unsigned char*)(_buffer + _bufferPosition);
    _bufferPosition += sizeof(result);
    return true;
}



bool
cTokenisedXMLInputStream::skipValue(tUnsigned32 size)
{
    if(_bufferSize < size)
    {
        return false;
    }
    if(_bufferSize - size < _bufferPosition)
    {
        return false;
    }
    _bufferPosition += size;
    return true;
}

bool
cTokenisedXMLInputStream::skipAttributes()
{
    while(1)
    {
        tUnsigned32 attribute;
        if(!readToken(_attributeSize, attribute))
        {
            ParseError("reached buffer end while reading attribute token.");
            return false;
        }
        if(attribute == 0)
        {
            break;
        }
        attribute--;

        if(attribute >= _attributes.size())
        {
            ParseError("attribute token out of range.");
            return false;
        }

        if(_attributeTypes[attribute] == 1)
        {
            bool wasEmpty;
            if(!skipString(wasEmpty))
            {
                ParseError("reached buffer end while reading attribute value.");
                return false;
            }
        }
        else
        {
            bool readOK;
            switch(_attributeTypes[attribute])
            {
            default:
                invalid();
            case 2:
                readOK = skipValue(4);
                break;
            case 3:
                readOK = skipValue(2);
                break;
            case 4:
                readOK = skipValue(1);
                break;
            }
            if(!readOK)
            {
                ParseError("reached buffer end while reading attribute value.");
                return false;
            }
        }        
    }
    return true;
}

bool
cTokenisedXMLInputStream::skipToCloseElement()
{
    while(1)
    {
        tUnsigned32 elementIndex;
        if(!readToken(_elementSize, elementIndex))
        {
            ParseError("reached buffer end while reading element token.");
            return false;
        }

        if(elementIndex == 0)
        {
            return true;
        }

        // start element
        elementIndex--;
        if(elementIndex >= _elements.size())
        {
            ParseError("element token out of range.");
            return false;
        }

        if(!skipAttributes())
        {
            return false;
        }

      // skip any contained elements

        if(!skipToCloseElement())
        {
            return false;
        }
    }
}

cTokenisedXMLInputStream::cTokenisedXMLInputStream(const char* buffer, tUnsigned32 bufferSize) :
 _buffer(buffer),
 _bufferSize(bufferSize)
{
    _bad = false;
    bool wasEmpty;

// prescan elements and attributes so vectors can be presized

    _bufferPosition = 0;
    tSigned32 elementsSize, attributesSize;
    {
        elementsSize = -1;
        do
        {
            elementsSize++;
            if(!skipString(wasEmpty))
            {
                ParseError("reached buffer end while reading element strings.");
                _bad = true;
                return;
            }
        }
        while(!wasEmpty);
        attributesSize = 0;
        while(1)
        {
            if(_bufferPosition == _bufferSize)
            {
                ParseError("reached buffer end while reading attribute type.");
                _bad = true;
                return;
            }
            int attributeType = _buffer[_bufferPosition++];
            if(attributeType == 0)
            {
                break;
            }
            if(attributeType < 0 || attributeType > 7)
            {
                ParseError("unsupported attribute type.");
                _bad = true;
                return;
            }
            attributesSize++;
            if(!skipString(wasEmpty))
            {
                ParseError("reached buffer end while reading attribute string.");
                _bad = true;
                return;
            }
            if(wasEmpty)
            {
                ParseError("attribute string is empty.");
                _bad = true;
                return;
            }
        }
    }

    _elements.resize(elementsSize);
    _attributes.resize(attributesSize);
    _attributeTypes.resize(attributesSize);

    _bufferPosition = 0;

    tSigned32 i;
    for(i = 0; i < elementsSize; ++i)
    {
        _elements[i] = _buffer + _bufferPosition;
        skipString(wasEmpty);
    }
    _bufferPosition++;

    for(i = 0; i < attributesSize; ++i)
    {
        _attributeTypes[i] = _buffer[_bufferPosition++];
        _attributes[i] = _buffer + _bufferPosition;
        skipString(wasEmpty);
    }
    _bufferPosition++;

    _elementSize = 1;
    if(_elements.size() >= 255)
    {
        _elementSize++;
    }
    assertD(_elements.size() < 65535);

    _attributeSize = 1;
    if(_attributes.size() >= 255)
    {
        _attributeSize++;
    }
    assertD(_attributes.size() < 65535);

    _nestedLevel = 0;
}

const char *const*
cTokenisedXMLInputStream::openElement(const char* elementString)
{
    if(_bad)
    {
        return 0;
    }

    const char* nextElementString;
    while(1)
    {
        nextElementString = peekNextElement();
        if(!nextElementString)
        {
            Error("NonFatal", "cTokenisedXMLInputStream::openElement() - element not found.");
            _bad = true;
            return 0;
        }
        if(strcmp(nextElementString, elementString))
        {
            skipNextElement();
        }
        else
        {
            return openNextElement();
        }
    }
}

void
cTokenisedXMLInputStream::closeElement()
{
    if(_bad)
    {
        return;
    }

    if(_nestedLevel == 0)
    {
        ParseError("too many end elements.");
        _bad = true;
        return;
    }
    _nestedLevel--;


    while(peekNextElement())
    {
        skipNextElement();
    }

    tUnsigned32 elementIndex;
    bool reachedEnd = !readToken(_elementSize, elementIndex);
    assertD(!reachedEnd);
    assertD(elementIndex == 0);
}

const char*
cTokenisedXMLInputStream::peekNextElement()
{
    if(_bad)
    {
        return 0;
    }

    tUnsigned32 elementIndex;
    if(!peekToken(_elementSize, elementIndex))
    {
        ParseError("reached buffer end while reading element token.");
        _bad = true;
        return 0;
    }
    if(elementIndex == 0)
    {
    // current enclosing element ends
        return 0;
    }

    // start element
    elementIndex--;
    if(elementIndex >= _elements.size())
    {
        ParseError("element token out of range.");
        _bad = true;
        return 0;
    }

    return _elements[elementIndex];
}

const char *const*
cTokenisedXMLInputStream::openNextElement()
{
    assertD(!_bad);
    _nestedLevel++;

    tUnsigned32 elementIndex;
    bool reachedEnd = !readToken(_elementSize, elementIndex);
    assertD(!reachedEnd);
    assertD(elementIndex);
    elementIndex--;
    assertD(elementIndex < _elements.size());

    // get attributes
    _ab.clear();
    while(1)
    {
        tUnsigned32 attribute;
        if(!readToken(_attributeSize, attribute))
        {
            ParseError("reached buffer end while reading attribute token.");
            _bad = true;
            return 0;
        }
        if(attribute == 0)
        {
            break;
        }
        attribute--;

        if(attribute >= _attributes.size())
        {
            ParseError("attribute token out of range.");
            _bad = true;
            return false;
        }

        if(_attributeTypes[attribute] == 1)
        {
            _ab.add(_attributes[attribute], _buffer + _bufferPosition);
            bool wasEmpty;
            if(!skipString(wasEmpty))
            {
                ParseError("reached buffer end while reading attribute value.");
                _bad = true;
                return false;
            }
        }
        else
        {
            tSigned32 longValue;
            tUnsigned32 unsignedLongValue;
//            float floatValue;
            bool readOK;
            switch(_attributeTypes[attribute])
            {
            default:
                invalid();
            case 2:
                readOK = readValue(longValue);
                break;
            case 3:
                {
                    short shortValue;
                    readOK = readValue(shortValue);
                    longValue = static_cast<tSigned32>(shortValue);
                    break;
                }
            case 4:
                {
                    signed char charValue;
                    readOK = readValue(charValue);
                    longValue = static_cast<tSigned32>(charValue);
                    break;
                }
            case 5:
                readOK = readValue(unsignedLongValue);
                break;
            case 6:
                {
                    unsigned short shortValue;
                    readOK = readValue(shortValue);
                    unsignedLongValue = static_cast<tUnsigned32>(shortValue);
                    break;
                }
            case 7:
                {
                    unsigned char charValue;
                    readOK = readValue(charValue);
                    unsignedLongValue = static_cast<tUnsigned32>(charValue);
                    break;
                }
//            case 8:
//                readOK = readValue(floatValue);
//                break;
            }
            if(!readOK)
            {
                ParseError("reached buffer end while reading attribute value.");
                return false;
            }
            switch(_attributeTypes[attribute])
            {
            default:
            case 2:
            case 3:
            case 4:
                _ab.add(_attributes[attribute], longValue);
                break;
            case 5:
            case 6:
            case 7:
                _ab.add(_attributes[attribute], unsignedLongValue);
                break;
//            case 8:
//                _ab.add(_attributes[attribute], floatValue);
//                break;
            }
        }
    }
    return _ab.get();    
}

void
cTokenisedXMLInputStream::skipNextElement()
{
    assertD(!_bad);
    _nestedLevel++;

    tUnsigned32 elementIndex;
    bool reachedEnd = !readToken(_elementSize, elementIndex);
    assertD(!reachedEnd);
    assertD(elementIndex);
    elementIndex--;
    assertD(elementIndex < _elements.size());

    if(!skipAttributes())
    {
        _bad = true;
        return;
    }

  // skip any contained elements

    if(!skipToCloseElement())
    {
        _bad = true;
    }
}

bool
cTokenisedXMLInputStream::good()
{
    return !_bad;
}
bool
cTokenisedXMLInputStream::atEnd()
{
    return _bufferSize == _bufferPosition;
}


