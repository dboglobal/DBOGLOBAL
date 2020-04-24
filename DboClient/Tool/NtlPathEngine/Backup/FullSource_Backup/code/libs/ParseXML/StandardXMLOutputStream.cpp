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
#include "interface/StandardXMLOutputStream.h"
#include "platform_common/FloatPersistence.h"
#include "common/interface/iOutputStream_Operators.h"
#include "common/interface/Assert.h"
#include "common/STL/sstream.h"

void
cStandardXMLOutputStream::openElement(const char* element)
{
    if(_hasNoChildren)
    {
    // first child of parent
    // close parent element first
        _os << ">\n";
    }
    tSigned32 i;
    for(i = 0; i < _indent; i++)
    {
        _os << "\t";
    }
    _os << "<";
    _os << element;
    _hasNoChildren = true;
    _indent++;
}

void
cStandardXMLOutputStream::addAttribute(const char* attribute, const char* value)
{
    assertD(_indent);
    assertD(_hasNoChildren);
    _os << " ";
    _os << attribute;
    _os << "=\"";
    _os << value;
    _os << "\"";
}

void
cStandardXMLOutputStream::addAttribute(const char* attribute, tSigned32 value)
{
    std::ostringstream oss;
    oss << value;
    addAttribute(attribute, oss.str().c_str());
}
void
cStandardXMLOutputStream::addAttribute(const char* attribute, tUnsigned32 value)
{
    std::ostringstream oss;
    oss << value;
    addAttribute(attribute, oss.str().c_str());
}

void
cStandardXMLOutputStream::closeElement(const char* element)
{
    assertD(_indent > 0);
    _indent--;
    if(_hasNoChildren)
    {
    // close parent element inline
        _os << "/>\n";
    }
    else
    {
        tSigned32 i;
        for(i = 0; i < _indent; i++)
        {
            _os << "\t";
        }
        _os << "</";
        _os << element;
        _os << ">\n";
    }
    _hasNoChildren = false;
}
