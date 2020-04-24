//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COMBINED_COST_CALCULATOR_INCLUDED
#define COMBINED_COST_CALCULATOR_INCLUDED

#include "libs/PathEngine_Core/interface/iMovementCostCalculator.h"
#include "libs/PathEngine_Core/interface/ShapesCostCalculator.h" //.. should not be included here, as is not directly required
#include "common/STL/vector.h"

template <class T1, class T2>
class cCombinedCostCalculator : public iMovementCostCalculator
{
    const T1* _b1;
    const std::vector<T2*>* _b2Vector;

public:

    cCombinedCostCalculator()
    {
        clear();
    }

    void clear()
    {
        _b1 = 0;
        _b2Vector = 0;
    }

    bool empty() const
    {
        return _b1 == 0 && _b2Vector == 0;
    }

    void setTarget1(const T1* b1)
    {
        assertD(_b1 == 0);
        _b1 = b1;
    }
    void setTarget2Vector(const std::vector<T2*>* b2Vector)
    {
        assertD(_b2Vector == 0);
        _b2Vector = b2Vector;
    }

// iMovementCostCalculator

    float calculate(cQueryContext& qc, const cInternalPosition& from, const tPoint& to, bool& blocked) const
    {
        float result = 0.f;
        if(_b1)
        {
            result += _b1->calculate(qc, from, to, blocked);
            if(blocked)
            {
                return 0.f;
            }
        }
        if(_b2Vector)
        {
            tSigned32 i;
            for(i = 0; i < SizeL(*_b2Vector); ++i)
            {
                result += (*_b2Vector)[i]->calculate(qc, from, to, blocked);
                if(blocked)
                {
                    return 0.f;
                }
            }
        }
        return result;
    }
};

#endif
