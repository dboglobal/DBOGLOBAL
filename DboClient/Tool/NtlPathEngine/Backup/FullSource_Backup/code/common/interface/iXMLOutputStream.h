//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_XMLOUTPUTSTREAM_HAS_BEEN_INCLUDED
#define I_XMLOUTPUTSTREAM_HAS_BEEN_INCLUDED
class iXMLOutputStream
{
public:
    virtual ~iXMLOutputStream() {};

    virtual void openElement(const char* element)=0;
    virtual void addAttribute(const char* attribute, const char* value)=0;
    virtual void addAttribute(const char* attribute, tSigned32 value)=0;
    virtual void addAttribute(const char* attribute, tUnsigned32 value)=0;
    virtual void closeElement(const char* element)=0;

    void addAttributes(const char *const* attributes)
    {
        if(!attributes)
        {
            return;
        }
        while(*attributes)
        {
            addAttribute(attributes[0], attributes[1]);
            attributes += 2;
        }
    }
};
#endif
