//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_XML_INPUT_STREAM_INCLUDED
#define I_XML_INPUT_STREAM_INCLUDED

class iXMLInputStream
{
public:
    virtual ~iXMLInputStream() {};

    virtual const char *const* openElement(const char* element) = 0;
    virtual void closeElement() = 0;
    virtual const char* peekNextElement() = 0;
    virtual const char *const* openNextElement() = 0;
    virtual void skipNextElement() = 0;
    virtual bool good() = 0;
    virtual bool atEnd() = 0;
};

#endif
