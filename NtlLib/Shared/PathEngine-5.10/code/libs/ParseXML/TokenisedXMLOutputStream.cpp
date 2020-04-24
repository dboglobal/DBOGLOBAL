//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/TokenisedXMLOutputStream.h"
#include "platform_common/FloatPersistence.h"
#include "platform_common/ToAndFromByteBuffer.h"
#include "common/interface/iOutputStream_Operators.h"
#include "common/interface/Assert.h"
#include "common/CLib/stdlib.h"
#include "common/STL/sstream.h"

// ========= stuff relating to the set of types supported

//1 - C string.
//2 - 32 bit signed integer.
//3 - 16 bit signed integer.
//4 - 8 bit signed integer.
//5 - 32 bit unsigned integer.
//6 - 16 bit unsigned integer.
//7 - 8 bit unsigned integer.

static tSigned32
SizeOfType(char type)
{
    const tSigned32 sizes[] = {0, 5, 4, 2, 1, 4, 2, 1};
    return sizes[type];
}

static char
SmallestTypeFor(tSigned32 value)
{
    char result = 2;
    if(value <= 0x7fff && value >= -0x8000)
    {
        result = 3;
        if(value <= 0x7f && value >= -0x80)
        {
            result = 4;
        }
    }
    return result;
}
static char
SmallestTypeFor(tUnsigned32 value)
{
    char result = 5;
    if(value <= 0xffffUL)
    {
        result = 6;
        if(value <= 0xffUL)
        {
            result = 7;
        }
    }
    return result;
}

static char
SmallestSignedTypeFor(const char* value)
{
    char* ptr;
    tSigned32 result = strtol(value, &ptr, 10);
    if(*ptr != 0)
    {
        return 0;
    }
    std::ostringstream oss;
    oss << result;
    if(strcmp(value, oss.str().c_str()))
    {
        return 0;
    }
//    if(result > 0x7fffffffL || result < -0x7fffffffL - 1)
//    {
//        return 0;
//    }
    return SmallestTypeFor(result);
}
static char
SmallestUnsignedTypeFor(const char* value)
{
    char* ptr;
    tUnsigned32 result = strtoul(value, &ptr, 10);
    if(*ptr != 0)
    {
        return 0;
    }
    std::ostringstream oss;
    oss << result;
    if(strcmp(value, oss.str().c_str()))
    {
        return 0;
    }
//    if(result > 0xffffffffL)
//    {
//        return 0;
//    }
    return SmallestTypeFor(result);
}

static char
SmallestTypeFor(const char* value)
{
    char smallest = 1;
    char candidate;
    candidate = SmallestSignedTypeFor(value);
    if(candidate && SizeOfType(candidate) < SizeOfType(smallest))
    {
        smallest = candidate;
    }
    candidate = SmallestUnsignedTypeFor(value);
    if(candidate && SizeOfType(candidate) < SizeOfType(smallest))
    {
        smallest = candidate;
    }
    return smallest;
}

static char
MostExpressiveType_GT(char type1, char type2)
{
    switch(type1)
    {
    default:
        invalid();
    case 1:
        return 1;
    case 2:
        {
            if(type2 == 3 || type2 == 4 || type2 == 6 || type2 == 7)
            {
                return type1;
            }
            return 1;
        }
    case 3:
        {
            if(type2 == 4 || type2 == 7)
            {
                return type1;
            }
//            if(type2 == 5 || type2 == 8)
            if(type2 == 5)
            {
                return type2;
            }
            return 1;
        }
    case 4:
        {
//            if(type2 == 5 || type2 == 6 || type2 == 8)
            if(type2 == 5 || type2 == 6)
            {
                return type2;
            }
            return 1;
        }
    case 5:
        {
            if(type2 == 6 || type2 == 7)
            {
                return type1;
            }
            return 1;
        }
    case 6:
        {
            assertD(type2 == 7);
            return type1;
//            if(type2 == 7)
//            {
//                return type1;
//            }
//            if(type2 == 8)
//            {
//                return type2;
//            }
//            return 1;
        }
//    case 7:
//        {
//            assertD(type2 == 8);
//            return type2;
//        }
    }   
}
static char
MostExpressiveType(char type1, char type2)
{
    if(type1 > type2)
    {
        return MostExpressiveType_GT(type2, type1);
    }
    return MostExpressiveType_GT(type1, type2);
}

