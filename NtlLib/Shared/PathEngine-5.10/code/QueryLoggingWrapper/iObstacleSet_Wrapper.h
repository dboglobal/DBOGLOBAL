//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_IOBSTACLESET_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_IOBSTACLESET_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iObstacleSet_Wrapper : public iObstacleSet, public iAPIObjectWrapper
{
public:

    typedef iObstacleSet tWrapped;

	iObstacleSet* _target;

    static tSigned32 interfaceIndex()
    {
        return 5;
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

    tSigned32
    size() const;

    bool
    empty() const;

    void
    updateCollisionPreprocessFor(iShape* shape) const;

    void
    updatePathfindingPreprocessFor(iShape* shape) const;

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
