/*****************************************************************************
 *
 * File			: LobbyGuiGroup.h
 * Author		: Hong Sungbock
 * Copyright	: (аж)NTL
 * Date			: 2006. 12. 7	
 * Update		: 2007. 10. 10	
 * Abstract		: DBO lobby stage gui group.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#pragma once

#include "ceventhandler.h"

class CCharCreateGuiGui;
class CCharSelectGui;
class CServerSelectGui;
class CBackboardGui;

class CCharGuiGroup : public RWS::CEventHandler
{
public:	
	CCharCreateGuiGui*			m_pCharCreate;
	CCharSelectGui*				m_pCharSelect;
	CServerSelectGui*			m_pServerSelect;
	CBackboardGui*				m_pTurnOverBackboard;
	
public:
	CCharGuiGroup();
	~CCharGuiGroup();

	RwBool Create(void);
	void Destroy(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);
};
