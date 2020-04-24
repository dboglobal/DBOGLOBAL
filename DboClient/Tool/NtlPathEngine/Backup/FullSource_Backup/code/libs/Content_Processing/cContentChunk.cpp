//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "libs/PathEngine_Interface/interface/cContentChunk.h"
#include "interface/LoadedFaceVertexMesh.h"
#include "interface/SaveFaceVertexMesh.h"
#include "SaveAnchorsAndPinnedShapes.h"
#include "InstancedFaceVertexMesh.h"
#include "interface/InstancedFaceVertexMesh_Float.h"
#include "InstancedAnchorsAndShapes.h"
#include "InstancedAnchorsAndShapes_Float.h"
#include "QuantisedRotation.h"
#include "interface/AnchorsAndPinnedShapes.h"
#include "interface/Export.h"
#include "libs/PathEngine_Interface/interface/cPathEngine.h"
#include "libs/PathEngine_Interface/interface/Singletons.h"
#include "libs/PathEngine_Interface/interface/cContentChunkInstance.h"
#include "libs/PathEngine_Core/interface/Version.h"
#include "libs/ParseXML/interface/TokenisedXMLOutputStream.h"
#include "libs/ParseXML/interface/XMLInputStreamToHandler.h"
#include "iAPIObjectWrapper.h"
#include "platform_common/CoordinateRanges.h"
#include "common/MutexLock.h"
#include "common/interface/Attributes.h"
#include "common/interface/iXMLInputStream.h"
#include "common/interface/Assert.h"
#include "common/interface/Error.h"

static void
UpdateRange(tSigned32& range, tSigned32 value)
{
    if(value > range)
    {
        range = value;
    }
    if(value < -range)
    {
        range = -value;
    }
}

void
cContentChunk::constructorCommon()
{
    _wrapper = 0;
    _groundRange = 0;
    _anchorsRange = 0;
    _shapesRange = 0;
    tSigned32 i;
    if(_ground)
    {
        for(i = 0; i != _ground->vertices(); ++i)
        {
            UpdateRange(_groundRange, _ground->vertexX(i));
            UpdateRange(_groundRange, _ground->vertexY(i));
        }
    }
    if(_anchorsAndShapes)
    {
        for(i = 0; i != _anchorsAndShapes->numberOfAnchors(); ++i)
        {
            tSigned32 x, y;
            _anchorsAndShapes->anchor_HorizontalPosition(i, x, y);
            UpdateRange(_anchorsRange, x);
            UpdateRange(_anchorsRange, y);
        }
        for(i = 0; i != _anchorsAndShapes->numberOfPinnedShapes(); ++i)
        {
            for(tSigned32 j = 0; j != _anchorsAndShapes->pinnedShape_NumberOfVertices(i); ++j)
            {
                tSigned32 x, y;
                _anchorsAndShapes->pinnedShape_Vertex(i, j, x, y);
                UpdateRange(_shapesRange, x);
                UpdateRange(_shapesRange, y);
            }
        }
    }

  // get the set of connecting edges
  // i.e. the set of ground edges that share vertex pairs with connecting edge marker faces

    if(_ground)
    {
      // mark all vertices that are on non-vertical edges of marker faces,
      // and store indices for next vertex around the marker

        std::vector<tSigned32> connectingEdgeNext(_ground->vertices() * 2, -1);
        for(i = 0; i != _ground->faces(); ++i)
        {
            if(_ground->faceAttribute(i, PE_FaceAttribute_MarksConnectingEdge) != 1)
            {
                continue;
            }
            tSigned32 j;
            for(j = 0; j != 3; ++j)
            {
                tSigned32 nextJ = (j + 1) % 3;
                tSigned32 v = _ground->vertexIndex(i, j);
                tSigned32 nextV = _ground->vertexIndex(i, nextJ);
                if(_ground->vertexX(v) == _ground->vertexX(nextV) && _ground->vertexY(v) == _ground->vertexY(nextV))
                {
                  // vertical edge
                    continue;
                }
                
                if(connectingEdgeNext[v * 2] == -1)
                {
                    connectingEdgeNext[v * 2] = nextV;
                }
                else
                {
                    if(connectingEdgeNext[v * 2 + 1] != -1)
                    {
                        Error("Warning", "Unexpected connection edge marker configuration encountered while scanning ground during content chunk construction."); 
                    }
                    connectingEdgeNext[v * 2 + 1] = nextV;
                }
            }
        }

      // and then scan through the ground for faces that connect to these
        for(i = 0; i != _ground->faces(); ++i)
        {
            if(_ground->faceAttribute(i, PE_FaceAttribute_MarksConnectingEdge) == 1)
            {
                continue;
            }
            if(_ground->faceAttribute(i, PE_FaceAttribute_MarksPortal) == 1)
            {
                continue;
            }
            if(_ground->faceAttribute(i, PE_FaceAttribute_MarksTerrainPortal) == 1)
            {
                continue;
            }
            if(_ground->faceAttribute(i, PE_FaceAttribute_MarksExternalShape) == 1)
            {
                continue;
            }
            assertC(PE_FaceAttribute_Top == 6); // if other portal markers or similar are added then further clauses may need to be added above
            tSigned32 j;
            for(j = 0; j != 3; ++j)
            {
                tSigned32 nextJ = (j + 1) % 3;
                tSigned32 v = _ground->vertexIndex(i, j);
                tSigned32 nextV = _ground->vertexIndex(i, nextJ);
                if(connectingEdgeNext[v * 2] == nextV
                 || connectingEdgeNext[v * 2 + 1] == nextV
                 || connectingEdgeNext[nextV * 2] == v
                 || connectingEdgeNext[nextV * 2 + 1] == v
                 )
                {
                    _connectingEdgeInfo.push_back(i);
                    _connectingEdgeInfo.push_back(j);
                }
            }
        }
    }
}

