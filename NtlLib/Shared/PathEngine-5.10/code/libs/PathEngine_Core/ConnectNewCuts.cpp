//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "ConnectNewCuts.h"
#include "ElementInFaceLookup.h"
#include "BoundaryCrossings2D.h"
#include "interface/PointIsInsideCircuits.h"
#include "interface/PointCollision.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "interface/LocalisedConvexShape.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "libs/Geometry/interface/InsideCorner.h"
#include "common/Containers/FlagVector.h"
#include "common/STL/list.h"
#include "common/STL/algorithm.h"

enum eIntersectionType
{
    IT_NO_INTERSECTION,
    IT_EXITS,
    IT_ENTERS,
    IT_IMMEDIATE_REENTRY,
};

static eIntersectionType
GetIntersectionType(
            const tLine& beforeBase, const tLine& afterBase,
            const tLine& beforeTest, const tLine& afterTest
            )
{
    eSide sideBefore;
    if(InsideCorner_Vector(beforeBase, afterBase, -beforeTest.axis()))
    {
        sideBefore = SIDE_RIGHT;
    }
    else if(beforeBase.direction(beforeTest) == DIRECTION_FORWARD)
    {
        sideBefore = SIDE_CENTRE;
    }
    else
    {
        sideBefore = SIDE_LEFT;
    }

    eSide sideAfter;
    if(InsideCorner(beforeBase, afterBase, afterTest))
    {
        sideAfter = SIDE_RIGHT;
    }
    else if(afterBase.direction(afterTest) == DIRECTION_FORWARD)
    {
        sideAfter = SIDE_CENTRE;
    }
    else
    {
        sideAfter = SIDE_LEFT;
    }
    
    if(sideBefore == sideAfter)
    {
        if(sideBefore == SIDE_RIGHT)
        {
            return IT_IMMEDIATE_REENTRY;
        }
        return IT_NO_INTERSECTION;
    }
    if(sideAfter == SIDE_RIGHT)
    {
        return IT_ENTERS;
    }
    if(sideBefore == SIDE_RIGHT)
    {
        return IT_EXITS;
    }
    return IT_NO_INTERSECTION;
}

// check whether element corner crosses any of the corners on the base circuits
// (to be used after splits have been added for any non corner crossings)
// usually there will be only one entry or exit
// but exit followed by reentry at a single point is possible
static void
GetCornerCrossings(tMeshCutSequence& baseCuts,
                        const cCircuitElement& testElement,
                        cCircuitElement*& exitedElement,
                        cCircuitElement*& enteredElement
                        )
{
    exitedElement = 0;
    enteredElement = 0;

    if(testElement.enters())
    {
        return;
    }

    tIntersection testIntersection;
    testElement.getIntersectionForCornerAtStart(testIntersection);

    tFace f = testElement.faceCrossed(0);


    tMeshCutSequence::iterator i;
    for(i = baseCuts.begin(); i != baseCuts.end(); ++i)
    {
        cCircuitElement* e = (*i)->getStartElement();
        do
        {
            cCircuitElement* baseElement = e;
            e = e->getNext();

            if(baseElement == &testElement)
            {
                continue;
            }
            if(baseElement->faceCrossed(0) != f)
            {
                continue;
            }
            if(baseElement->enters())
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

            eIntersectionType intersectionType = GetIntersectionType(
                    before, after,
                    testElement.getPrev()->refLine(), testElement.refLine()
                    );

            switch(intersectionType)
            {
            case IT_ENTERS:
                assertD(!enteredElement);
                enteredElement = baseElement;
                break;
            case IT_EXITS:
                assertD(!exitedElement);
                exitedElement = baseElement;
                break;
            case IT_IMMEDIATE_REENTRY:
                assertD(!enteredElement);
                enteredElement = baseElement;
                assertD(!exitedElement);
                exitedElement = baseElement;
                break;
            }
        }
        while(e != (*i)->getStartElement());
    }
}

