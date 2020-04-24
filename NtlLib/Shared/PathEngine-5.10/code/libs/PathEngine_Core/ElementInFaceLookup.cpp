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
#include "ElementInFaceLookup.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Instrumentation/interface/Profiler.h"

void
LookupElementsInFace(tFace f, const tMeshCutSequence& circuit, std::vector<cElementInFaceEntry>& result)
{
    tMeshCutSequence::const_iterator j;
    for(j = circuit.begin(); j != circuit.end(); ++j)
    {
        const cCircuitElement* e = (*j)->getStartElement();
        do
        {
            tSigned32 k;
            for(k = 0; k < e->facesCrossed(); k++)
            {
                tFace candidate = e->faceCrossed(k);
                if(candidate == f)
                    result.push_back(cElementInFaceEntry(e,k));
            }
            e = e->getNext();
        }
        while(e != (*j)->getStartElement());
    }
}

void
LookupElementsInFace(tFace f, const tMeshCircuits& circuits, std::vector<cElementInFaceEntry>& result)
{
    tMeshCircuits::const_iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        LookupElementsInFace(f, **i, result);
    }
}

void
AddElementToLookup(const cCircuitElement& element, std::vector<std::vector<cElementInFaceEntry> >& lookup)
{
    tSigned32 i;
    for(i = 0; i < element.facesCrossed(); i++)
    {
        tFace f = element.faceCrossed(i);
        lookup[f.index()].push_back(cElementInFaceEntry(&element, i));
    }
}
void
AddCutToLookup(const cMeshCut& cut, std::vector<std::vector<cElementInFaceEntry> >& lookup)
{
    const cCircuitElement* e = cut.getStartElement();
    assertD(e);
    do
    {
        AddElementToLookup(*e, lookup);
        e = e->getNext();
    }
    while(e != cut.getStartElement());
}


void
BuildElementsInFaceLookup(const tMeshCutSequence& circuit,
                      std::vector<std::vector<cElementInFaceEntry> >& result)
{
    tMeshCutSequence::const_iterator j;
    for(j = circuit.begin(); j != circuit.end(); ++j)
    {
        AddCutToLookup(**j, result);
    }
}
void
BuildElementsInFaceLookup(const tMeshCircuits& circuits,
                      std::vector<std::vector<cElementInFaceEntry> >& result)
{
    tMeshCircuits::const_iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        tMeshCutSequence::const_iterator j;
        for(j = (*i)->begin(); j != (*i)->end(); ++j)
        {
            AddCutToLookup(**j, result);
        }
    }
}

std::vector<std::vector<cElementInFaceEntry> >*
ConstructAndBuildElementsInFaceLookup(const tMeshCutSequence& sequence, tSigned32 facesInMesh)
{
    std::vector<std::vector<cElementInFaceEntry> >* result =
        new std::vector<std::vector<cElementInFaceEntry> >(facesInMesh);
    BuildElementsInFaceLookup(sequence, *result);
    return result;
}
std::vector<std::vector<cElementInFaceEntry> >*
ConstructAndBuildElementsInFaceLookup(const tMeshCircuits& circuits, tSigned32 facesInMesh)
{
    std::vector<std::vector<cElementInFaceEntry> >* result =
        new std::vector<std::vector<cElementInFaceEntry> >(facesInMesh);
    BuildElementsInFaceLookup(circuits, *result);
    return result;
}
std::vector<std::vector<cElementInFaceEntry> >*
ConstructElementsInFaceLookup(tSigned32 facesInMesh)
{
    std::vector<std::vector<cElementInFaceEntry> >* result =
        new std::vector<std::vector<cElementInFaceEntry> >(facesInMesh);
    return result;
}
void
ClearElementsInFaceLookup(std::vector<std::vector<cElementInFaceEntry> >& lookup)
{
    size_t i;
    for(i = 0; i < lookup.size(); i++)
    {
        lookup[i].clear();
    }
}
void
DestroyElementsInFaceLookup(std::vector<std::vector<cElementInFaceEntry> >* ptr)
{
    delete ptr;
}

