//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_ICOLLISIONCONTEXT_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_ICOLLISIONCONTEXT_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iCollisionContext_Wrapper : public iCollisionContext, public iAPIObjectWrapper
{
public:

    typedef iCollisionContext tWrapped;

	iCollisionContext* _target;

    static tSigned32 interfaceIndex()
    {
        return 4;
    }

    void
    destroy();

    void
    addAgent(iAgent* agent);

    void
    removeAgent(iAgent* agent);

    tSigned32
    getNumberOfAgents() const;

    iAgent*
    getAgent(tSigned32 index) const;

    bool
    includes(iAgent* agent) const;

    void
    addObstacleSet(iObstacleSet* context);

    void
    removeObstacleSet(iObstacleSet* context);

    tSigned32
    getNumberOfObstacleSets() const;

    iObstacleSet*
    getObstacleSet(tSigned32 index) const;

    void
    updateCollisionPreprocessFor(iShape* shape) const;

    void
    updatePathfindingPreprocessFor(iShape* shape) const;

    void
    setSurfaceTypeTraverseCost(tSigned32 surfaceType, float cost);

    void
    setSurfaceTypeTraverseCostDirection(tSigned32 surfaceType, tSigned32 directionVectorX, tSigned32 directionVectorY);

    void
    setQueryBounds(const cHorizontalRange& bounds);

    void
    clearQueryBounds();

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