// this version is only called when checking for self crossing
static void
GetRedirectsAtCrossings_ExitsOnly(
        tMeshCutSequence& baseCuts,
        cMeshCut& test,
        std::list<cCircuitElement*>& crossingElements,
        std::list<cCircuitElement*>& exitedElements,
        std::list<cCircuitElement*>& enteredElements
        )
{
    cCircuitElement* e = test.getStartElement();

    do
    {
        cCircuitElement* exitedElement;
        cCircuitElement* enteredElement;

        GetCornerCrossings(baseCuts, *e, exitedElement, enteredElement);

        assertD(!(exitedElement && enteredElement));
        enteredElement = 0;
        if(exitedElement || enteredElement)
        {
            crossingElements.push_back(e);
            exitedElements.push_back(exitedElement);
            enteredElements.push_back(enteredElement);
        }

        e = e->getNext();
    }
    while(e != test.getStartElement());
}

static bool
ElementCrossesCut_NotBothCorners(
                    cMeshCut& cut,
                    const cCircuitElement& element,
                    bool& elementAtStart,
                    bool& elementAtEnd,
                    tSigned32& elementFaceIndex,
                    cCircuitElement*& crossedElement,
                    bool& crossedElementAtStart,
                    tSigned32& crossedElementFaceIndex,
                    bool& crossingFound
                    )
{
    bool crossedElementAtEnd;
    tSigned32 faces = element.facesCrossed();
    for(elementFaceIndex = 0; elementFaceIndex < faces; elementFaceIndex++)
    {
        tFace face = element.faceCrossed(elementFaceIndex);
        crossedElement = cut.getStartElement();
        do
        {
            crossedElementFaceIndex = crossedElement->indexForFace(face);
            if(crossedElementFaceIndex != -1)
            {
                if(crossedElement->elementEntersThroughThis(
                            crossedElementFaceIndex,
                            element, elementFaceIndex,
                            crossedElementAtStart, crossedElementAtEnd, elementAtStart
                            ))
                {
                    crossingFound = true;
                    if((!crossedElementAtStart && !crossedElementAtEnd) || !elementAtStart)
                    {
                        if(crossedElementAtEnd)
                        {
                            crossedElement = crossedElement->getNext();
                            crossedElementAtStart = true;
                        }
                        elementAtEnd = false;
                        return true;
                    }
                }
                // NOTE - no else because it is possible to exit and enter the same element
                // but optimise - this can only happen in the case of atStartOfTest being set
                if(crossedElement->elementExitsThroughThis(
                            crossedElementFaceIndex,
                            element, elementFaceIndex,
                            crossedElementAtStart, crossedElementAtEnd, elementAtEnd
                            ))
                {
                    crossingFound = true;
                    if((!crossedElementAtStart && !crossedElementAtEnd) || !elementAtEnd)
                    {
                        if(crossedElementAtEnd)
                        {
                            crossedElement = crossedElement->getNext();
                            crossedElementAtStart = true;
                        }
                        elementAtStart = false;
                        return true;
                    }
                }
            }
            crossedElement = crossedElement->getNext();
        }
        while(crossedElement != cut.getStartElement());
    }
    return false;
}

// used by SortOutSelfCrossings()
static bool
SplitAtCrossings(cMeshCut& base, cMeshCut& testCut)
{
    bool crossingFound = false;
    cCircuitElement* test = testCut.getStartElement();

    while(1)
    {
        bool testAtStart, testAtEnd;
        tSigned32 testFaceIndex;
        cCircuitElement* crossed;
        bool crossedAtStart;
        tSigned32 crossedFaceIndex;

        if(ElementCrossesCut_NotBothCorners(
                base, *test,
                testAtStart, testAtEnd, testFaceIndex,
                crossed, crossedAtStart, crossedFaceIndex,
                crossingFound
                ))
        {
            const tLine* crossedSplit = 0;
		  // if either element needs to be split, obtain a line for the split
            if(!crossedAtStart)
			{
				if(testAtStart)
				{
					crossedSplit = &test->getIntersectAtCorner(crossed->refLine());
				}
				else
				if(testAtEnd)
				{
					crossedSplit = &test->getNext()->getIntersectAtCorner(crossed->refLine());
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
					testSplit = &crossed->getIntersectAtCorner(test->refLine());
				}
				else
				{
					testSplit = &crossed->refLine();
				}
			}
            if(crossedSplit)
            {
                crossed->split(crossedFaceIndex, *crossedSplit);
            }
            if(testSplit)
            {
                test->split(testFaceIndex, *testSplit);
            }
        }
        else
        {
            test = test->getNext();
            if(test == testCut.getStartElement())
                break;
        }
    }

    return crossingFound;
}

