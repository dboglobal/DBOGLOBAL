//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef C_OBSTACLE_SET_INCLUDED
#define C_OBSTACLE_SET_INCLUDED

#include "libs/Geometry/interface/tPoint_Header.h"
#include "common/ModifiedStateTracker.h"
#include "common/Handle.h"
#include "platform_common/ThreadSafeHandleTarget.h"
#include "platform_common/Mutex.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"
#include "i_pathengine.h"

class cQueryContext;
class cMesh;
class cAgent;
class cCollisionPreprocess;
class cPathfindPreprocess;
class cContextCollisionPreprocess;
class cContextPathfindPreprocess;
class cCompoundContextCollisionPreprocess;
class iAPIObjectWrapper;

class cObstacleSet : public iObstacleSet, public bThreadSafeHandleTarget
{
  // immutable
    cMesh* _mesh;
    tSigned32 _indexInMesh; // for removal from meshes held vector, and for agents to track containing sets
    bool _markedForPreprocessing;

  // protected by _mesh->_agentsAndObstacleSetsMutex
    std::vector<cAgent*> _agentVector;

    cMutex _preprocessMutex;
    cHandle<cContextCollisionPreprocess> _unexpandedPreprocess;
    mutable std::vector<cHandle<cContextCollisionPreprocess> > _collisionPreprocess;
    cModifiedStateTracker _pathfindingStateTracker;
    mutable std::vector<cHandle<cPathfindPreprocess> > _pathfindPreprocess;

    bool _hasBeenDestroyed;

    void collisionAndPathfindingStateModified();
    void pathfindingStateOnlyModified();

public:

    iAPIObjectWrapper* _wrapper;

    cObstacleSet(cMesh* mesh, const char *const* attributes);
    void setIndexInMesh(tSigned32 value); // must be called post construction
    
    ~cObstacleSet();

// iObstacleSet

    void destroy();
    void addAgent(iAgent* agent);
    void removeAgent(iAgent* agent);
    tSigned32 getNumberOfAgents() const;
    iAgent* getAgent(tSigned32 index) const;
    bool includes(iAgent* agent) const;
    tSigned32 size() const;
    bool empty() const;
    void updateCollisionPreprocessFor(iShape* shape) const;
    void updatePathfindingPreprocessFor(iShape* shape) const;
    void operator delete(void* p) {::operator delete(p);}

//=================

    cHandle<cContextCollisionPreprocess> getUnexpandedPreprocess(cQueryContext& qc);
    cHandle<cContextCollisionPreprocess> getCollisionPreprocessFor(cQueryContext& qc, tSigned32 i) const;
    cHandle<cPathfindPreprocess> getPathfindPreprocessFor(cQueryContext& qc, tSigned32 i) const;

    void addAgentsToPathfindPreprocess(
        cQueryContext& qc, 
        tSigned32 expansionShapeIndex,
        const cCollisionPreprocess& cp, const cPathfindPreprocess& pp,
        bool boundsValid,
        const tPoint& boundsMin, const tPoint& boundsMax,
        cContextPathfindPreprocess& addTo
        );

    void notifyAgentChangeAffectingCollisionAndPathfindingState();
    void notifyAgentChangeAffectingPathfindingStateOnly();

    void addUnexpandedPreprocess(cQueryContext& qc, cCompoundContextCollisionPreprocess& addTo);
    void addCollisionPreprocessFor(cQueryContext& qc, tSigned32 index, cCompoundContextCollisionPreprocess& addTo);

    tUnsigned32 getPathfindingStateHandle();
    bool pathfindingStateHasBeenModifiedSince(tUnsigned32 stateHandle) const;

    bool hasBeenDestroyed() const
    {
        return _hasBeenDestroyed;
    }

    bool isMarkedForPreprocessing() const
    {
        return _markedForPreprocessing;
    }
};

inline cObstacleSet* FromI(iObstacleSet* ptr)
{
    return static_cast<cObstacleSet*>(ptr);
}

#endif
