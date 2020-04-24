/*****************************************************************************
*
* File			: NtlPLEventHandler.h
* Author		: HyungSuk, Jang
* Copyright	: (аж)NTL
* Date			: 2006. 5. 09	
* Abstract		: presentation layer event handler
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_PLEVENT_HANDLER_H__
#define __NTL_PLEVENT_HANDLER_H__

#include "ceventhandler.h"

class CNtlPLEventHandler : public RWS::CEventHandler
{
public:

	CNtlPLEventHandler();
	~CNtlPLEventHandler();

	RwBool Create(void);
	void Destroy(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);
};

#endif