static void
RemoveElementsFromList(const cCircuitElement* start_element,
                       std::list<cCircuitElement*>& element_list)
{
    const cCircuitElement* e = start_element;
    do
    {
        std::list<cCircuitElement*>::iterator i;
        i = std::find(element_list.begin(), element_list.end(), e);
        if(i != element_list.end())
        {
            element_list.erase(i);
        }
        e = e->getNext();
    }
    while(e != start_element);
}

static void
Redirect(
    std::list<cCircuitElement*>& crossingElements,
    std::list<cCircuitElement*>& exitedElements,
    std::list<cCircuitElement*>& enteredElements,
    std::list<cCircuitElement*>& after_in,
    std::list<cCircuitElement*>& after_out)
{
// implement redirects, keeping track of inside and outside circuit parts
    std::list<cCircuitElement*>::iterator i,j,k;
    for(i = crossingElements.begin(),
         j = exitedElements.begin(),
         k = enteredElements.begin();
        i != crossingElements.end();
        ++i, ++j, ++k
        )
    {
        if(*j && *k)
        {
          // immediate reentry
            if(*j == *k)
            {
              // test circuit comes from inside and touches a single boundary corner
                after_out.push_back(*j);
                after_in.push_back(*i);
            }
            else
            {
              // test circuit bridges two pieces of boundary at this corner
			    (*j)->getPrev()->redirect(*(*k)->getPrev());			
                after_out.push_back(*j);
                after_in.push_back(*i);
                after_in.push_back(*k);
            }
        }
        else
        {
            if(*j)
            {
            // exit
                after_out.push_back(*i);
                after_in.push_back(*j);
            }
            else
            {
            // entry
                assertD(*k);
                after_in.push_back(*i);
                after_out.push_back(*k);
            }
			after_out.back()->getPrev()->redirect(*after_in.back()->getPrev());
        }    
    }
}

static void
DeleteFragments(std::list<cCircuitElement*>& after_in,
                std::list<cCircuitElement*>& after_out
                )
{
    while(!after_in.empty())
    {
        cMeshCut cut;
        cut.initFromFragment(*after_in.front());

        RemoveElementsFromList(cut.getStartElement(), after_in);

        // this must be done because the original circuits may contain borders inside borders,
        // so a circuit element tagged as in may connect to another tagged as out
        RemoveElementsFromList(cut.getStartElement(), after_out);
    }
}

static void
RebuildFromFragments(std::list<cCircuitElement*>& after_out,
                    tMeshCutSequence& result
                     )
{
    while(!after_out.empty())
    {
        result.push_back(new cMeshCut());
        result.back()->initFromFragment(*after_out.front());
        RemoveElementsFromList(result.back()->getStartElement(), after_out);
        result.back()->joinInlineCorners();
//        result.back()->removeZeroLengthSections();
        result.back()->checkNoZeroLengthSections();
    }
}


static void
RedirectAndRebuildFromFragments(
    std::list<cCircuitElement*>& crossingElements,
    std::list<cCircuitElement*>& exitedElements,
    std::list<cCircuitElement*>& enteredElements,
    tMeshCutSequence& result)
{
    std::list<cCircuitElement*> afterIn;
    std::list<cCircuitElement*> afterOut;
    Redirect(crossingElements, exitedElements, enteredElements, afterIn, afterOut);
    DeleteFragments(afterIn, afterOut);
    RebuildFromFragments(afterOut, result);
}

