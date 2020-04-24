/******************************************************************************
* File			: GroupChatManager.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 10. 17
* Abstract		: 
*****************************************************************************
* Desc			: 그룹 채팅 관리자
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

class CGroupChatGui;

class CGroupChatManager : public CNtlPLGui, public RWS::CEventHandler
{
public:
	typedef std::map<RwUInt32, CGroupChatGui*>			MAP_CHATGUI;

	CGroupChatManager(const RwChar* pName);
	virtual ~CGroupChatManager();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

	VOID		AddText(RwUInt8 byChatType, const WCHAR* pwcText);

protected:
	CGroupChatManager() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

protected:
};