/*****************************************************************************
 *
 * File			: DboEventHandler.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 29	
 * Abstract		: DBO event handler
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DBO_EVENT_HANDLER_H__
#define __DBO_EVENT_HANDLER_H__

#include "ceventhandler.h"

class CDboEventHandler : public RWS::CEventHandler
{
public:

	CDboEventHandler();
	~CDboEventHandler();

	RwBool Create(void);
	void Destroy(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);
};



#endif