void
SortOutSelfCrossings(tMeshCutSequence& cuts)
{
    tMeshCutSequence unaffected;
    cBitVector crossing_found_vector(SizeL(cuts), false);

    tMeshCutSequence::iterator i;
    tSigned32 l = 0;
    for(i = cuts.begin(); i != cuts.end(); ++i, l++)
    {
        tMeshCutSequence::iterator j;
        tSigned32 k = l;
        for(j = cuts.begin(); j != cuts.end(); ++j, k++)
        {
            // note that SplitAtCrossings must handle the case where base and test are the same circuit
            bool crossing_found;
            crossing_found = SplitAtCrossings(**i, **j);
            if(crossing_found)
            {
                crossing_found_vector.setBit(l, true);
                crossing_found_vector.setBit(k, true);
            }
        }
    }

    l = 0;
    for(i = cuts.begin(); i != cuts.end(); l++)
    {
        if(!crossing_found_vector[l])
        {
            unaffected.push_back(*i);
            i = cuts._list.erase(i);
        }
        else
        {
            ++i;
        }
    }

    std::list<cCircuitElement*> crossingElements;
    std::list<cCircuitElement*> exitedElements;
    std::list<cCircuitElement*> enteredElements;

    {
        tMeshCutSequence::iterator j;
        for(j = cuts.begin(); j != cuts.end(); ++j)
        {
            GetRedirectsAtCrossings_ExitsOnly(
                cuts, **j,
                crossingElements, exitedElements, enteredElements
                );
        }
    }

// all elements will be accounted for as fragments

    {
        tMeshCutSequence::iterator j;
        for(j = cuts.begin(); j != cuts.end(); ++j)
        {
            (*j)->takeCircuit();
        }
        cuts.clear();
    }

    RedirectAndRebuildFromFragments(crossingElements, exitedElements, enteredElements, cuts);

    assertF(!CutSequenceContainsInlineCorner(cuts));
    cuts._list.splice(cuts._list.end(), unaffected._list);
}


static void
Redirect(
    std::list<cCutRegistryEntry*>& crossingElements,
    std::list<cCutRegistryEntry*>& exitedElements,
    std::list<cCutRegistryEntry*>& enteredElements,
    std::list<cCutRegistryEntry*>& afterIn,
    std::list<cCutRegistryEntry*>& afterOut)
{
// implement redirects, keeping track of inside and outside circuit parts
    std::list<cCutRegistryEntry*>::iterator i, j, k;
    for(i = crossingElements.begin(),
         j = exitedElements.begin(),
         k = enteredElements.begin();
        i != crossingElements.end();
        ++i, ++j, ++k
        )
    {
        if(*j && *k)
        {
          // immediate reentry
            if(*j == *k)
            {
              // test circuit comes from inside and touches a single boundary corner
                afterOut.push_back(*j);
                afterIn.push_back(*i);
            }
            else
            {
              // test circuit bridges two pieces of boundary at this corner
			    (*j)->element->getPrev()->redirect(*(*k)->element->getPrev());    			
                afterOut.push_back(*k);
                afterIn.push_back(*i);
                afterIn.push_back(*j);
            }
        }
        else
        {
            if(*j)
            {
            // exit
                afterOut.push_back(*i);
                afterIn.push_back(*j);
            }
            else
            {
            // entry
                assertD(*k);
                afterIn.push_back(*i);
                afterOut.push_back(*k);
            }
			afterOut.back()->element->getPrev()->redirect(*afterIn.back()->element->getPrev());
        }    
    }
}

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
        cCutRegistryEntry*& enteredElement
        )
{
    exitedElement = 0;
    enteredElement = 0;

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

        eIntersectionType intersectionType = GetIntersectionType(
                before, after,
                testElement->getPrev()->refLine(), testElement->refLine()
                );

        switch(intersectionType)
        {
        case IT_ENTERS:
            assertD(!enteredElement);
            enteredElement = &*baseI;
            break;
        case IT_EXITS:
            assertD(!exitedElement);
            exitedElement = &*baseI;
            break;
        case IT_IMMEDIATE_REENTRY:
            assertD(!enteredElement);
            enteredElement = &*baseI;
            assertD(!exitedElement);
            exitedElement = &*baseI;
            break;
        }
    }
}

static void
GetRedirectsInFace(
        std::list<cCutRegistryEntry>& entriesInFace,
        std::list<cCutRegistryEntry>::iterator firstBeforeAdding,
        std::list<cCutRegistryEntry*>& crossingElements,
        std::list<cCutRegistryEntry*>& exitedElements,
        std::list<cCutRegistryEntry*>& enteredElements
        )
{
    std::list<cCutRegistryEntry>::iterator i;
    for(i = entriesInFace.begin(); i != firstBeforeAdding; ++i)
    {
        cCutRegistryEntry* exitedElement;
        cCutRegistryEntry* enteredElement;
        GetCornerCrossings(entriesInFace, firstBeforeAdding, i, exitedElement, enteredElement);
        if(exitedElement || enteredElement)
        {
            crossingElements.push_back(&*i);
            exitedElements.push_back(exitedElement);
            enteredElements.push_back(enteredElement);
        }
    }
}


