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
#include "interface/ParseXML.h"
#include "interface/SimpleDOM.h"
#include "interface/iXMLHandler.h"

class cSimpleDOMBuilder : public iXMLHandler
{
    std::vector<cSimpleDOM*> _stack;
    bool _waitingForRoot;

public:

    cSimpleDOMBuilder(cSimpleDOM& result)
    {
        _stack.push_back(&result);
        _waitingForRoot = true;
    }

// iXMLHandler

    virtual iXMLHandler* element(const char* el, const char *const* attributes)
    {
        assertD(!_stack.empty());
        if(_waitingForRoot)
        {
            _waitingForRoot = false;
        }
        else
        {
            cSimpleDOM& parent = *_stack.back();
            _stack.push_back(new cSimpleDOM);
            parent.appendChild(_stack.back());
        }
        _stack.back()->setName(el);
        if(attributes)
        {
            while(*attributes)
            {
                _stack.back()->setAttribute(attributes[0], attributes[1]);
                attributes += 2;
            }
        }
        return this;
    }
    virtual void elementCompleted(iXMLHandler*)
    {
        assertD(!_stack.empty());
        _stack.pop_back();
    }
    virtual void completed()
    {
        assertD(_stack.empty());
    }
};

bool ParseToSimpleDOM(const char* format, const char* buffer, tUnsigned32 bufferSize, cSimpleDOM& result)
{
    cSimpleDOMBuilder builder(result);
    return ParseXML(format, buffer, bufferSize, builder);
}
