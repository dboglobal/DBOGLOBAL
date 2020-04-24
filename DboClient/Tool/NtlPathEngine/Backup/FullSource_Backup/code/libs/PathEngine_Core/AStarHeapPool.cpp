//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/AStarHeapPool.h"
#include "AStarHeap.h"
#include "common/MutexLock.h"

cAStarHeapPool::cAStarHeapPool() :
 _heap(1),
 _inUse(1, false)
{
     _heap[0] = new cAStarHeap;
}
cAStarHeapPool::~cAStarHeapPool()
{
    tSigned32 i;
    for(i = 0; i < SizeL(_heap); ++i)
    {
        assertR(!_inUse[i]);
        delete _heap[i];
    }
}

cAStarHeap&
cAStarHeapPool::lockHeap()
{
    cMutexLock lock(_mutex);
    tSigned32 i;
    for(i = 0; i < SizeL(_heap); ++i)
    {
        if(!_inUse[i])
        {
            break;
        }
    }
    if(i == _heap.size())
    {
        _heap.push_back(new cAStarHeap);
        _inUse.push_back(false);
    }
    _inUse.setBit(i, true);
    return *_heap[i];
}
void
cAStarHeapPool::releaseHeap(cAStarHeap& heap)
{
    cMutexLock lock(_mutex);
    tSigned32 i;
    for(i = 0; i < SizeL(_heap); ++i)
    {
        if(&heap == _heap[i])
        {
            break;
        }
    }
    assertD(i != _heap.size());
    assertD(_inUse[i]);
    _inUse.setBit(i, false);
}
