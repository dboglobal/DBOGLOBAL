//**********************************************************************
//
// Copyright (c) 2003-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/TransformTree.h"
#include "libs/Instrumentation/interface/MemoryReport.h"
#include "common/STL_Helpers.h"
#include "common/interface/Assert.h"
#include "common/Containers/Array.h"

using std::vector;

namespace nTransformTree
{

cTree::cTree(tSigned32 topSourceElement)
{
    _firstOperation.resize(topSourceElement + 1);
    _lastOperation.resize(topSourceElement + 1);
    tSigned32 i;
    for(i = 0; i != topSourceElement + 1; ++i)
    {
        _firstOperation[i] = -2 - i;
        _lastOperation[i] = -2 - i;
    }
}

cTree::~cTree()
{
#ifdef MEMORY_REPORTS_ON
    {
        MEMORY_REPORT_SCOPE("transform tree: sources and results");
        CompletelyClear(_prevOperation);
        CompletelyClear(_nextOperation);
    }
#endif
    {
        MEMORY_REPORT_SCOPE("transform tree: allocated operations");
        tSigned32 i;
        for(i = 0; i < SizeL(_operations); i++)
        {
            delete _operations[i];
        }
    }
#ifdef MEMORY_REPORTS_ON
    {
        MEMORY_REPORT_SCOPE("transform tree: other vectors");
        CompletelyClear(_firstOperation);
        CompletelyClear(_lastOperation);
        CompletelyClear(_operations);
    }
#endif
}

//void
//cTree::validate() const
//{
//    assertD(_transformDown.size() == _transformUp.size());
//    assertD(_operations.size() == _sources.size());
//    assertD(_operations.size() == _results.size());
//    cArray<char> elementUsedAsSource(SizeL(_transformDown), false);
//    cArray<char> elementUsedAsResult(SizeL(_transformDown), false);
//    tSigned32 elements = static_cast<tSigned32>(SizeL(_transformDown));
//    tSigned32 i;
//    for(i = 0; i < SizeL(_operations); i++)
//    {
//        tSigned32 j;
//        for(j = 0; j < SizeL(_sources[i]); j++)
//        {
//            tSigned32 element = _sources[i][j];
//            assertD(element >= 0);
//            assertD(element < elements);
//            elementUsedAsSource[element] = true;
//            assertD(_transformDown[element] == i + 1);
//        }
//        for(j = 0; j < SizeL(_results[i]); j++)
//        {
//            tSigned32 element = _results[i][j];
//            assertD(element >= 0);
//            assertD(element < elements);
//            elementUsedAsResult[element] = true;
//            assertD(_transformUp[element] == i + 1);
//        }
//    }
//    for(i = 0; i < SizeL(_transformDown); i++)
//    {
//        if(!elementUsedAsSource[i])
//        {
//            assertD(_transformDown[i] == 0);
//        }
//        if(!elementUsedAsResult[i])
//        {
//            assertD(_transformUp[i] == 0);
//        }
//    }
//}

void
cTree::addOperation(iOperation* operation, const std::vector<tSigned32>& sources, const std::vector<tSigned32>& results)
{
    tSigned32 newOperation = SizeL(_operations);
    _operations.push_back(operation);
    _prevOperation.push_back(vector<tSigned32>());
    _prevOperation.back().resize(sources.size());
    _nextOperation.push_back(vector<tSigned32>());
    _nextOperation.back().resize(results.size());
    tSigned32 i;
    for(i = 0; i < SizeL(sources); i++)
    {
        tSigned32 source = sources[i];
        tSigned32 lastOperation = _lastOperation[source];
        if(lastOperation < 0)
        {
        // no operations yet on this source element
            _firstOperation[source] = newOperation;
        }
        else
        {
            // update results of the last operation on this element to point to the new operation
            for(tSigned32 j = 0; j != SizeL(_nextOperation[lastOperation]); ++j)
            {
                if(_nextOperation[lastOperation][j] == -2 - source)
                {
                    _nextOperation[lastOperation][j] = newOperation;
                }
            }
        }
        _prevOperation.back()[i] = lastOperation;
    }
    for(i = 0; i < SizeL(results); i++)
    {
        tSigned32 result = results[i];
        if(result >= SizeL(_lastOperation))
        {
            _lastOperation.resize(result + 1, -1);
        }
        _lastOperation[result] = newOperation;
        _nextOperation.back()[i] = -2 - result;
    }
}

void
cTree::remap(const vector<tSigned32>& elementRemaps, tSigned32 elementsAfterRemap)
{
    tSigned32 i;
    for(i = 0; i != SizeL(_firstOperation); ++i)
    {
        if(_firstOperation[i] >= 0)
        {
            continue;
        }
        assertD(_firstOperation[i] != -1);
        tSigned32 beforeRemap = -2 - _firstOperation[i];
        tSigned32 afterRemap = elementRemaps[beforeRemap];
        _firstOperation[i] = -2 - afterRemap;
    }
    assertD(elementRemaps.size() == _lastOperation.size());
    {
        std::vector<tSigned32> remapped_lastOperation(elementsAfterRemap);
        for(i = 0; i != SizeL(elementRemaps); ++i)
        {
            if(elementRemaps[i] == -1)
            {
                continue;
            }
            remapped_lastOperation[elementRemaps[i]] = _lastOperation[i];
        }
        _lastOperation.swap(remapped_lastOperation);
    }
    for(i = 0; i != SizeL(_nextOperation); ++i)
    {
        for(tSigned32 j = 0; j != SizeL(_nextOperation[i]); ++j)
        {
            if(_nextOperation[i][j] < 0)
            {
                assertD(_nextOperation[i][j] != -1);
                tSigned32 beforeRemap = -2 - _nextOperation[i][j];
                tSigned32 afterRemap = elementRemaps[beforeRemap];
                _nextOperation[i][j] = -2 - afterRemap;
            }
        }
    }
}

tSigned32
cTree::transformDown(tSigned32 element, const iPointLike& p) const
{
    tSigned32 operation = _firstOperation[element];
    while(operation >= 0)
    {
        int result = _operations[operation]->resultFor(p);
        operation = _nextOperation[operation][result];
    }
    assertD(operation != -1);
    return -2 - operation;
}
tSigned32
cTree::transformUp(tSigned32 element, const iPointLike& p) const
{
    tSigned32 operation = _lastOperation[element];
    while(operation >= 0)
    {
        int source = _operations[operation]->sourceFor(p);
        operation = _prevOperation[operation][source];
    }
    assertD(operation != -1);
    return -2 - operation;
}

}
