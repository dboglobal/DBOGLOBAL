#ifndef __SERVER_SPS_CONTROLFACTORY__
#define __SERVER_SPS_CONTROLFACTORY__


#include "ControlStateFactory.h"

class CControlState;
class CNpc;

class CPlayScriptControlFactoy : public CControlStateFactory
{

public:
	CPlayScriptControlFactoy(CNpc* pOwner);
	virtual ~CPlayScriptControlFactoy();


public:


	virtual CControlState*		CreateScriptControlState(int ctrlScrNodeID);

	virtual bool				CanCreateControlState(int ctrlScrNodeID);


private:

	CNpc*					m_pOwner;

};


#endif