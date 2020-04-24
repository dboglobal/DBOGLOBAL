//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef DOCUMENT_HANDLER_INCLUDED
#define DOCUMENT_HANDLER_INCLUDED

#include "libs/ParseXML/interface/iXMLHandler.h"

class cAttributesBuilder;

class cDocumentHandler : public iXMLHandler
{
    iXMLHandler& _rootHandler;
    const char* _expectedRootElement;
    bool _matched;
    cAttributesBuilder* _rootAttributesBuilder;
    const char *const * _rootAttributes;

public:

    cDocumentHandler(iXMLHandler& rootHandler, const char* expectedRootElement);
    ~cDocumentHandler();

    bool matched() const
    {
        return _matched;
    }
    const char *const * rootAttributes() const
    {
        return _rootAttributes;
    }

// interface for iXMLHandler

    iXMLHandler* element(const char* el, const char *const* attributes);
};

//.. not currently used
//class cSwitchingDocumentHandler : public iXMLHandler
//{
//    iXMLHandler& _rootHandler1;
//    iXMLHandler& _rootHandler2;
//    const char* _expectedRootElement1;
//    const char* _expectedRootElement2;
//    tSigned32 _indexHandled;
//public:
//
//    cSwitchingDocumentHandler(
//            iXMLHandler& rootHandler1, const char* expectedRootElement1,
//            iXMLHandler& rootHandler2, const char* expectedRootElement2
//            );
//
//    tSigned32
//    indexHandled() const
//    {
//        return _indexHandled;
//    }
//
//// interface for iXMLHandler
//
//    iXMLHandler* element(const char* el, const char *const* attributes);
//    void completed();
//};

class cNestedHandler : public iXMLHandler
{
    iXMLHandler& _childHandler;
    const char* _childElement;
public:

    cNestedHandler(iXMLHandler& childHandler, const char* childElement);

// interface for iXMLHandler

    iXMLHandler* element(const char* el, const char *const* attributes);
};

#endif
