//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_CHUNK_INCLUDED
#define CONTENT_CHUNK_INCLUDED

#include "i_pathengine.h"
#include "platform_common/Mutex.h"
#include "common/Containers/ReplacementList.h"
#include "common/Containers/VectorWithFreeList.h"
#include "common/STL/vector.h"

class iAPIObjectWrapper;
class iXMLOutputStream;
class iXMLInputStream;

class cContentChunk : public iContentChunk
{
  // immutable, except on construction and deletion
    cList_NoSize<cContentChunk*>::iterator _heldNode;
    const iFaceVertexMesh* _ground;
    const iAnchorsAndPinnedShapes* _anchorsAndShapes;
    bool _deleteBaseObjects;
    tSigned32 _groundRange;
    tSigned32 _anchorsRange;
    tSigned32 _shapesRange;
    std::vector<tSigned32> _connectingEdgeInfo;

    void constructorCommon();

    cMutex _mutex; // covers all mutable member data
    bool _validated;
    tSigned32 _validationMaximumScale, _validationTranslationRange;
    cVectorWithFreeList<iContentChunkInstance*> _instances;

    bool validateGroundAtRotation(tSigned32 rotation) const;
    bool validateAnchorsAndShapesAtRotation(tSigned32 rotation) const;
    void checkPreValidation(tSigned32 scale, tSigned32 translationX, tSigned32 translationY) const;
    void doSave(iXMLOutputStream& xos) const;

public:

    iAPIObjectWrapper* _wrapper;

    // does not take ownership of the base objects passed in to this constructor
    // so these must remain valid over the lifetime of the constructed object, and then be deleted independantly
    cContentChunk(
            const iFaceVertexMesh* ground, const iAnchorsAndPinnedShapes* anchorsAndShapes, const char *const* attributes,
            cList_NoSize<cContentChunk*>::iterator heldNode
            );
    cContentChunk(iXMLInputStream& is, cList_NoSize<cContentChunk*>::iterator heldNode);
    ~cContentChunk();

    void getConnectingEdgeInfo(tSigned32 connectingEdgeIndex, tSigned32& face, tSigned32& firstVertexInFace) const;

    void releaseInstance(tSigned32 index);

// iContentChunk

    void destroy();
    bool preValidate(tSigned32 maximumScale, tSigned32 translationRange);
    void save(iOutputStream* outputStream) const;
    iContentChunkInstance* instantiate(tSigned32 rotation, tSigned32 scale, tSigned32 translationX, tSigned32 translationY, float translationZ, tSigned32 sectionID);
    iContentChunkInstance* instantiate_FloatRotateAndScale(float rotation, float scale, tSigned32 translationX, tSigned32 translationY, float translationZ, tSigned32 sectionID);
    void deleteAllInstances();
    bool hasGroundComponent() const;
    bool hasAnchorsAndShapesComponent() const;
    tSigned32 numberOfConnectingEdges() const;
    void operator delete(void* p) {::operator delete(p);}

//==================
};

inline cContentChunk* FromI(iContentChunk* ptr)
{
    return static_cast<cContentChunk*>(ptr);
}

#endif