static tSigned32
As32BitSigned(const char* value)
{
    char* ptr;
    tSigned32 result = strtol(value, &ptr, 10);
    assertD(*ptr == 0);
    return result;
}
static tUnsigned32
As32BitUnsigned(const char* value)
{
    char* ptr;
    tUnsigned32 result = strtoul(value, &ptr, 10);
    assertD(*ptr == 0);
    return result;
}

static void
OutputAsType(const char* value, char type, iOutputStream& os)
{
    if(type == 1)
    {
        os << value;
        os << char(0);
        return;
    }
    char buffer[4];
    tSigned32 size = SizeOfType(type);
    if(type >= 2 && type <= 4)
    {
        tSigned32 l = As32BitSigned(value);
        SignedToByteBuffer(l, size, buffer);
    }
    else
    {
        assertD(type >= 5 && type <= 7);
        unsigned ul = As32BitUnsigned(value);
        UnsignedToByteBuffer(ul, size, buffer);
    }
    os.put(buffer, size);
}

static void
OutputAsType(tSigned32 value, char type, iOutputStream& os)
{
    char asBytes[4];
    switch(type)
    {
    default:
        invalid();
    case 1:
        {
        // convert to string
            std::ostringstream oss;
            oss << value;
            os << oss.str().c_str();
            os << char(0);
        }
        break;
    case 2:
        SignedToByteBuffer(value, 4, asBytes);
        os.put(asBytes, 4);
        break;
    case 3:
        SignedToByteBuffer(value, 2, asBytes);
        os.put(asBytes, 2);
        break;
    case 4:
        SignedToByteBuffer(value, 1, asBytes);
        os.put(asBytes, 1);
        break;
    case 5:
        UnsignedToByteBuffer(static_cast<tUnsigned32>(value), 4, asBytes);
        os.put(asBytes, 4);
        break;
    case 6:
        UnsignedToByteBuffer(static_cast<tUnsigned32>(value), 2, asBytes);
        os.put(asBytes, 2);
        break;
//    case 8:
//        FloatToByteBuffer(static_cast<float>(value), asBytes);
//        os.put(asBytes, 4);
//        break;
    }
}

static void
OutputAsType(tUnsigned32 value, char type, iOutputStream& os)
{
    char asBytes[4];
    switch(type)
    {
    default:
        invalid();
    case 1:
        {
        // convert to string
            std::ostringstream oss;
            oss << value;
            os << oss.str().c_str();
            os << char(0);
        }
        break;
    case 2:
        SignedToByteBuffer(static_cast<tSigned32>(value), 4, asBytes);
        os.put(asBytes, 4);
        break;
    case 3:
        SignedToByteBuffer(static_cast<tSigned32>(value), 2, asBytes);
        os.put(asBytes, 2);
        break;
    case 5:
        UnsignedToByteBuffer(value, 4, asBytes);
        os.put(asBytes, 4);
        break;
    case 6:
        UnsignedToByteBuffer(value, 2, asBytes);
        os.put(asBytes, 2);
        break;
    case 7:
        UnsignedToByteBuffer(value, 1, asBytes);
        os.put(asBytes, 1);
        break;
//    case 8:
//        FloatToByteBuffer(static_cast<float>(value), asBytes);
//        os.put(asBytes, 4);
//        break;
    }
}

//static void
//OutputAsType(float value, char type, iOutputStream& os)
//{
//    char asBytes[4];
//    switch(type)
//    {
//    default:
//        invalid();
//    case 1:
//        {
//        // convert to string
//            std::string persistent = FloatToPersistent(value);
//            os << persistent.c_str();
//            os << char(0);
//        }
//        break;
//    case 3:
//        SignedToByteBuffer(static_cast<tSigned32>(value), 2, asBytes);
//        os.put(asBytes, 2);
//        break;
//    case 4:
//        SignedToByteBuffer(static_cast<tSigned32>(value), 1, asBytes);
//        os.put(asBytes, 1);
//        break;
//    case 6:
//        UnsignedToByteBuffer(static_cast<tUnsigned32>(value), 2, asBytes);
//        os.put(asBytes, 2);
//        break;
//    case 7:
//        UnsignedToByteBuffer(static_cast<tUnsigned32>(value), 1, asBytes);
//        os.put(asBytes, 1);
//        break;
//    case 8:
//        FloatToByteBuffer(static_cast<float>(value), asBytes);
//        os.put(asBytes, 4);
//        break;
//    }
//}

