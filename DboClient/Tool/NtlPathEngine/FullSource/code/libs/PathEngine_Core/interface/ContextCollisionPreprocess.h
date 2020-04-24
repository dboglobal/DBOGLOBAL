//**********************************************************************
//
// Copyright (c) 2004-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTEXT_COLLISION_PREPROCESS_INCLUDED
#define CONTEXT_COLLISION_PREPROCESS_INCLUDED

#include "libs/PathEngine_Core/interface/ManagedShapeSet.h"
#include "platform_common/ThreadSafeHandleTarget.h"
#include "common/Handle.h"
#include "common/STL/vector.h"

class cAgent;

class cContextCollisionPreprocess : public bThreadSafeHandleTarget
{
    cHandle<cManagedShapeSet> _shapeSet;

    // these agents could potentially have been deleted by another thread
    // so useage must be restricted to comparison with other agent pointers, returning pointers for colliding shapes etc.
    std::vector<cAgent*> _ownerAgents;

    bool _isValid;

public:

    cContextCollisionPreprocess(tSigned32 facesInMesh) :
     _isValid(false)
    {
        _shapeSet = new cManagedShapeSet(facesInMesh);
    }

    bool isValid() const
    {
        return _isValid;
    }
    void setInvalid()
    {
        _isValid = false;
    }
    void setValid()
    {
        _isValid = true;
    }

    void clear()
    {
        _shapeSet->clear();
        _ownerAgents.resize(0);
    }
    void push_back(cAgent* agent, cHandle<cShapeCollisionPreprocess> scp)
    {
        _shapeSet->push_back(scp);
        _ownerAgents.push_back(agent);
    }
    void setCompleted()
    {
        _shapeSet->setCompleted();
    }

    const cLocalisedConvexShapeSet& refShapeSet() const
    {
        return _shapeSet->refShapeSet();
    }
    cHandle<cManagedShapeSet>
    getShapeSet() const
    {
        return _shapeSet;
    }

    tSigned32 getIndexForAgent(const cAgent* agent) const
    {
        tSigned32 i;
        for(i = 0; i != _ownerAgents.size(); ++i)
        {
            if(_ownerAgents[i] == agent)
            {
                return i;
            }
        }
        return -1;
    }
    cAgent*
    getOwnerAgent(tSigned32 index) const
    {
        return _ownerAgents[index];
    }

    bool testPointCollision_IgnoringOneAgent(const cInternalPosition& p, const cAgent* agentToIgnore) const;
    tSigned32 getAllAgentsOverlapped(const cInternalPosition& p, cAgent** resultsBuffer) const;
    void firstCollision(
        tFace startF, const tLine& line, bool& collides, cAxisFraction& distanceAlong, tLine& collidingLine,
        tSigned32& collidingShape
        );
};

#endif
