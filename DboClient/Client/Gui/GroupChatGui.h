/******************************************************************************
* File			: GroupChatGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 10. 17
* Abstract		: 
*****************************************************************************
* Desc			: 채팅방을 생성하여 그 방안의 사람들과 이야기를 한다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

class CGroupChatGui : public CNtlPLGui
{
public:
	CGroupChatGui(const RwChar* pName);
	virtual ~CGroupChatGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);
	virtual VOID	HandleEvents( RWS::CMsg &msg );

protected:
	CGroupChatGui() {}
};