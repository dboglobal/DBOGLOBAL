//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_MOVEMENT_COST_CALCULATOR_INCLUDED
#define I_MOVEMENT_COST_CALCULATOR_INCLUDED

#include "libs/Geometry/interface/tPoint_Header.h"

class cQueryContext;
class cInternalPosition;

class iMovementCostCalculator
{
public:

    virtual ~iMovementCostCalculator() {}
    virtual float calculate(cQueryContext& qc, const cInternalPosition& from, const tPoint& to, bool& blocked) const = 0;
};

#endif
