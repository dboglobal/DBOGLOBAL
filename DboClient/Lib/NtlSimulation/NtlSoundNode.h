/*****************************************************************************
* File			: NtlSoundNode.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 
* Update		: 2009. 7. 10
* Abstract		: Simulation sound Node
*****************************************************************************
* Desc         : 
*****************************************************************************/

#ifndef __NTL_SOUND_NODE_H__
#define __NTL_SOUND_NODE_H__

// core
#include "ceventhandler.h"

class CNtlSoundNode
{
public:
	CNtlSoundNode() {};
	virtual ~CNtlSoundNode() {};

	virtual RwBool			Create() = 0;
	virtual VOID			Update(RwReal fElapsed) = 0;
	virtual VOID			Destory() = 0;
	virtual VOID			HandleEvents(RWS::CMsg &pMsg) {}
};

#endif