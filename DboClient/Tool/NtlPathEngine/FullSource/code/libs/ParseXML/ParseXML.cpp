//**********************************************************************
//
// Copyright (c) 2002-2003
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/ParseXML.h"
#include "interface/iXMLHandler.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "libs/Expat/interface/Expat.h"
#include "platform_common/ToAndFromByteBuffer.h"
#include "common/interface/Assert.h"
#include "common/interface/Error.h"
#include "common/AttributesBuilder.h"
#include "common/STL/vector.h"
#include "common/STL/sstream.h"
#include "common/CLib/string.h"

static void StartCallBack(void* data, const char* el, const char** attr)
{
    XML_Parser p = (XML_Parser)data;
    iXMLHandler* handler = (iXMLHandler*)(XML_GetUserData(p));
    handler->setCurrentLine(XML_GetCurrentLineNumber(p));
    iXMLHandler* after = handler->start(el, const_cast<const char *const*>(attr));
    XML_SetUserData(p,after);
}
static void EndCallBack(void* data, const char* el)
{
    XML_Parser p = (XML_Parser)data;
    iXMLHandler* handler = (iXMLHandler*)(XML_GetUserData(p));
    handler->setCurrentLine(XML_GetCurrentLineNumber(p));
    iXMLHandler* after = handler->end();
    XML_SetUserData(p,after);
}

bool ParseXML(const char* buffer, tUnsigned32 bufferSize, iXMLHandler& handler)
{
    bool parsed;

    XML_Parser p = XML_ParserCreate(NULL);
    assertR(p);

    XML_SetElementHandler(p, StartCallBack, EndCallBack);
    XML_SetUserData(p, &handler);
    XML_UseParserAsHandlerArg(p);

    parsed=XML_Parse(p, buffer, bufferSize, true)!=0;

    if (!parsed)
    {
        cAttributesBuilder attributes;
        attributes.add("errorcode","8");
        attributes.add("line",XML_GetCurrentLineNumber(p));
        attributes.add("expat_error", XML_ErrorString(XML_GetErrorCode(p)));
        Error("NonFatal", "Error parsing XML.", attributes.get());
        return false;
    }

    XML_ParserFree(p);

    handler.completed();

    return true;
}

void ParseError(const char* description)
{
    cAttributesBuilder attributes;
    attributes.add("errorcode","13");
    std::string s="Error parsing tokenised XML, ";
    s += description;
    Error("NonFatal",s.c_str(),attributes.get());
}

//..... refactor - arrange this as stream input

static bool ReadString(
                const char* buffer, tUnsigned32 bufferSize, 
                tUnsigned32& bufferPosition, 
                std::string& result)
{
    tUnsigned32 start_position = bufferPosition;
    while(bufferPosition < bufferSize && buffer[bufferPosition]!=0)
        bufferPosition++;
    if(bufferPosition == bufferSize)
    {
        return false;
    }
    bufferPosition++;
    result = std::string(buffer+start_position);
    return true;
}
static bool SkipString(
                const char* buffer, tUnsigned32 bufferSize, 
                tUnsigned32& bufferPosition, 
                bool& wasEmpty)
{
    tUnsigned32 start_position = bufferPosition;
    while(bufferPosition < bufferSize && buffer[bufferPosition]!=0)
        bufferPosition++;
    if(bufferPosition == bufferSize)
    {
        return false;
    }
    bufferPosition++;
    wasEmpty = (buffer[start_position] == 0);
    return true;
}

static bool
ReadSigned(
        const char* buffer, tUnsigned32 bufferSize, 
        tUnsigned32& bufferPosition,
        tSigned32 tokenSize,
        tSigned32& result
        )
{
    if(bufferSize - bufferPosition < static_cast<tUnsigned32>(tokenSize))
    {
        return false;
    }
    result = SignedFromByteBuffer(buffer + bufferPosition, tokenSize);
    bufferPosition += tokenSize;
    return true;
}
static bool
ReadUnsigned(
        const char* buffer, tUnsigned32 bufferSize, 
        tUnsigned32& bufferPosition,
        tSigned32 tokenSize,
        tUnsigned32& result
        )
{
    if(bufferSize - bufferPosition < static_cast<tUnsigned32>(tokenSize))
    {
        return false;
    }
    result = UnsignedFromByteBuffer(buffer + bufferPosition, tokenSize);
    bufferPosition += tokenSize;
    return true;
}

