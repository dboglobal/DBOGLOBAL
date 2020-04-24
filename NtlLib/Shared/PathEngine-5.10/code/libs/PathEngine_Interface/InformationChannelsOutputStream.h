//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef INFORMATION_CHANNELS_OUTPUT_STREAM
#define INFORMATION_CHANNELS_OUTPUT_STREAM

#include "common/interface/iXMLOutputStream.h"
#include "common/STL/vector.h"

class cInformationChannelsOutputStream : public iXMLOutputStream
{
    iXMLOutputStream& _base;
    tSigned32 _depth;
    bool _inMesh3D;
    tSigned32 _currentIndex;
    std::vector<tSigned32>* _surfaceTypes;
    std::vector<tSigned32>* _sectionIDs;
    std::vector<tSigned32>* _userData;

public:

    cInformationChannelsOutputStream(iXMLOutputStream& base,
                                    std::vector<tSigned32>* surfaceTypes,
                                    std::vector<tSigned32>* sectionIDs,
                                    std::vector<tSigned32>* userData
									) :
      _base(base)
    {
        _depth = 0;
        _inMesh3D = false;
        _surfaceTypes = surfaceTypes;
        _sectionIDs = sectionIDs;
        _userData = userData;
    }

// iXMLOutputStream

    void openElement(const char* element);
    void addAttribute(const char* attribute, const char* value);
    void addAttribute(const char* attribute, tSigned32 value);
    void addAttribute(const char* attribute, tUnsigned32 value);
    void closeElement(const char* element);
};

#endif
