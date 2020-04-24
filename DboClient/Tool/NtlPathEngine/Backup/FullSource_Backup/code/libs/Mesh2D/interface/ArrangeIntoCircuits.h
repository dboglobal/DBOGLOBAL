//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ARRANGE_INTO_CIRCUITS_INCLUDED
#define ARRANGE_INTO_CIRCUITS_INCLUDED

#include "libs/Mesh2D/interface/tMesh.h"
#include "common/Handle.h"
#include "common/HandleTarget.h"
#include "common/HandleList.h"
#include "common/STL/list.h"

template <class T>
typename cHandleList<T>::iterator
FirstEntryAfterExit_Convex(tEdge exit,
                            cHandleList<T>& list)
{
    typedef typename cHandleList<T>::iterator iterator;
    while(1)
    {
        iterator i;
        iterator result = list._list.end();
        for(i = list._list.begin(); i != list._list.end(); ++i)
        {
            tEdge entry = (*i)->entryEdge();
            if(entry == exit)
            {
                assertD(result == list._list.end());// we depend on the fact that a given edge can be entered at most once
                result = i;
            }
        }
        if(result != list._list.end())
        {
            return result;
        }

        exit = exit.twin().prev().twin();
    }
}

// assumes that at most one of the cuts passed in enters or exits any given mesh edge
// assumes also that entry will always come after entry within any given mesh edge
// these assumptions will be valid, for example, for overlapping polygons or boolean results
// leaves 'cuts' empty
template <class T>
void ArrangeIntoCircuits_Convex(cHandleList<T>& cuts,
                        cHandleList<cHandleList<T> >& circuits)
{
    if(cuts._list.empty())
    {
        return;
    }
    if(cuts._list.size() == 1)
    {
    // internal cut or circuit with only one cut
        circuits._list.push_back(new cHandleList<T>);
        circuits._list.back()->_list.swap(cuts._list);
        return;
    }

    do
    {
        circuits._list.push_back(new cHandleList<T>);
        cHandle<T> exiting_cut = cuts._list.front();
        circuits._list.back()->_list.push_back(exiting_cut);

        while(1)
        {
            tEdge exit = exiting_cut->exitEdge();
            typename cHandleList<T>::iterator next_i =
                FirstEntryAfterExit_Convex(exit, cuts);

            if(next_i == cuts._list.begin())
            {
                break;
            }

            exiting_cut = *next_i;
            circuits._list.back()->_list.push_back(exiting_cut);
            cuts._list.erase(next_i);
        }

        cuts._list.pop_front();
    }
    while(!cuts._list.empty());
}


template <class T>
typename cHandleList<T>::iterator
FirstEntryAfterExit(tEdge exitEE,
                    const tLine& exitLine,
                    cHandleList<T>& list)
{
    typedef typename cHandleList<T>::iterator iterator;

// first, entries in the same element edge as the exit

    iterator i;
    iterator result = list._list.end();
    for(i = list._list.begin(); i != list._list.end(); ++i)
    {
        tEdge entry = (*i)->entryEdge();
        if(entry == exitEE
            &&
            EntryIsAfterExit(exitEE, exitLine, (*i)->refEntryLine())
            )
        {
            if(result == list._list.end()
                ||
                EntryIsAfterEntry(exitEE, (*i)->refEntryLine(), (*result)->refEntryLine())
                )
            {
                result = i;
            }
        }
    }
    if(result != list._list.end())
    {
        return result;
    }

// then follow external element edges

    tEdge loopCheck = exitEE;

    exitEE = exitEE.twin().prev().twin();

    while(1)
    {
        //.. TODO add check for infinite loop

        result = list._list.end();
        for(i = list._list.begin(); i != list._list.end(); ++i)
        {
            tEdge entry = (*i)->entryEdge();
            if(entry == exitEE)
            {
                if(result == list._list.end()
                    ||
                    EntryIsAfterEntry(exitEE, (*i)->refEntryLine(), (*result)->refEntryLine())
                    )
                {
                    result = i;
                }
            }
        }
        if(result != list._list.end())
        {
            return result;
        }

        assertD(exitEE != loopCheck);

        exitEE = exitEE.twin().prev().twin();
    }
}

// leaves 'cuts' empty
template <class T>
void ArrangeIntoCircuits(cHandleList<T>& cuts,
                        cHandleList<cHandleList<T> >& circuits)
{
    if(cuts._list.empty())
    {
        return;
    }
    if(cuts._list.size() == 1)
    {
    // internal cut or circuit with only one cut
        circuits._list.push_back(new cHandleList<T>);
        circuits._list.back()->_list.swap(cuts._list);
        return;
    }

    do
    {
        circuits._list.push_back(new cHandleList<T>);
        cHandle<T> exiting_cut = cuts._list.front();
        circuits._list.back()->_list.push_back(exiting_cut);

        while(1)
        {
            tEdge exit = exiting_cut->exitEdge();
            typename cHandleList<T>::iterator next_i =
                FirstEntryAfterExit(exit, exiting_cut->refExitLine(), cuts);

            if(next_i == cuts._list.begin())
            {
                break;
            }

            exiting_cut = *next_i;
            circuits._list.back()->_list.push_back(exiting_cut);
            cuts._list.erase(next_i);
        }

        cuts._list.pop_front();
    }
    while(!cuts._list.empty());
}

#endif
