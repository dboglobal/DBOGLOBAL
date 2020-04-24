//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PARALLEL_HANDLER_INCLUDED
#define PARALLEL_HANDLER_INCLUDED

#include "libs/ParseXML/interface/iXMLHandler.h"
#include "common/STL/vector.h"

class cParallelHandler : public iXMLHandler
{
    std::vector<iXMLHandler*> _handlers;

public:

    // must only be called during setup
    void addHandler(iXMLHandler* handler)
    {
        _handlers.push_back(handler);
    }

// iXMLHandler

    void init(const char *const* attr)
    {
        tSigned32 i;
        for(i = 0; i < SizeL(_handlers); ++i)
        {
            if(_handlers[i])
            {
                _handlers[i]->init(attr);
            }
        }
    }
    iXMLHandler* element(const char* el, const char *const* attr)
    {
        invalid();
        return 0;
    }
    void elementCompleted(iXMLHandler*)
    {
        invalid();
    }
//    void text(const char* string, int length)
//    {
//        tSigned32 i;
//        for(i = 0; i < SizeL(_handlers); ++i)
//        {
//            _handlers[i]->text(string, length);
//        }
//    }
    void completed()
    {
        invalid();
    }
    iXMLHandler* start(const char* el, const char *const* attr)
    {
        tSigned32 i;
        for(i = 0; i < SizeL(_handlers); ++i)
        {
            _handlers[i] = _handlers[i]->start(el, attr);
        }
        _depth++;
        return this;
    }
    iXMLHandler* end()
    {
        iXMLHandler* parent;
        if(_depth)
        {
            tSigned32 i;
            for(i = 0; i < SizeL(_handlers); ++i)
            {
                _handlers[i] = _handlers[i]->end();
            }
            _depth--;
            parent = this;
        }
        else
        {
            parent = _parent;
            parent->elementCompleted(this);
        }
        return parent;
    }
};

#endif
