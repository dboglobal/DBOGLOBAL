//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

/*
#include "platform_common/Header.h"
#include "ProcessCircuitIntersections.h"
#include "ElementInFaceLookup.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/tMesh.h"

static bool
ElementLineCrossesLine(
                    cCutRegistry& registry,
                    const cCircuitElement& element,
                    bool& elementAtStart,
                    bool& elementAtEnd,
                    tSigned32& elementFaceIndex,
                    cCutRegistryEntry*& crossedEntry,
                    bool& crossedElementAtStart,
                    bool& crossedElementAtEnd
                    )
{
    tSigned32 faces = element.facesCrossed();
    for(elementFaceIndex = 0; elementFaceIndex < faces; elementFaceIndex++)
    {
        tFace f = element.faceCrossed(elementFaceIndex);
        cCutRegistry::tFaceIterator k;
        for(k = registry.refLookup()[f->index].begin(); k != registry.refLookup()[f->index].end(); ++k)
        {
            crossedEntry = &*k;

            if(crossedEntry->element->elementEntersThroughThis(
                        crossedEntry->faceIndex,
                        element, elementFaceIndex,
                        crossedElementAtStart, crossedElementAtEnd, elementAtStart, true))
            {
                cMeshCut& cut = **crossedEntry->ownerNode;
                if((!crossedElementAtStart && !crossedElementAtEnd) || !elementAtStart)
                {
                    elementAtEnd = false;
                    return true;
                }
            }
            // NOTE - no else because it is possible to exit and enter the same element
            // but optimise - this should only happen in the case of crossedElementAtStart being set
            if(crossedEntry->element->elementExitsThroughThis(
                        crossedEntry->faceIndex,
                        element, elementFaceIndex,
                        crossedElementAtStart, crossedElementAtEnd, elementAtEnd, true))
            {
                cMeshCut& cut = **crossedEntry->ownerNode;
                if((!crossedElementAtStart && !crossedElementAtEnd) || !elementAtEnd)
                {
                    elementAtStart = false;
                    return true;
                }
            }
        }
    }
    return false;
}

static bool
SplitAtCrossings(cCutRegistry& registry, cMeshCut& cut)
{
    bool splitsMade = false;
    cCircuitElement* test = cut.getStartElement();

    while(1)
    {
        bool testAtStart, testAtEnd;
        tSigned32 testFaceIndex;
        cCutRegistryEntry* crossedEntry;
        bool crossedAtStart, crossedAtEnd;

        if(ElementLineCrossesLine(
                registry,
                *test,
                testAtStart, testAtEnd, testFaceIndex,
                crossedEntry, crossedAtStart, crossedAtEnd
                ))
        {
            cCircuitElement* crossedElement = crossedEntry->element;
            const tLine* crossedSplit = 0;
		  // if either element needs to be split, obtain a line for the split
            if(!crossedAtStart && !crossedAtEnd)
			{
				if(testAtStart)
				{
					crossedSplit = &test->getIntersectAtCorner(crossedElement->refLine());
				}
				else
				if(testAtEnd)
				{
					crossedSplit = &test->getNext()->getIntersectAtCorner(crossedElement->refLine());
				}
                else
				{
					crossedSplit = &test->refLine();
				}
			}
			const tLine* testSplit = 0;
            if(!testAtStart && !testAtEnd)
			{
				if(crossedAtStart)
				{
					testSplit = &crossedElement->getIntersectAtCorner(test->refLine());
				}
				else
				if(crossedAtEnd)
				{
					testSplit = &crossedElement->getNext()->getIntersectAtCorner(test->refLine());
				}
				else
				{
					testSplit = &crossedElement->refLine();
				}
			}
            if(crossedSplit)
            {
                splitsMade = true;
                registry.splitElement(*crossedEntry, *crossedSplit);
            }
            if(testSplit)
            {
                splitsMade = true;
                test->split(testFaceIndex, *testSplit);
            }
        }
        else
        {
            test = test->getNext();
            if(test == cut.getStartElement())
                break;
        }
    }

    return splitsMade;
}

class cCornerCrossings
{
    cCutRegistryEntry* _toAddEntry;

    // both zero = immediate reentry
    // one set = simple circuit crossing
    // both set = exit and reentry at a single point
    cCutRegistryEntry* _exited;
    cCutRegistryEntry* _entered;

public:

    cCornerCrossings(

};




// check whether element corner crosses any of the corners on the base circuits
// (to be used after splits have been added for any non corner crossings)
// usually there will be only one entry or exit
// but exit followed by reentry at a single point is possible
static void
GetCornerCrossings(
        std::list<cCutRegistryEntry>& entriesInFace,
        std::list<cCutRegistryEntry>::iterator firstBeforeAdding,
        std::list<cCutRegistryEntry>::iterator testI,
        cCutRegistryEntry*& exitedElement,
        cCutRegistryEntry*& enteredElement,
        bool& immediateReentry
        )
{
    exitedElement = 0;
    enteredElement = 0;
    immediateReentry = false;

    if(testI->element->enters())
    {
        return;
    }
    if(testI->faceIndex != 0)
    {
        return;
    }

    cCircuitElement* testElement = testI->element;

    tIntersection testIntersection;
    testElement->getIntersectionForCornerAtStart(testIntersection);

    std::list<cCutRegistryEntry>::iterator baseI;
    for(baseI = firstBeforeAdding; baseI != entriesInFace.end(); ++baseI)
    {
        cCircuitElement* baseElement = baseI->element;

        if(baseElement == testElement)
        {
            continue;
        }
        if(baseElement->enters())
        {
            continue;
        }
        if(baseI->faceIndex != 0)
        {
            continue;
        }

        tIntersection baseIntersection;
        baseElement->getIntersectionForCornerAtStart(baseIntersection);
        
        if(baseIntersection != testIntersection)
        {
            continue;
        }

      // this is a crossing if either approach line or depart line go inside the base circuit

        const tLine& before = baseElement->getPrev()->refLine();
        const tLine& after = baseElement->refLine();

//        bool comesFromInside = InsideCorner(before, after, testElement->getPrev()->refLine().reversed());
//        bool goesInside = InsideCorner(before, after, testElement->refLine());
        bool comesFromInside, goesInside;
        ComputeCornerRedirects(
                before, after,
                testElement->getPrev()->refLine(), testElement->refLine(),
                comesFromInside, goesInside);

        if(comesFromInside && goesInside)
        {
            assertD(!immediateReentry);
            assertD(!enteredElement);
            assertD(!exitedElement);
            immediateReentry = true;
            enteredElement = &*baseI;
        }
        else
        if(comesFromInside)
        {
            assertD(!immediateReentry);
            assertD(!exitedElement);
            exitedElement = &*baseI;
        }
        else
        if(goesInside)
        {
            assertD(!immediateReentry);
            assertD(!enteredElement);
            enteredElement = &*baseI;
        }
    }
}

static void
GetRedirectsInFace(
        std::list<cCutRegistryEntry>& entriesInFace,
        std::list<cCutRegistryEntry>::iterator firstBeforeAdding,
        std::list<cCutRegistryEntry*>& crossingElements,
        std::list<cCutRegistryEntry*>& exitedElements,
        std::list<cCutRegistryEntry*>& enteredElements,
        std::list<cCutRegistryEntry*>& immediateReentries,
        std::list<cCutRegistryEntry*>& reenteredElements
        )
{
    std::list<cCutRegistryEntry>::iterator i;
    for(i = entriesInFace.begin(); i != firstBeforeAdding; ++i)
    {
        cCutRegistryEntry* exitedElement;
        cCutRegistryEntry* enteredElement;
        bool immediateReentry;

        GetCornerCrossings(entriesInFace, firstBeforeAdding, i, exitedElement, enteredElement, immediateReentry);

        if(immediateReentry)
        {
            immediateReentries.push_back(&*i);
            reenteredElements.push_back(enteredElement);
        }
        else
        {
            if(exitedElement || enteredElement)
            {
                crossingElements.push_back(&*i);
                exitedElements.push_back(exitedElement);
                enteredElements.push_back(enteredElement);
            }
        }
    }
}



static void
DoRedirects(cCutRegistry& registry, cMeshCut& toAdd)
{
    cCircuitElement* e = toAdd.getStartElement();
    do
    {
        if(e->enters())
        {
            continue;
        }

    }
    while(toAdd.toNextElement(e));





    std::vector<cCutRegistry::tFaceIterator> firstBeforeAdding(registry.getFacesInMesh());

    {
        size_t i;
        for(i = 0; i < faceContainsCrossings.size(); i++)
        {
            if(faceContainsCrossings[i])
            {
                firstBeforeAdding[i] = registry.refLookup()[i].begin();
            }
        }
    }

    registry.addCuts(toAdd);

    std::list<cCutRegistryEntry*> crossingElements;
    std::list<cCutRegistryEntry*> exitedElements;
    std::list<cCutRegistryEntry*> enteredElements;

    std::list<cCutRegistryEntry*> immediateReentries;
    std::list<cCutRegistryEntry*> reenteredElements;

    // doesn't affect the lookup
    {
        size_t i;
        for(i = 0; i < faceContainsCrossings.size(); i++)
        {
            if(faceContainsCrossings[i])
            {
                GetRedirectsInFace(
                    registry.refLookup()[i], 
                    firstBeforeAdding[i],
                    crossingElements, exitedElements, enteredElements,
                    immediateReentries, reenteredElements);
            }
        }
    }

    ExplodeAnyOwningCuts(registry, crossingElements);
    ExplodeAnyOwningCuts(registry, exitedElements);
    ExplodeAnyOwningCuts(registry, enteredElements);

    ExplodeAnyOwningCuts(registry, immediateReentries);
    ExplodeAnyOwningCuts(registry, reenteredElements);

    {        
        std::list<cCutRegistryEntry*> afterIn;
        std::list<cCutRegistryEntry*> afterOut;

        Redirect(crossingElements, exitedElements, enteredElements, afterIn, afterOut);

        afterIn.splice(afterIn.end(), immediateReentries);
        afterOut.splice(afterOut.end(), reenteredElements);

        RebuildFromFragments(registry, afterIn, true);
        RebuildFromFragments(registry, afterOut, false);
        registry.joinInlineCorners(afterOut);
        registry.commitRebuilds();
    }

    assertR(!registry.containsOrphanedEntries());
}



bool
ProcessCircuitIntersections(cCutRegistry& registry, tMeshCutSequence& toAdd)
{
    bool splitsMade = false;
    tMeshCutSequence::iterator i;
    for(i = toAdd.begin(); i != toAdd.end(); ++i)
    {    
        bool splitsMadeForCut = SplitAtCrossings(registry, **i);
        splitsMade = (splitsMade || splitsMadeForCut);
    }



    return false;
}

*/