void
cExternalSectionEntry::addSection(
                        std::vector<std::list<cExternalSectionEntry> >& lookup,
                        const cCircuitElement* exitingElement,
                        const cCircuitElement* enteringElement
                        )
{
    assertD(exitingElement->exits());
    tEdge startEE, endEE;
    startEE = exitingElement->exitEdge();
    endEE = enteringElement->entryEdge();

    cExternalSectionEntry toAdd;
    toAdd._exitingElement = exitingElement;
    toAdd._enteringElement = enteringElement;

    if(startEE == endEE)
    {
        if(EntryIsAfterExit(startEE, exitingElement->refLine(), enteringElement->refLine()))
        {
        // external section is in the middle of this element edge
            toAdd._type = ENTRY_AFTER_EXIT;
            lookup[startEE.index()].push_back(toAdd);
            return;
        }

        // external section is outer part of this element edge

        toAdd._type = EXIT_AFTER_ENTRY;
        lookup[startEE.index()].push_back(toAdd);
        return;
    }

    toAdd._type = EXIT_ONLY;
    lookup[startEE.index()].push_back(toAdd);
    toAdd._type = ENTRY_ONLY;
    lookup[endEE.index()].push_back(toAdd);

    startEE = startEE.twin().prev().twin();
    while(startEE != endEE)
    {
        toAdd._type = CONTAINED;
        lookup[startEE.index()].push_back(toAdd);
        startEE = startEE.twin().prev().twin();
    }
}

bool
cExternalSectionEntry::lineExitsInside(tEdge exitEE, const tLine& exitLine) const
{
    switch(_type)
    {
    default:
        invalid();
    case CONTAINED:
        return true;
    case ENTRY_AFTER_EXIT:
        return SideOfExit(exitEE, _exitingElement->refLine(), exitLine) == SIDE_RIGHT
                &&
                SideOfEntry(exitEE, _enteringElement->refLine(), exitLine) == SIDE_LEFT;
    case EXIT_AFTER_ENTRY:
        return
            SideOfExit(exitEE, _exitingElement->refLine(), exitLine) == SIDE_RIGHT
            ||
            SideOfEntry(exitEE, _enteringElement->refLine(), exitLine) == SIDE_LEFT;
    case EXIT_ONLY:
        return SideOfExit(exitEE, _exitingElement->refLine(), exitLine) == SIDE_RIGHT;
    case ENTRY_ONLY:
        return SideOfEntry(exitEE, _enteringElement->refLine(), exitLine) == SIDE_LEFT;
    }
}

tSigned32
cExternalSectionEntry::countExits(
        const std::vector<std::list<cExternalSectionEntry> >& lookup,
        tEdge exitEE, const tLine& exitLine)
{
    tSigned32 result = 0;

    const std::list<cExternalSectionEntry>& l = lookup[exitEE.index()];
    std::list<cExternalSectionEntry>::const_iterator i;
    for(i = l.begin(); i != l.end(); ++i)
    {
        if(i->lineExitsInside(exitEE, exitLine))
        {
            result++;
        }
    }

    return result;
}





void
BuildExternalSectionLookup(const tMeshCircuits& circuits,
                      std::vector<std::list<cExternalSectionEntry> >& result)
{
    tMeshCircuits::const_iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        const tMeshCutSequence& circuit = **i;
        if(CircuitIsInternal(circuit))
        {
            continue;
        }
        tMeshCutSequence::const_iterator j;
        for(j = circuit.begin(); j != circuit.end(); ++j)
        {
            tMeshCutSequence::const_iterator nextJ = j;
            ++nextJ;
            if(nextJ == circuit.end())
            {
                nextJ = circuit.begin();
            }
            const cCircuitElement* exitingElement = (*j)->getStartElement()->getPrev();
            const cCircuitElement* enteringElement = (*nextJ)->getStartElement();

            cExternalSectionEntry::addSection(result, exitingElement, enteringElement);
        }
    }
}


cCutRegistry::cCutRegistry(tSigned32 facesInMesh, tSigned32 edgesInMesh)
{
    _lookup.resize(facesInMesh);
    _orphanedCount = 0;
    _edgesInMesh = edgesInMesh;
}
cCutRegistry::~cCutRegistry()
{
    assertD(_orphanedCount == 0);
}

tSigned32
cCutRegistry::getFacesInMesh() const
{
    return static_cast<tSigned32>(_lookup.size());
}
tSigned32
cCutRegistry::getEdgesInMesh() const
{
    return _edgesInMesh;
}

