/******************************************************************************
* File			: GMGuiGroup.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 11. 18
* Abstract		: 
*****************************************************************************
* Desc			: GM Mode로 들어갔을 때 사용되는 Gui의 Group
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

/**
* \ingroup GM의 Gui Group
*/

class CGMQuickSlotGui;

class CGMGuiGroup : public RWS::CEventHandler
{
public:
	CGMGuiGroup();
	~CGMGuiGroup();

	RwBool	Create();
	VOID	Destroy();

	VOID	HandleEvents( RWS::CMsg& msg );	

protected:
	CGMQuickSlotGui*			m_pGMQuickSlot;
};