bool
ParseTokenisedXML(const char* buffer, tUnsigned32 bufferSize, iXMLHandler& handler)
{
     PROFILER_SCOPE("ParseTokenisedXML actually parsing");
     bool wasEmpty;

// prescan elements and attributes so vectors can be presized

    tUnsigned32 bufferPosition = 0;
    tSigned32 elementsSize, attributesSize;
    {
        elementsSize = -1;
        do
        {
            elementsSize++;
            if(!SkipString(buffer, bufferSize, bufferPosition, wasEmpty))
            {
                ParseError("reached buffer end while reading element strings.");
                return false;
            }
        }
        while(!wasEmpty);
        attributesSize = 0;
        while(1)
        {
            if(bufferPosition == bufferSize)
            {
                ParseError("reached buffer end while reading attribute type.");
                return false;
            }
            int attributeType = buffer[bufferPosition++];
            if(attributeType == 0)
            {
                break;
            }
            if(attributeType < 0 || attributeType > 7)
            {
                ParseError("unsupported attribute type.");
                return false;
            }
            attributesSize++;
            if(!SkipString(buffer, bufferSize, bufferPosition, wasEmpty))
            {
                ParseError("reached buffer end while reading attribute string.");
                return false;
            }
            if(wasEmpty)
            {
                ParseError("attribute string is empty.");
                return false;
            }
        }
    }

    std::vector<const char*> elements(elementsSize);
    std::vector<const char*> attributes(attributesSize);
    std::vector<int> attributeTypes(attributesSize);

    bufferPosition = 0;

    tSigned32 i;
    for(i = 0; i < elementsSize; ++i)
    {
        elements[i] = buffer + bufferPosition;
        SkipString(buffer, bufferSize, bufferPosition, wasEmpty);
    }
    bufferPosition++;

    for(i = 0; i < attributesSize; ++i)
    {
        attributeTypes[i] = buffer[bufferPosition++];
        attributes[i] = buffer + bufferPosition;
        SkipString(buffer, bufferSize, bufferPosition, wasEmpty);
    }
    bufferPosition++;

    tSigned32 element_size = 1;
    if(elements.size()>=255)
        element_size++;
    assertD(elements.size()<65535);

    tSigned32 attribute_size = 1;
    if(attributes.size()>=255)
        attribute_size++;
    assertD(attributes.size()<65535);

    tSigned32 nestedLevel = 0;
    iXMLHandler* currentHandler = &handler;

    cAttributesBuilder ab;

    while(bufferPosition < bufferSize)
    {
        tUnsigned32 element;
        if(!ReadUnsigned(buffer, bufferSize, bufferPosition, element_size, element))
        {
            ParseError("reached buffer end while reading element token.");
            return false;
        }

        if(element == 0)
        {
        // end element
            if(nestedLevel == 0)
            {
                ParseError("too many end elements.");
                return false;
            }
            currentHandler = currentHandler->end();
            nestedLevel--;
            continue;
        }

        // start element
        element--;
        if(element >= elements.size())
        {
            ParseError("element token out of range.");
            return false;
        }

        // get attributes
        ab.clear();
        while(1)
        {
            tUnsigned32 attribute;
            if(!ReadUnsigned(buffer, bufferSize, bufferPosition, attribute_size, attribute))
            {
                ParseError("reached buffer end while reading attribute token.");
                return false;
            }
            if(attribute == 0)
                break;
            attribute--;

            if(attribute >= attributes.size())
            {
                ParseError("attribute token out of range.");
                return false;
            }

            if(attributeTypes[attribute]==1)
            {
                ab.add(attributes[attribute], buffer + bufferPosition);
                if(!SkipString(buffer, bufferSize, bufferPosition, wasEmpty))
                {
                    ParseError("reached buffer end while reading attribute value.");
                    return false;
                }
            }
            else
            {
                tSigned32 longValue;
                tUnsigned32 unsignedLongValue;
//                float floatValue;
                bool read_ok;
                switch(attributeTypes[attribute])
                {
                default:
                    invalid();
                case 2:
                    read_ok = ReadSigned(buffer, bufferSize, bufferPosition, 4, longValue);
                    break;
                case 3:
                    read_ok = ReadSigned(buffer, bufferSize, bufferPosition, 2, longValue);
                    break;
                case 4:
                    read_ok = ReadSigned(buffer, bufferSize, bufferPosition, 1, longValue);
                    break;
                case 5:
                    read_ok = ReadUnsigned(buffer, bufferSize, bufferPosition, 4, unsignedLongValue);
                    break;
                case 6:
                    read_ok = ReadUnsigned(buffer, bufferSize, bufferPosition, 2, unsignedLongValue);
                    break;
                case 7:
                    read_ok = ReadUnsigned(buffer, bufferSize, bufferPosition, 1, unsignedLongValue);
                    break;
//                case 8:
//                    read_ok = ReadFloat(buffer, bufferSize, bufferPosition, floatValue);
//                    break;
                }
                if(!read_ok)
                {
                    ParseError("reached buffer end while reading attribute value.");
                    return false;
                }
                switch(attributeTypes[attribute])
                {
                default:
                case 2:
                case 3:
                case 4:
                    {
                        PROFILER_SCOPE("ParseTokenisedXML integer attribute add");
                        ab.add(attributes[attribute], longValue);
                    }
                    break;
                case 5:
                case 6:
                case 7:
                    {
                        PROFILER_SCOPE("ParseTokenisedXML integer attribute add");
                        ab.add(attributes[attribute], unsignedLongValue);
                    }
                    break;
//                case 8:
//                    ab.add(attributes[attribute], floatValue);
//                    break;
                }
            }
        }

        PROFILER_CHANGES("ParseTokenisedXML calling to handler->start()");
        currentHandler = currentHandler->start(elements[element], ab.get());
        PROFILER_CHANGES("ParseTokenisedXML actually parsing");
        nestedLevel++;
    }

    if(nestedLevel != 0)
    {
        ParseError("reached buffer end without closing all opened elements.");
        return false;
    }

    PROFILER_CHANGES("ParseTokenisedXML calling to handler->completed()");
    handler.completed();
    return true;
}

bool
ParseXML(const char* format, const char* buffer, tUnsigned32 bufferSize, iXMLHandler& handler)
{
    if(strcmp(format, "xml") == 0)
    {
        return ParseXML(buffer, bufferSize, handler);
    }
    else
    {
        assertD(strcmp(format, "tok") == 0);
        return ParseTokenisedXML(buffer, bufferSize, handler);
    }
}