static bool
ElementCrossesCuts_NotBothCorners(
                    cCutRegistry& registry,
                    const cCircuitElement& element,
                    bool& elementAtStart,
                    bool& elementAtEnd,
                    tSigned32& elementFaceIndex,
                    cCutRegistryEntry*& crossedEntry,
                    bool& crossedElementAtStart,
                    bool& crossedElementAtEnd,
                    bool& crossingFound,
                    std::vector<char>& faceContainsCrossings,
                    std::list<cMeshCut*>& affectedCuts
                    )
{
    tSigned32 faces = element.facesCrossed();
    for(elementFaceIndex = 0; elementFaceIndex < faces; elementFaceIndex++)
    {
        tFace f = element.faceCrossed(elementFaceIndex);
        cCutRegistry::tFaceIterator k;
        for(k = registry.refLookup()[f.index()].begin(); k != registry.refLookup()[f.index()].end(); ++k)
        {
            crossedEntry = &*k;

            if(crossedEntry->element->elementEntersThroughThis(
                        crossedEntry->faceIndex,
                        element, elementFaceIndex,
                        crossedElementAtStart, crossedElementAtEnd, elementAtStart))
            {
                crossingFound = true;
                faceContainsCrossings[f.index()] = true;
                cMeshCut& cut = **crossedEntry->ownerNode;
                if(!cut.wasAffected())
                {
                    cut.setAffected();
                    affectedCuts.push_back(&cut);
                }
                if((!crossedElementAtStart && !crossedElementAtEnd) || !elementAtStart)
                {
                    elementAtEnd = false;
                    return true;
                }
            }
            // NOTE - no else because it is possible to exit and enter the same element
            // but optimise - this can only happen in the case of atStartOfTest being set
            if(crossedEntry->element->elementExitsThroughThis(
                        crossedEntry->faceIndex,
                        element, elementFaceIndex,
                        crossedElementAtStart, crossedElementAtEnd, elementAtEnd))
            {
                crossingFound = true;
                faceContainsCrossings[f.index()] = true;
                cMeshCut& cut = **crossedEntry->ownerNode;
                if(!cut.wasAffected())
                {
                    cut.setAffected();
                    affectedCuts.push_back(&cut);
                }
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
SplitAtCrossings(cCutRegistry& registry,
                 cMeshCut& testCut,
                 std::vector<char>& faceContainsCrossings,
                 std::list<cMeshCut*>& affectedCuts
                 )
{
    bool crossingFound = false;
    cCircuitElement* test = testCut.getStartElement();

    while(1)
    {
        bool testAtStart, testAtEnd;
        tSigned32 testFaceIndex;
        cCutRegistryEntry* crossedEntry;
        bool crossedAtStart, crossedAtEnd;

        if(ElementCrossesCuts_NotBothCorners(
                registry,
                *test,
                testAtStart, testAtEnd, testFaceIndex,
                crossedEntry, crossedAtStart, crossedAtEnd,
                crossingFound,
                faceContainsCrossings,
                affectedCuts
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
                registry.splitElement(*crossedEntry, *crossedSplit);
            }
            if(testSplit)
            {
                test->split(testFaceIndex, *testSplit);
            }
        }
        else
        {
            test = test->getNext();
            if(test == testCut.getStartElement())
                break;
        }
    }

    return crossingFound;
}

static void
RebuildFromFragments(cCutRegistry& registry,
                     const std::list<cCutRegistryEntry*>& fragmentHandles,
                     bool discard)
{
    std::list<cCutRegistryEntry*>::const_iterator i;
    for(i = fragmentHandles.begin(); i != fragmentHandles.end(); ++i)
    {
        registry.rebuildFromFragmentIfNotAccountedFor(**i, discard);
    }
}

static void
ExplodeAnyOwningCuts(cCutRegistry& registry, const std::list<cCutRegistryEntry*>& entries)
{
    std::list<cCutRegistryEntry*>::const_iterator i;
    for(i = entries.begin(); i != entries.end(); ++i)
    {
        if(*i)
        {
            registry.explodeOwnerIfOwned(**i);
        }
    }
}

static void
DoRedirects(cCutRegistry& registry,
            tMeshCutSequence& toAdd,
            const std::vector<char>& faceContainsCrossings)
{
    std::vector<cCutRegistry::tFaceIterator>
        firstBeforeAdding(registry.getFacesInMesh());

  // TODO optimise - should have a list of faces containing crossings

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
                    crossingElements, exitedElements, enteredElements
                    );
            }
        }
    }

    ExplodeAnyOwningCuts(registry, crossingElements);
    ExplodeAnyOwningCuts(registry, exitedElements);
    ExplodeAnyOwningCuts(registry, enteredElements);

    {        
        std::list<cCutRegistryEntry*> afterIn;
        std::list<cCutRegistryEntry*> afterOut;

        Redirect(crossingElements, exitedElements, enteredElements, afterIn, afterOut);

        RebuildFromFragments(registry, afterIn, true);
        RebuildFromFragments(registry, afterOut, false);
        registry.joinInlineCorners(afterOut);
        registry.commitRebuilds();
    }

    assertR(!registry.containsOrphanedEntries());
}


