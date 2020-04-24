//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ELEMENT_IN_FACE_LOOKUP_DEFINED
#define ELEMENT_IN_FACE_LOOKUP_DEFINED

#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "common/Containers/ReplacementList.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Geometry/interface/tPoint.h"
#include "ElementInFaceLookup_Header.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"

class cElementInFaceEntry
{
public:
    cElementInFaceEntry()
    {
    }
    cElementInFaceEntry(const cCircuitElement* element,
                        tSigned32 faceIndex
                        ) :
      element(element),
      faceIndex(faceIndex)
    {
    }
    const cCircuitElement* element;
    tSigned32 faceIndex;
};

typedef cList_NoSize<cHandle<cMeshCut> >::iterator tCutRegistryOwnerIterator;
typedef cList_NoSize<cHandle<cMeshCut> >::const_iterator tCutRegistryOwnerConstIterator;

class cCutRegistryEntry
{
public:
    cCutRegistryEntry()
    {
    }
    cCutRegistryEntry(
        cCircuitElement* element, tSigned32 faceIndex,
        tCutRegistryOwnerIterator ownerNode) :
      element(element),
      faceIndex(faceIndex),
      ownerNode(ownerNode)
    {
    }
    cCircuitElement* element;
    tSigned32 faceIndex;
    tCutRegistryOwnerIterator ownerNode;
};

class cCutRegistry
{
public:

    typedef std::list<cCutRegistryEntry>::iterator tFaceIterator;

    cCutRegistry(tSigned32 facesInMesh, tSigned32 edgesInMesh);
    ~cCutRegistry();

    tSigned32 getFacesInMesh() const;
    tSigned32 getEdgesInMesh() const;

    // no smart pointers to these cuts should be held externally 
    // the sequences passed in will be cleared
    // entries get added at the front of the face lists
    void addCuts(tMeshCutSequence& toAdd);
    void addCircuits(tMeshCircuits& toAdd);

//    // as above, the sequence passed in will be cleared and the registry takes over ownership for the elements
//    void addCuts(std::vector<cCircuitElement*>& toAdd);

    void splitElement(const cCutRegistryEntry& toSplit, const tLine& splitLine);

    void explodeOwnerIfOwned(const cCutRegistryEntry& entry);

    void rebuildFromFragmentIfNotAccountedFor(const cCutRegistryEntry& entry, bool discard);
    void commitRebuilds();

    bool containsOrphanedEntries() const;

    std::vector<std::list<cCutRegistryEntry> >& refLookup()
    {
        return _lookup;
    }
    const std::vector<std::list<cCutRegistryEntry> >& refLookup() const
    {
        return _lookup;
    }

    bool containsInlineCorner();

    // this call invalidates pointers to entries, including the pointers passed in
    // this is dealt with internally
    void joinInlineCorners(const std::list<cCutRegistryEntry*>& after);
    
    void stripAsCuts(tMeshCutSequence& appendTo);
    void stripAsCircuits(tMeshCircuits& appendTo);

    void removeCutsConnectingToEdge(tEdge e);
    void removeCutsThatArentInternal();

    // should only call certain methods while affected set is non-empty
    // splitElement() can be called
    void markOwnerAsAffected(const cCutRegistryEntry& entry);

    tCutRegistryOwnerIterator unaffectedOwnersBegin();
    tCutRegistryOwnerIterator unaffectedOwnersEnd();

    void clearAffected();
    bool affectedEmpty() const;

    void removeFromUnaffected(tCutRegistryOwnerIterator ownerNode);

    void validate() const;
    void validateElements() const;
    bool allCutsAreInternal() const;

private:

    void validateEntry(const cCutRegistryEntry&) const;

    tFaceIterator lookupElement(size_t faceInMesh, cCircuitElement* element);

    void addElement(cCircuitElement& element, tCutRegistryOwnerIterator ownerNode);
    void orphanElement(cCircuitElement& element);
    void unOrphanElement(cCircuitElement& element, tCutRegistryOwnerIterator ownerNode);

    void joinElement(cCircuitElement& element, tCutRegistryOwnerIterator ownerNode);
    void removeElement(cCircuitElement& element);
    void removeEntriesForElementsInCut(cMeshCut& cut);    

    void removeOwningCut(cCutRegistryEntry&);

    std::vector<std::list<cCutRegistryEntry> > _lookup;
    cList_NoSize<cHandle<cMeshCut> > _owners;
    cList_NoSize<cHandle<cMeshCut> > _ownersAffected;
    tSigned32 _orphanedCount;
    tSigned32 _edgesInMesh;

  // rebuilds waiting to be committed
    cList_NoSize<cHandle<cMeshCut> > _toBeOwned;
    cList_NoSize<cHandle<cMeshCut> > _toBeDiscarded;
};

template <class T> size_t 
CountEntriesInLookup(const std::vector<std::vector<T> >& lookup)
{
    size_t result = 0;
    size_t i;
    for(i = 0; i < lookup.size(); i++)
    {
        result += lookup[i].size();
    }
    return result;
}

#endif
