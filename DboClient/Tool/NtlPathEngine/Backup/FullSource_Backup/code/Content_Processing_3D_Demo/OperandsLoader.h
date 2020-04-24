//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D_DEMO__OPERANDS_LOADER_INCLUDED
#define CONTENT_PROCESSING_3D_DEMO__OPERANDS_LOADER_INCLUDED

#include "libs/ParseXML/interface/iXMLHandler.h"
#include "common/interface/Attributes.h"

class cOperandsLoader : public iXMLHandler
{
public:

    std::vector<std::vector<tSigned32> > _operands;

// iXMLHandler

    iXMLHandler*
    element(const char* el, const char *const* attr)
    {
        if(strcmp(el, "obstacle3D") == 0)
        {
            _operands.push_back(std::vector<tSigned32>());
            return this;
        }
        if(strcmp(el, "point3D") == 0)
        {
            assertD(!_operands.empty());
            _operands.back().push_back(ValueAsLong(attr, "x"));
            _operands.back().push_back(ValueAsLong(attr, "y"));
            _operands.back().push_back(ValueAsLong(attr, "z"));
            return 0;
        }
        return this;
    }
};

#endif