void
CheckNoSelfIntersections(cCutRegistry& registry)
{
    tSigned32 i;
    for(i = 0; i < registry.getFacesInMesh(); i++)
    {
        std::list<cCutRegistryEntry>& entriesInFace = registry.refLookup()[i];
        std::list<cCutRegistryEntry>::iterator i;
        for(i = entriesInFace.begin(); i != entriesInFace.end(); ++i)
        {
            cCutRegistryEntry* exitedElement;
            cCutRegistryEntry* enteredElement;
            std::list<cCutRegistryEntry>::iterator j = i;
            ++j;
            GetCornerCrossings(entriesInFace, j, i, exitedElement, enteredElement);
            assertD(exitedElement == 0);
            assertD(enteredElement == 0);
        }
    }
}
void
CheckNoSelfIntersections(tSigned32 facesInMesh, tSigned32 edgesInMesh, const tMeshCutSequence& cuts)
{
    cCutRegistry registry(facesInMesh, edgesInMesh);
    cHandle<tMeshCutSequence> copyOfCuts = CopyCutSequence(cuts);
    registry.addCuts(*copyOfCuts);
    CheckNoSelfIntersections(registry);
}

void ConnectExternalEdgeExpansion(cCutRegistry& registry, tMeshCutSequence& toAdd)
{
#ifdef ASSERTIONS_ON
//    CheckNoSelfIntersections(registry);
//    CheckNoSelfIntersections(registry.getFacesInMesh(), registry.getEdgesInMesh(), toAdd);
#endif

    tMeshCutSequence unaffectedNew;
    std::vector<char> faceContainsCrossings(registry.getFacesInMesh(), false);

    // TODO get rid of this in the case of external edge expansion
    std::list<cMeshCut*> affectedCuts;

    tMeshCutSequence::iterator i;
    for(i = toAdd.begin(); i != toAdd.end(); )
    {
        // SplitAtCrossings() will update the registry for any splits
        bool crossingFound = SplitAtCrossings(registry, **i, faceContainsCrossings, affectedCuts);
        if(crossingFound)
        {
            ++i;
        }
        else
        {
            unaffectedNew.push_back(*i);
            i = toAdd._list.erase(i);
        }
    }
    
    while(!affectedCuts.empty())
    {
        assertD(affectedCuts.front()->wasAffected());
        affectedCuts.front()->clearAffected();
        affectedCuts.pop_front();
    }

    DoRedirects(registry, toAdd, faceContainsCrossings);

    registry.addCuts(unaffectedNew);

#ifdef ASSERTIONS_ON
//    CheckNoSelfIntersections(registry);
#endif
}


