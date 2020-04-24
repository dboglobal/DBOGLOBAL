/*****************************************************************************
 *
 * File			: NtlSoundEventHandler.h
 * Author		: Houng Sungbock
 * Copyright	: (аж)NTL
 * Date			: 2007. 5. 29
 * Abstract		: Ntl sound event handler
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#pragma once

#include "ceventhandler.h"

class CNtlSoundEventHandler : public RWS::CEventHandler
{
public:

	CNtlSoundEventHandler();
	~CNtlSoundEventHandler();

	RwBool Create(void);
	void Destroy(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);
};