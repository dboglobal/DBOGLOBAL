#ifndef __SERVER_AIS_CONTROLFACTORY__
#define __SERVER_AIS_CONTROLFACTORY__


#include "ControlStateFactory.h"

class CControlState;
class CNpc;

class CAIScriptControlFactoy : public CControlStateFactory
{

public:
	CAIScriptControlFactoy(CNpc* pOwner);
	virtual ~CAIScriptControlFactoy();


public:


	virtual CControlState* CreateScriptControlState(int controlStateID);

	virtual CControlState* CreateNodeControlState(int controlStateID, CControlState* controlstate);

	virtual bool CanCreateControlState(int controlStateID);

	virtual bool CanCreateControlStateLoopCountCheck(DWORD dwScriptNodeIndex, DWORD & rdwLoopCount);


private:

	CNpc*					m_pOwner;

};


#endif