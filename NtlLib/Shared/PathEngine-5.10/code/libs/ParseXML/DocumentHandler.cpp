//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/DocumentHandler.h"
#include "common/AttributesBuilder.h"
#include <string.h>

cDocumentHandler::cDocumentHandler(iXMLHandler& rootHandler, const char* expectedRootElement) :
 _rootHandler(rootHandler)
{
    _expectedRootElement = expectedRootElement;
    _matched = false;
    _rootAttributesBuilder = 0;
    _rootAttributes = 0;
}

cDocumentHandler::~cDocumentHandler()
{
    delete _rootAttributesBuilder;
}

iXMLHandler*
cDocumentHandler::element(const char* el, const char *const* attributes)
{
    if(strcmp(el, _expectedRootElement) != 0)
    {
        return 0;
    }
    _matched = true;
    _rootAttributesBuilder = new cAttributesBuilder;
    _rootAttributesBuilder->addCopies(attributes);
    _rootAttributes = _rootAttributesBuilder->get();
    _rootHandler.init(attributes);
    return &_rootHandler;
}

//cSwitchingDocumentHandler::cSwitchingDocumentHandler(
//        iXMLHandler& rootHandler1, const char* expectedRootElement1,
//        iXMLHandler& rootHandler2, const char* expectedRootElement2
//        ) :
// _rootHandler1(rootHandler1),
// _rootHandler2(rootHandler2)
//{
//    _expectedRootElement1 = expectedRootElement1;
//    _expectedRootElement2 = expectedRootElement2;
//    _indexHandled = -1;
//}
//
//iXMLHandler*
//cSwitchingDocumentHandler::element(const char* el, const char *const* attributes)
//{
//    if(strcmp(el, _expectedRootElement1) == 0)
//    {
//        _indexHandled = 0;
//        _rootHandler1.init(attributes);
//        return &_rootHandler1;
//    }
//    if(strcmp(el, _expectedRootElement2) == 0)
//    {
//        _indexHandled = 1;
//        _rootHandler2.init(attributes);
//        return &_rootHandler2;
//    }
//    {
//        const char* attributes[5];
//        attributes[0] = "expectedElement1";
//        attributes[1] = _expectedRootElement1;
//        attributes[0] = "expectedElement2";
//        attributes[1] = _expectedRootElement2;
//        attributes[2] = "encounteredElement";
//        attributes[3] = el;
//        attributes[4] = 0;
//        Error("NonFatal",
//            "cSwitchingDocumentHandler: root element is not identical to either expected root element, skipping.",
//            attributes);
//    }
//    return 0;
//}

cNestedHandler::cNestedHandler(iXMLHandler& childHandler, const char* childElement) :
 _childHandler(childHandler)
{
    _childElement = childElement;
}

iXMLHandler*
cNestedHandler::element(const char* el, const char *const* attributes)
{
    if(strcmp(el, _childElement) == 0)
    {
        _childHandler.init(attributes);
        return &_childHandler;
    }
    return 0;
}