static bool
ExistingCutIsContainedOrIdentical(
        const cLocalisedConvexShape& lcs,
        const cMeshCut& existingCut)
{
    tIntersection p;
    existingCut.getStartIntersection(p);
    tFace f = existingCut.getStartFace();
    if(lcs.testPointCollision(f, p))
    {
        return true;
    }
    if(lcs.testPointIsOnBoundary(f, p))
    {
        if(existingCut.isHole())
        {
            return false;
        }

        const cCircuitElement* e = existingCut.getStartElement()->getNext();
        assertD(e != existingCut.getStartElement());
        do
        {
            tIntersection p;
            e->getIntersectionForCornerAtStart(p);
            tFace f = e->faceCrossed(0);
            if(lcs.testPointCollision(f, p))
            {
                return true;
            }
            if(!lcs.testPointIsOnBoundary(f, p))
            {
                return false;
            }
            e = e->getNext();
        }
        while(e != existingCut.getStartElement());

        // all points on boundary -> cut exactly overlays the existing cut
        return true;
    }
    return false;
}

static bool
NewCutIsContained(const cCutRegistry& registry, const cMeshCut& cut)
{
    if(!cut.isInternalLoop())
    {
        // base obstacles expansions are only aded after
        // all external expansions have been combined
        return true;
    }
    tIntersection testVertex;
    cut.getStartIntersection(testVertex);
    tFace testVertexFace = cut.getStartFace();

    if(TestPointCollision(
            registry.refLookup(), testVertexFace, testVertex
            ))
    {
        return true;
    }
    return false;
}

void
ConnectBaseObstacleExpansion(cCutRegistry& registry, tMeshCircuits& toAdd, const cLocalisedConvexShape& lcs, cFlagVector& faceFlagsBuffer)
{
#ifdef ASSERTIONS_ON
//    CheckNoSelfIntersections(registry);
//    CheckNoSelfIntersections(registry.getFacesInMesh(), registry.getEdgesInMesh(), toAdd);
#endif

    std::vector<char> faceContainsCrossings(registry.getFacesInMesh(), false);
    std::list<cMeshCut*> affectedExisting;
    tMeshCutSequence affectedNew;
    tMeshCutSequence unaffectedNew;
    while(!toAdd.empty())
    {
        do
        {
            cHandle<cMeshCut> cut = toAdd.front()->front();
            toAdd.front()->pop_front();

        // SplitAtCrossings() will update the registry for any splits
            bool crossingFound = SplitAtCrossings(registry, *cut, faceContainsCrossings, affectedExisting);
            if(crossingFound)
            {
                affectedNew.push_back(cut);
            }
            else
            {
                if(!NewCutIsContained(registry, *cut))
                {
                    unaffectedNew.push_back(cut);
                }
            }
        }
        while(!toAdd.front()->empty());
        toAdd.pop_front();
    }

// check for any contained existing circuits

    {
        const std::vector<tSigned32>& facesOverlapped = lcs.refFacesOverlapped();
        std::list<tCutRegistryOwnerIterator> toRemove;

        tSigned32 j;
        for(j = 0; j < SizeL(facesOverlapped); j++)
        {
            tSigned32 faceIndex = facesOverlapped[j];
            std::list<cCutRegistryEntry>& lookup = registry.refLookup()[faceIndex];
            std::list<cCutRegistryEntry>::iterator i;
            for(i = lookup.begin(); i != lookup.end(); ++i)
            {
                cMeshCut& cut = **i->ownerNode;

              // we only want cuts that are rooted in this face
                if(i->element != cut.getStartElement() || i->faceIndex != 0)
                {
                    continue;
                }

              // and didnt intersect the circuit being added
                if(cut.wasAffected())
                {
                    continue;
                }

                if(ExistingCutIsContainedOrIdentical(lcs, cut))
                {
                    toRemove.push_back(i->ownerNode);
                }
            }
        }

        while(!toRemove.empty())
        {
            registry.removeFromUnaffected(toRemove.front());
            toRemove.pop_front();
        }
    }

    while(!affectedExisting.empty())
    {
        assertD(affectedExisting.front()->wasAffected());
        affectedExisting.front()->clearAffected();
        affectedExisting.pop_front();
    }

    if(!affectedNew.empty())
    {
        DoRedirects(registry, affectedNew, faceContainsCrossings);
    }

    registry.addCuts(unaffectedNew);

    assertF(!registry.containsInlineCorner());
#ifdef ASSERTIONS_ON
//    CheckNoSelfIntersections(registry);
#endif
}