// ========= end of stuff relating to the set of types supported

void
cElementAndAttributeEnumerator::AddOrUpdateAttributeAndType(const char* attribute, char type)
{
    tSigned32 i;
    for(i = 0; i != SizeL(_attributes); ++i)
    {
        if(strcmp(attribute, _attributes[i].c_str()) == 0)
        {
            break;
        }
    }
    if(i == SizeL(_attributes))
    {
      // not encountered yet, simply add with the specified type
        _attributes.push_back(std::string(attribute));
        _types.push_back(type);
        return;
    }
    if(type == _types[i])
    {
        return;
    }
    _types[i] = MostExpressiveType(type, _types[i]);
}

void
cElementAndAttributeEnumerator::openElement(const char* element)
{
    _indent++;
    tSigned32 i;
    for(i = 0; i < SizeL(_elements); i++)
    {
        if(strcmp(element, _elements[i].c_str()) == 0)
        {
            return;
        }
    }
    _elements.push_back(std::string(element));
}
void
cElementAndAttributeEnumerator::addAttribute(const char* attribute, const char* value)
{
    char type = SmallestTypeFor(value);
    AddOrUpdateAttributeAndType(attribute, type);
}
void
cElementAndAttributeEnumerator::addAttribute(const char* attribute, tSigned32 value)
{
    char type = SmallestTypeFor(value);
    AddOrUpdateAttributeAndType(attribute, type);
}
void
cElementAndAttributeEnumerator::addAttribute(const char* attribute, tUnsigned32 value)
{
    char type = SmallestTypeFor(value);
    AddOrUpdateAttributeAndType(attribute, type);
}
//void
//cElementAndAttributeEnumerator::addAttribute(const char* attribute, float value)
//{
//    //.... could check here whether value could be represented by an integer type
//    AddOrUpdateAttributeAndType(attribute, 8);
//}

void
cElementAndAttributeEnumerator::writeTo(iOutputStream& os)
{
    size_t i;
    for(i = 0; i < _elements.size(); i++)
    {
        os << _elements[i].c_str();
        os << char(0);
    }
    os << char(0);
    for(i = 0; i < _attributes.size(); i++)
    {
        os << _types[i];
        os << _attributes[i].c_str();
        os << char(0);
    }
    os << char(0);
}

static char
GetIndexFor(const std::vector<std::string> lookup, const char* value)
{
    size_t i;
    for(i = 0; i < lookup.size(); i++)
    {
        assertD(i < 126);
        if(strcmp(value, lookup[i].c_str()) == 0)
        {
            return static_cast<char>(i + 1);
        }
    }
    invalid();
    return 1;
}

void
cTokenisedXMLOutputStream::openElement(const char* element)
{
    if(!_attributeListClosed)
    {
        _os << char(0);
    }
    char index = GetIndexFor(_enumerator._elements, element);
    _os << index;
    _attributeListClosed = false;
}
void
cTokenisedXMLOutputStream::addAttribute(const char* attribute, const char* value)
{
    assertD(!_attributeListClosed);
    char index = GetIndexFor(_enumerator._attributes, attribute);
    _os << index;
    OutputAsType(value, _enumerator._types[index - 1], _os);
}
void
cTokenisedXMLOutputStream::addAttribute(const char* attribute, tSigned32 value)
{
    assertD(!_attributeListClosed);
    char index = GetIndexFor(_enumerator._attributes, attribute);
    _os << index;
    OutputAsType(value, _enumerator._types[index - 1], _os);
}
void
cTokenisedXMLOutputStream::addAttribute(const char* attribute, tUnsigned32 value)
{
    assertD(!_attributeListClosed);
    char index = GetIndexFor(_enumerator._attributes, attribute);
    _os << index;
    OutputAsType(value, _enumerator._types[index - 1], _os);
}
//void
//cTokenisedXMLOutputStream::addAttribute(const char* attribute, float value)
//{
//    assertD(!_attributeListClosed);
//    char index = GetIndexFor(_enumerator._attributes, attribute);
//    _os << index;
//    OutputAsType(value, _enumerator._types[index - 1], _os);
//}

void
cTokenisedXMLOutputStream::closeElement(const char* element)
{
    if(!_attributeListClosed)
    {
        _os << char(0);
    }
    _os << char(0);
    _attributeListClosed = true;
}