void
cCutRegistry::addCuts(tMeshCutSequence& toAdd)
{
    tMeshCutSequence::iterator i;
    for(i = toAdd.begin(); i != toAdd.end(); ++i)
    {
        assertD((*i)->getRef() == 1);
        _owners.push_front(*i);

        cCircuitElement* e = (*i)->getStartElement();
        assertD(e);
        do
        {
            addElement(*e, _owners.begin());
            e = e->getNext();
        }
        while(e != (*i)->getStartElement());
    }
    toAdd.clear();
}
void
cCutRegistry::addCircuits(tMeshCircuits& toAdd)
{
    tMeshCircuits::iterator i;
    for(i = toAdd.begin(); i != toAdd.end(); ++i)
    {
        addCuts(**i);
    }
    toAdd.clear();
}

//void
//cCutRegistry::addCuts(std::vector<cCircuitElement*>& toAdd)
//{
//    assertD((SizeL(toAdd) & 1) == 0);
//    tSigned32 i;
//    for(i = 0; i != SizeL(toAdd) / 2; ++i)
//    {
//
//    }
//}

void
cCutRegistry::splitElement(const cCutRegistryEntry& toSplit, const tLine& splitLine)
{
    cCircuitElement* e = toSplit.element;
    e->split(toSplit.faceIndex, splitLine);
    cCircuitElement* after = e->getNext();
    tSigned32 faceInMesh = after->faceCrossed(0).index();
    _lookup[faceInMesh].push_back(cCutRegistryEntry(after, 0, toSplit.ownerNode));
    
  // update any entries after the new element leaves this face
    tSigned32 i;
    for(i = 1; i < after->facesCrossed(); i++)
    {
        tSigned32 faceInMesh = after->faceCrossed(i).index();
        tFaceIterator j = lookupElement(faceInMesh,e);
        j->element = after;
        j->faceIndex = i;
    }
}

void
cCutRegistry::explodeOwnerIfOwned(const cCutRegistryEntry& entry)
{
    if(entry.ownerNode == _owners.end())
    {
      // not owned
        return;
    }
    tCutRegistryOwnerIterator ownerNode = entry.ownerNode;
    cMeshCut& cut = **ownerNode;
    cCircuitElement* e = cut.getStartElement();
    assertD(e);
    do
    {
        orphanElement(*e);
        e = e->getNext();
    }
    while(e != cut.getStartElement());
    cut.takeCircuit();
    _owners.erase(ownerNode);
}

void
cCutRegistry::rebuildFromFragmentIfNotAccountedFor(const cCutRegistryEntry& entry, bool discard)
{
    if(entry.ownerNode != _owners.end())
    {
      // accounted for
        return;
    }

    tCutRegistryOwnerIterator ownerNode;
    if(discard)
    {
        _toBeDiscarded.push_front(new cMeshCut);
        ownerNode = _toBeDiscarded.begin();
    }
    else
    {
        _toBeOwned.push_front(new cMeshCut);
        ownerNode = _toBeOwned.begin();
    }

    cMeshCut& cut = **ownerNode;
    cut.initFromFragment(*entry.element);
    cCircuitElement* e = cut.getStartElement();
    assertD(e);
    do
    {
        unOrphanElement(*e, ownerNode);
        e = e->getNext();
    }
    while(e != cut.getStartElement());
}

void
cCutRegistry::commitRebuilds()
{
    tCutRegistryOwnerIterator i;
    for(i = _toBeDiscarded.begin(); i != _toBeDiscarded.end(); ++i)
    {
        removeEntriesForElementsInCut(**i);
    }
    _toBeDiscarded.clear();
    _owners.splice(_owners.end(), _toBeOwned);
}

bool
cCutRegistry::containsOrphanedEntries() const
{
    return _orphanedCount != 0;
}

bool
cCutRegistry::containsInlineCorner()
{
    size_t i;
    for(i = 0; i < _lookup.size(); i++)
    {
        std::list<cCutRegistryEntry>::const_iterator j;
        for(j = _lookup[i].begin(); j != _lookup[i].end(); ++j)
        {
            if(j->element->cornerIsInline())
            {
                return true;
            }
        }
    }
    return false;
}

