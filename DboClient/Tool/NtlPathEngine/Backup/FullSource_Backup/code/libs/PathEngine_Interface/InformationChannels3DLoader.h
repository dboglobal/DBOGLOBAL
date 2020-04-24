//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef INFORMATION_CHANNELS_3D_LOADER_INCLUDED
#define INFORMATION_CHANNELS_3D_LOADER_INCLUDED

#include "libs/ParseXML/interface/iXMLHandler.h"
#include "common/STL/vector.h"

class cInformationChannels3DLoader : public iXMLHandler
{
    tSigned32 _facesIn3DMesh;
    std::vector<tSigned32>*& _surfaceTypes;
    std::vector<tSigned32>*& _sectionIDs;
    std::vector<tSigned32>*& _userData;
    tSigned32 _currentIndex;

public:

    cInformationChannels3DLoader(tSigned32 facesIn3DMesh,
                    std::vector<tSigned32>*& surfaceTypes,
                    std::vector<tSigned32>*& sectionIDs,
                    std::vector<tSigned32>*& userData
                    ) :
     _surfaceTypes(surfaceTypes),
     _sectionIDs(sectionIDs),
     _userData(userData)
    {
        _facesIn3DMesh = facesIn3DMesh;
        _surfaceTypes = 0;
        _sectionIDs = 0;
        _userData = 0;
    }

    void processFaceAttributes(tSigned32 faceIndex, const char *const* attributes);

// interface for iXMLHandler

    void init(const char *const* attr);
    iXMLHandler* element(const char* el, const char *const* attributes);
};

#endif
