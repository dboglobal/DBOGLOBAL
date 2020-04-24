//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/ParsedXMLElement.h"
#include "common/AttributesBuilder.h"
#include "common/interface/Assert.h"
#include "common/CLib/stdlib.h"

cParsedXMLElement::cParsedXMLElement(const char *el, const char *const* attr)
{
    m_element=el;
    while(attr[0])
    {
        assertD(attr[1]);
        m_attributes.push_back(std::string());
        m_attributes.back()=attr[0];
        m_attributes.push_back(std::string());
        m_attributes.back()=attr[1];
        attr+=2;
    }
}

const char*
cParsedXMLElement::getValueForAttribute(const char *attribute) const
{
    tSigned32 i = 0;
    while(i + 2 <= SizeL(m_attributes))
    {
        if(strcmp(attribute,m_attributes[i+0].c_str())==0)
        {
            return m_attributes[i+1].c_str();
        }
        i+=2;
    }
    return 0;
}

tSigned32 cParsedXMLElement::getLong(const char *str) const
{
    const char *value=getValueForAttribute(str);
    assertR(value);
    return strtol(value,0,10);
}

const cParsedXMLElement* cParsedXMLElement::getSubElement(const char *name) const
{
    std::list<cParsedXMLElement*>::const_iterator i;
    for(i=m_contained_elements.begin();i!=m_contained_elements.end();++i)
    {
        if(strcmp((*i)->getName(),name)==0)
            return *i;
    }
    return 0;
}







void
cParsedXMLElement::reParse_Init(iXMLHandler* handler) const
{
    cAttributesBuilder builder;
    builder.addPointers(m_attributes);
    handler->init(builder.get());

    std::list<cParsedXMLElement*>::const_iterator i;
    for(i = m_contained_elements.begin(); i != m_contained_elements.end(); ++i)
    {
        (*i)->reParse(handler);
    }

    handler->completed();
}

void
cParsedXMLElement::reParse(iXMLHandler* startHandler) const
{
    cAttributesBuilder builder;
    builder.addPointers(m_attributes);
    iXMLHandler* handler = startHandler->start(m_element.c_str(), builder.get());

    std::list<cParsedXMLElement*>::const_iterator i;
    for(i = m_contained_elements.begin(); i != m_contained_elements.end(); ++i)
    {
        (*i)->reParse(handler);
    }

    handler = handler->end();
    assertD(handler == startHandler);
}