void
cCutRegistry::joinInlineCorners(const std::list<cCutRegistryEntry*>& after)
{
    std::vector<cCutRegistryEntry> copy(after.size());
    std::list<cCutRegistryEntry*>::const_iterator i;
    size_t j = 0;
    for(i = after.begin(); i != after.end(); ++i)
    {
        copy[j++] = **i;
    }
    for(j = 0; j < copy.size(); j++)
    {
        cCircuitElement* e = copy[j].element;
        if(e->cornerIsInline())
        {
            tCutRegistryOwnerIterator ownerNode = copy[j].ownerNode;
            assertD(ownerNode != _owners.end());
            joinElement(*e, ownerNode);
        }
    }
}

void
cCutRegistry::stripAsCuts(tMeshCutSequence& appendTo)
{
    assertD(_orphanedCount == 0);
    tCutRegistryOwnerIterator i;
    for(i = _owners.begin(); i !=_owners.end(); ++i)
    {
        appendTo.push_back(*i);
    }
    size_t j;
    for(j = 0; j < _lookup.size(); j++)
    {
        _lookup[j].clear();
    }
}
void
cCutRegistry::stripAsCircuits(tMeshCircuits& appendTo)
{
    assertD(_orphanedCount == 0);
    while(!_owners.empty())
    {
        appendTo.push_back(new tMeshCutSequence);
        appendTo.back()->push_back(_owners.front());
        _owners.pop_front();
    }
    {
        size_t i;
        for(i = 0; i < _lookup.size(); i++)
        {
            _lookup[i].clear();
        }
    }
}


void
cCutRegistry::removeCutsConnectingToEdge(tEdge e)
{
    tEdge ee = e.twin();
    tFace f = e.twin().face();
    tFaceIterator i;
    for(i = refLookup()[f.index()].begin(); i != refLookup()[f.index()].end();)
    {
        cCircuitElement* e = i->element;
        tFaceIterator candidate = i;
        ++i;

        if(e->enters())
        {
            tEdge entry = e->entryEdge();
            if(entry == ee) 
            {
                removeOwningCut(*candidate);
                i = refLookup()[f.index()].begin();
            }
        }
    }    
}

void
cCutRegistry::removeCutsThatArentInternal()
{
    assertD(_ownersAffected.empty());
    tCutRegistryOwnerIterator i;
    for(i = _owners.begin(); i != _owners.end(); )
    {
        if((*i)->isInternalLoop())
        {
            ++i;
        }
        else
        {
            removeEntriesForElementsInCut(**i);
            i = _owners.erase(i);
        }
    }
}

void
cCutRegistry::markOwnerAsAffected(const cCutRegistryEntry& entry)
{
    tCutRegistryOwnerIterator i;
    for(i = _ownersAffected.begin(); i != _ownersAffected.end(); ++i)
    {
        if(i == entry.ownerNode)
        {
        // already in the affected list
            return;
        }
    }
    _ownersAffected.splice(_ownersAffected.end(), _owners, entry.ownerNode);
}

//tMeshCutSequence&
//cCutRegistry::refUnaffectedOwners()
//{
//    return _owners;
//}

tCutRegistryOwnerIterator
cCutRegistry::unaffectedOwnersBegin()
{
    return _owners.begin();
}
tCutRegistryOwnerIterator
cCutRegistry::unaffectedOwnersEnd()
{
    return _owners.end();
}

void
cCutRegistry::clearAffected()
{
    _owners.splice(_owners.end(), _ownersAffected);
}

bool
cCutRegistry::affectedEmpty() const
{
    return _ownersAffected.empty();
}

void
cCutRegistry::removeFromUnaffected(tCutRegistryOwnerIterator ownerNode)
{
    removeEntriesForElementsInCut(**ownerNode);
    _owners.erase(ownerNode);
}

void
cCutRegistry::validate() const
{
    size_t i;
    for(i = 0; i < _lookup.size(); i++)
    {
        std::list<cCutRegistryEntry>::const_iterator j;
        for(j = _lookup[i].begin(); j != _lookup[i].end(); ++j)
        {
            validateEntry(*j);
        }
    }
}

void
cCutRegistry::validateElements() const
{
    size_t i;
    for(i = 0; i < _lookup.size(); i++)
    {
        std::list<cCutRegistryEntry>::const_iterator j;
        for(j = _lookup[i].begin(); j != _lookup[i].end(); ++j)
        {
            assertD(j->element->isValid());
        }
    }
}

bool
cCutRegistry::allCutsAreInternal() const
{
    assertD(_ownersAffected.empty());
    tCutRegistryOwnerConstIterator i;
    for(i = _owners.begin(); i != _owners.end(); ++i)
    {
        if(!(*i)->isInternalLoop())
        {
            return false;
        }
    }
    return true;
}

