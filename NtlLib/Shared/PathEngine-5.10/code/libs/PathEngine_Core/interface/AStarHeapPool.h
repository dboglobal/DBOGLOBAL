//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Mutex.h"
#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"
class cAStarHeap;

class cAStarHeapPool
{
    cMutex _mutex;
    std::vector<cAStarHeap*> _heap;
    cBitVector _inUse;

    // prevent copying and assignment
    cAStarHeapPool(const cAStarHeapPool&);
    const cAStarHeapPool& operator=(const cAStarHeapPool&);

    cAStarHeap& lockHeap();
    void releaseHeap(cAStarHeap& heao);

public:

    cAStarHeapPool();
    ~cAStarHeapPool();

    friend class cAStarLock;
};

class cAStarLock
{
public:

    cAStarHeapPool& _pool;
    cAStarHeap& _heap;

    cAStarLock(cAStarHeapPool& pool) :
     _pool(pool),
     _heap(_pool.lockHeap())
    {
    }
    ~cAStarLock()
    {
        _pool.releaseHeap(_heap);
    }
};