bool
cContentChunk::validateGroundAtRotation(tSigned32 rotation) const
{
    if(_ground == 0)
    {
        return true;
    }
    cInstancedFaceVertexMesh rotated(_ground, rotation, 1, 0, 0, 0.f);
    const char* options[3];
    options[0] = "processConnections";
    options[1] = "false";
    options[2] = 0;
    iMesh* mesh = nSingletons::pathEngine().buildMeshFromContent(&_ground, 1, options);
    bool result = (mesh != 0);
    delete mesh;
    return result;
}
bool
cContentChunk::validateAnchorsAndShapesAtRotation(tSigned32 rotation) const
{
    if(_anchorsAndShapes == 0)
    {
        return true;
    }
    cInstancedAnchorsAndShapes rotated(_anchorsAndShapes, rotation, 1, 0, 0, 0.f);
    return PrevalidateAnchorsAndPinnedShapes(nSingletons::pathEngine(), rotated);
}

void
cContentChunk::checkPreValidation(tSigned32 scale, tSigned32 translationX, tSigned32 translationY) const
{
    cMutexLock lock(_mutex);
    if(!_validated)
    {
//.... this is a pain where content chunks are used effectively as a way to load content snapshots for either buildMeshFromContent() or buildMeshFrom3DContent()
//.... if we sort out an alternative way to load content snapshots then this can be reinstated
//        Error("Warning", "iContentChunk instanced using integer rotate and scale, but was not prevalidated.");
    }
    else if(scale > _validationMaximumScale)
    {
        Error("Warning", "iContentChunk instanced with scale exceeding the maximum scale specified at prevalidation.");
    }
    else if(translationX > _validationTranslationRange || translationX < -_validationTranslationRange || translationY > _validationTranslationRange || translationY < -_validationTranslationRange)
    {
        Error("Warning", "iContentChunk instanced with translation offset exceeding the range specified at prevalidation.");
    }
}

void
cContentChunk::doSave(iXMLOutputStream& xos) const
{
    cMutexLock lock(_mutex);
    xos.openElement("content");
    xos.addAttribute("majorRelease", MajorRelease());
    xos.addAttribute("minorRelease", MinorRelease());
    if(_validated)
    {
        xos.addAttribute("validationMaximumScale", _validationMaximumScale);
        xos.addAttribute("validationTranslationRange", _validationTranslationRange);
    }
    if(_ground)
    {
        xos.openElement("mesh");
        SaveFaceVertexMesh(*_ground, xos);
        xos.closeElement("mesh");
    }
    if(_anchorsAndShapes)
    {
        SaveAnchorsAndPinnedShapes(*_anchorsAndShapes, xos);
    }
    xos.closeElement("content");
}