void
cCutRegistry::validateEntry(const cCutRegistryEntry& entry) const
{
    tCutRegistryOwnerConstIterator i;
    for(i = _owners.begin(); i != _owners.end(); ++i)
    {
        if(i == entry.ownerNode)
        {
            break;
        }
    }
    assertD(i != _owners.end());    
}

void
cCutRegistry::removeOwningCut(cCutRegistryEntry& entry)
{
    tCutRegistryOwnerIterator ownerNode = entry.ownerNode;
    removeEntriesForElementsInCut(**entry.ownerNode);
    _owners.erase(ownerNode);
}

cCutRegistry::tFaceIterator
cCutRegistry::lookupElement(size_t faceInMesh, cCircuitElement* element)
{
    tFaceIterator i;
    for(i = _lookup[faceInMesh].begin(); i != _lookup[faceInMesh].end(); ++i)
    {
        if(i->element == element)
        {
            return i;
        }
    }
    invalid();
    return i;
}


void
cCutRegistry::addElement(cCircuitElement& element, tCutRegistryOwnerIterator ownerNode)
{
    tSigned32 i;
    for(i = 0; i < element.facesCrossed(); i++)
    {
        tFace f = element.faceCrossed(i);
        _lookup[f.index()].push_front(cCutRegistryEntry(&element, i, ownerNode));
    }
}
void
cCutRegistry::orphanElement(cCircuitElement& element)
{
    tSigned32 i;
    for(i = 0; i < element.facesCrossed(); i++)
    {
        tSigned32 faceInMesh = element.faceCrossed(i).index();
        tFaceIterator j = lookupElement(faceInMesh, &element);
        assertD(j->ownerNode != _owners.end());
        j->ownerNode = _owners.end();
        _orphanedCount++;
    }
}
void
cCutRegistry::unOrphanElement(cCircuitElement& element, tCutRegistryOwnerIterator ownerNode)
{
    tSigned32 i;
    for(i = 0; i < element.facesCrossed(); i++)
    {
        tSigned32 faceInMesh = element.faceCrossed(i).index();
        tFaceIterator j = lookupElement(faceInMesh, &element);
        assertD(j->ownerNode == _owners.end());
        j->ownerNode = ownerNode;
        assertD(_orphanedCount > 0);
        _orphanedCount--;
    }
}

void
cCutRegistry::joinElement(cCircuitElement& element, tCutRegistryOwnerIterator ownerNode)
{
    cMeshCut& cut = **ownerNode;
    if(&element == cut.getStartElement())
    {
        cut.changeStartElement();
    }
// TODO optimise
    cCircuitElement* prev = element.getPrev();
    removeElement(*prev);
    removeElement(element);
    prev->join();
    addElement(*prev, ownerNode);
}

void
cCutRegistry::removeElement(cCircuitElement& element)
{
    tSigned32 i;
    for(i = 0; i < element.facesCrossed(); i++)
    {
        tSigned32 faceInMesh = element.faceCrossed(i).index();
        tFaceIterator j = lookupElement(faceInMesh, &element);
        _lookup[faceInMesh].erase(j); 
    }
}

void
cCutRegistry::removeEntriesForElementsInCut(cMeshCut& cut)
{
    cCircuitElement* e = cut.getStartElement();
    assertD(e);
    do
    {
        removeElement(*e);
        e = e->getNext();
    }
    while(e != cut.getStartElement());
}


const cCircuitElement*
FindMatchingCorner(const std::vector<cElementInFaceEntry>& fLookup, const cCircuitElement& matchWith)
{
    size_t i;
    tPoint p = matchWith.cornerPoint();
    for(i = 0; i < fLookup.size(); i++)
    {
        if(fLookup[i].faceIndex != 0)
        {
            continue;
        }
        const cCircuitElement* candidate = fLookup[i].element;
        if(!candidate->cornerIsConvex())
        {
            continue;
        }
        if(candidate->cornerPoint() != p)
        {
            continue;
        }
        if(candidate->refLine().side(p) != SIDE_CENTRE)
        {
            continue;
        }
        if(candidate->refLine().direction(matchWith.refLine()) != DIRECTION_FORWARD)
        {
            continue;
        }
        return candidate;
    }
    invalid();
    return 0;
}


