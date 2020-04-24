//**********************************************************************
//
// Copyright (c) 2003-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/STL/vector.h"

class iPointLike;

namespace nTransformTree
{

class iOperation
{
public:

    virtual ~iOperation() {}

    virtual int resultFor(const iPointLike& p) const = 0;
    virtual int sourceFor(const iPointLike& p) const = 0;
};

class cTree
{
    // negative values indicate no operations performed in a given element
    std::vector<tSigned32> _firstOperation;
    std::vector<tSigned32> _lastOperation; // indexed by current state

    // per operation
    std::vector<iOperation*> _operations;
    //... optimise replace vector of vectors with something more efficient and cache friendly
    std::vector<std::vector<tSigned32> > _prevOperation;
    std::vector<std::vector<tSigned32> > _nextOperation;

//    void validate() const;

public:

    cTree(tSigned32 topSourceElement);
    ~cTree();

    void addOperation(iOperation* operation, const std::vector<tSigned32>& sources, const std::vector<tSigned32>& results);
    void addNullOperation(tSigned32 source, tSigned32 result);

    void remap(const std::vector<tSigned32>& elementRemaps, tSigned32 elementsAfterRemap);

    tSigned32 transformDown(tSigned32 startElement, const iPointLike& p) const;
    tSigned32 transformUp(tSigned32 startElement, const iPointLike& p) const;
};

}