cContentChunk::cContentChunk(
            const iFaceVertexMesh* ground, const iAnchorsAndPinnedShapes* anchorsAndShapes, const char *const* attributes,
            cList_NoSize<cContentChunk*>::iterator heldNode
            )
{
    INTERFACE_ASSERT(ground || anchorsAndShapes);
    INTERFACE_ASSERT(ground == 0 || ground->faces() > 0);
    INTERFACE_ASSERT(anchorsAndShapes == 0 || anchorsAndShapes->numberOfAnchors() > 0);
    _heldNode = heldNode;
    _ground = ground;
    _anchorsAndShapes = anchorsAndShapes;
    _deleteBaseObjects = false;
    _validated = false;
    _validationMaximumScale = 0;
    _validationTranslationRange = 0;
    constructorCommon();
}
cContentChunk::cContentChunk(iXMLInputStream& is, cList_NoSize<cContentChunk*>::iterator heldNode)
{
    _heldNode = heldNode;
    _deleteBaseObjects = true;
    const char *const* attributes = is.openElement("content");
    _validated = false;
    _validationMaximumScale = 0;
    _validationTranslationRange = 0;
    if(GetValueForAttribute(attributes, "validationMaximumScale"))
    {
        _validated = true;
        _validationMaximumScale = ValueAsLong(attributes, "validationMaximumScale");
        _validationTranslationRange = ValueAsLong(attributes, "validationTranslationRange");
    }
    _ground = 0;
    _anchorsAndShapes = 0;
    const char* nextElement = is.peekNextElement();
    while(nextElement)
    {
        if(strcmp(nextElement, "mesh") == 0)
        {
            const char *const* attributes = is.openNextElement();
            if(_ground)
            {
                Error("NonFatal", "Content chunk being loaded has more than one mesh element."
                    " Looks like the more general 'content snapshot' format."
                    " This will be loaded, but meshes after the first will be ignored.");
            }
            else
            {
                cLoadedFaceVertexMesh* ground = new cLoadedFaceVertexMesh();
                _ground = ground;
                bool ok = XMLInputStreamToHandler(is, attributes, *ground);
                assertD(ok);
            }
            is.closeElement();
        }
        else
        if(strcmp(nextElement, "anchorsAndPinnedShapes") == 0)
        {
            const char *const* attributes = is.openNextElement();
            assertD(!_anchorsAndShapes);
            cAnchorsAndPinnedShapes* anchorsAndShapes = new cAnchorsAndPinnedShapes;
            _anchorsAndShapes = anchorsAndShapes;
            bool ok = XMLInputStreamToHandler(is, attributes, *anchorsAndShapes);
            assertD(ok);
            is.closeElement();
        }
        else
        {
            is.skipNextElement();
        }
        nextElement = is.peekNextElement();
    }
    is.closeElement();
    constructorCommon();
}
cContentChunk::~cContentChunk()
{
    assertD(!_heldNode.valid());
    deleteAllInstances();
    if(_deleteBaseObjects)
    {
        delete _ground;
        delete _anchorsAndShapes;
    }
}

void
cContentChunk::getConnectingEdgeInfo(tSigned32 connectingEdgeIndex, tSigned32& face, tSigned32& firstVertexInFace) const
{
    assertD(connectingEdgeIndex >=0 && connectingEdgeIndex < numberOfConnectingEdges());
    face = _connectingEdgeInfo[connectingEdgeIndex * 2];
    firstVertexInFace = _connectingEdgeInfo[connectingEdgeIndex * 2 + 1];
}

void
cContentChunk::releaseInstance(tSigned32 index)
{
    cMutexLock lock(_mutex);
    _instances.release(index);    
}

