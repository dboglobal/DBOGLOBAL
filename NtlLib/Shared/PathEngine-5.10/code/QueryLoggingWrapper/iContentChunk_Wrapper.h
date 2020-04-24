//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_ICONTENTCHUNK_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_ICONTENTCHUNK_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iContentChunk_Wrapper : public iContentChunk, public iAPIObjectWrapper
{
public:

    typedef iContentChunk tWrapped;

	iContentChunk* _target;

    static tSigned32 interfaceIndex()
    {
        return 10;
    }

    void
    destroy();

    bool
    preValidate(tSigned32 maximumScale, tSigned32 translationRange);

    void
    save(iOutputStream* outputStream) const;

    iContentChunkInstance*
    instantiate(tSigned32 rotation, tSigned32 scale, tSigned32 translationX, tSigned32 translationY, float translationZ, tSigned32 sectionID);

    iContentChunkInstance*
    instantiate_FloatRotateAndScale(float rotation, float scale, tSigned32 translationX, tSigned32 translationY, float translationZ, tSigned32 sectionID);

    void
    deleteAllInstances();

    bool
    hasGroundComponent() const;

    bool
    hasAnchorsAndShapesComponent() const;

    tSigned32
    numberOfConnectingEdges() const;

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
