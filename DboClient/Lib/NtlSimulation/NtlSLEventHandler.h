/*****************************************************************************
 *
 * File			: NtlSLEventHandler.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 01	
 * Abstract		: Simulation layer event handler
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SLEVENT_HANDLER_H__
#define __NTL_SLEVENT_HANDLER_H__

#include "ceventhandler.h"

class CNtlSLEventHandler : public RWS::CEventHandler
{
public:

	CNtlSLEventHandler();
	~CNtlSLEventHandler();

	RwBool Create(void);
	void Destroy(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);
};

#endif