void 
cContentChunk::destroy()
{
    INTERFACE_ASSERT(_heldNode.valid());
    if(_wrapper) {_wrapper->destroy_APIObjectWrapper();}
    nSingletons::pathEngine().lockHeldContentChunks();
    _heldNode.eraseAndSetInvalid();
    nSingletons::pathEngine().unlockHeldContentChunks();
    delete this;
}
bool
cContentChunk::preValidate(tSigned32 maximumScale, tSigned32 translationRange)
{
    INTERFACE_ASSERT(maximumScale >= 1);
    INTERFACE_ASSERT(translationRange >= 0);

    if(_groundRange > MaximumValueSupportedByQuantisedRotation())
    {
        Error("NonFatal", "Content chunk ground geometry exceeds the range supported for integer rotation.");
        return false;
    }
    if(_anchorsRange > MaximumValueSupportedByQuantisedRotation())
    {
        Error("NonFatal", "Content chunk anchors exceed the range supported for integer rotation.");
        return false;
    }
    if(_shapesRange > MaximumValueSupportedByQuantisedRotation())
    {
        Error("NonFatal", "Content chunk shape vertices exceed the range supported for integer rotation.");
        return false;
    }
    double transformed;
    transformed = _groundRange;
    transformed *= maximumScale;
    transformed += translationRange;
    if(transformed > WORLD_RANGE)
    {
        Error("NonFatal", "Scaling or translation could put ground geometry outside world range.");
        return false;
    }
    transformed = _anchorsRange;
    transformed *= maximumScale;
    transformed += translationRange;
    if(transformed > WORLD_RANGE)
    {
        Error("NonFatal", "Scaling or translation could put anchors outside world range.");
        return false;
    }
    transformed = _shapesRange;
    transformed *= maximumScale;
    if(transformed > WORLD_RANGE)
    {
        Error("NonFatal", "Scaling or translation could put shape vertices outside world range.");
        return false;
    }

    tSigned32 rotation;
    for(rotation = 0; rotation != QuantisedRotationNumberOfRotations() / 4; ++rotation)
    {
        if(!validateGroundAtRotation(rotation))
        {
            return false;
        }
        if(!validateAnchorsAndShapesAtRotation(rotation))
        {
            return false;
        }
    }

    cMutexLock lock(_mutex);

    _validated = true;
    if(maximumScale > _validationMaximumScale)
    {
        _validationMaximumScale = maximumScale;
    }
    if(translationRange > _validationTranslationRange)
    {
        _validationTranslationRange = translationRange;
    }

    return true;
}
void 
cContentChunk::save(iOutputStream* outputStream) const
{
    cElementAndAttributeEnumerator enumerator;
    doSave(enumerator);
    enumerator.writeTo(*outputStream);
    cTokenisedXMLOutputStream xos(enumerator, *outputStream);
    doSave(xos);
}
iContentChunkInstance*
cContentChunk::instantiate(tSigned32 rotation, tSigned32 scale, tSigned32 translationX, tSigned32 translationY, float translationZ, tSigned32 sectionID)
{
    checkPreValidation(scale, translationX, translationY);
    iFaceVertexMesh* faceVertex = 0;
    iAnchorsAndPinnedShapes* anchorsAndShapes = 0;
    if(_ground)
    {
        faceVertex = new cInstancedFaceVertexMesh(_ground, rotation, scale, translationX, translationY, translationZ, sectionID);
    }
    if(_anchorsAndShapes)
    {
        anchorsAndShapes = new cInstancedAnchorsAndShapes(_anchorsAndShapes, rotation, scale, translationX, translationY, translationZ);
    }
    cMutexLock lock(_mutex);
    tSigned32 allocatedIndex = _instances.reuseOrPushBack(0);
    _instances[allocatedIndex] = new cContentChunkInstance(this, allocatedIndex, faceVertex, _connectingEdgeInfo, anchorsAndShapes, sectionID);
    return _instances[allocatedIndex];
}
iContentChunkInstance*
cContentChunk::instantiate_FloatRotateAndScale(float rotation, float scale, tSigned32 translationX, tSigned32 translationY, float translationZ, tSigned32 sectionID)
{
    iFaceVertexMesh* faceVertex = 0;
    iAnchorsAndPinnedShapes* anchorsAndShapes = 0;
    if(_ground)
    {
        faceVertex = new cInstancedFaceVertexMesh_Float(_ground, rotation, scale, translationX, translationY, translationZ, sectionID);
    }
    if(_anchorsAndShapes)
    {
        anchorsAndShapes = new cInstancedAnchorsAndShapes_Float(_anchorsAndShapes, rotation, scale, translationX, translationY, translationZ);
    }
    cMutexLock lock(_mutex);
    tSigned32 allocatedIndex = _instances.reuseOrPushBack(0);
    _instances[allocatedIndex] = new cContentChunkInstance(this, allocatedIndex, faceVertex, _connectingEdgeInfo, anchorsAndShapes, sectionID);
    return _instances[allocatedIndex];
}

void 
cContentChunk::deleteAllInstances()
{
    cMutexLock lock(_mutex);
    tSigned32 i;
    for(i = 0; i != _instances.size(); ++i)
    {
        if(_instances.indexIsInUse(i))
        {
            _instances[i]->destroy();
        }
    }
}

bool
cContentChunk::hasGroundComponent() const
{
    return _ground != 0;
}
bool
cContentChunk::hasAnchorsAndShapesComponent() const
{
    return _anchorsAndShapes != 0;
}

tSigned32
cContentChunk::numberOfConnectingEdges() const
{
    return SizeL(_connectingEdgeInfo) / 2;
}
