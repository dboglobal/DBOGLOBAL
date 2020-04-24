//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PATHENGINE_CORE__CALL_BACK_GOAL_CONDITIONS_INCLUDED
#define PATHENGINE_CORE__CALL_BACK_GOAL_CONDITIONS_INCLUDED

#include "PointsOfVisibilityState.h"
#include "i_pathengine.h"

template <class tBase>
class cCallBackGoalConditions
{
    const tBase& _base;
    iQueryCallBack& _callBack;
    tSigned32 _callBackFrequency;
    mutable tSigned32 _callBackCount;

public:

    cCallBackGoalConditions(const tBase& base, iQueryCallBack& callBack) :
      _base(base),
      _callBack(callBack)
    {
        _callBackFrequency = _callBack.desiredCallBackFrequency();
        assertR(_callBackFrequency >= 1);
        _callBackCount = _callBackFrequency;
    }

// goal conditions interface

    bool
    isGoal(cPointsOfVisibilityState state) const
    {
        --_callBackCount;
        if(_callBackCount == 0)
        {
            _callBackCount = _callBackFrequency;
            if(_callBack.abort())
            {
                return true;
            }
        }
        return _base.isGoal(state);
    }
    tUnsigned32
    minimumCostToGoal(tSigned32 pointIndex) const
    {
        return _base.minimumCostToGoal(pointIndex);
    }
};

#endif
