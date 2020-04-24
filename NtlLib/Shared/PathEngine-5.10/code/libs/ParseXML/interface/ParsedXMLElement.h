//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PARSEDXMLELEMENT_INCLUDED
#define PARSEDXMLELEMENT_INCLUDED

#include "common/STL/string.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"
#include "libs/ParseXML/interface/iXMLHandler.h"

//... sort out better accessors
//... contained elements should be in a vector
//... giving out the contained elements pointers is nasty
//... (eg can modify elements owned by a const object)

//....... dump this and use something like cSimpleDOM ;-)

class cParsedXMLElement : public iXMLHandler
{
    std::string m_element;
    std::vector<std::string> m_attributes;
    std::list<cParsedXMLElement*> m_contained_elements;
public:
    
    cParsedXMLElement(const char *el, const char *const* attr);
    ~cParsedXMLElement()
    {
        while(!m_contained_elements.empty())
        {
            delete m_contained_elements.front();
            m_contained_elements.pop_front();
        }
    }
    iXMLHandler* element(const char *el, const char *const* attr)
    {
        m_contained_elements.push_back(new cParsedXMLElement(el,attr));
        return m_contained_elements.back();
    }

    const char *getName() const
    {
        return m_element.c_str();
    }
    const char* getValueForAttribute(const char *attribute) const;
    tSigned32 getLong(const char *str) const;
    const std::vector<std::string>& refAttributes() const
    {
        return m_attributes;
    }

    const cParsedXMLElement* getSubElement(const char *element) const;

    const std::list<cParsedXMLElement*>& refContainedElements() const
    {
        return m_contained_elements;
    }

    void reParse_Init(iXMLHandler* handler) const;
    void reParse(iXMLHandler* startHandler) const;
};

#endif
