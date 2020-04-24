/******************************************************************************
* File			: DeveloperPreviewGui.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 2009. 1. 15
* Abstract		: 
*****************************************************************************
* Desc			: 
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

class CDeveloperPreviewGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CDeveloperPreviewGui(const RwChar* pName);
	virtual ~CDeveloperPreviewGui();

	RwBool		Create();
	VOID		Destroy();

protected:
	CDeveloperPreviewGui() {};
	virtual VOID	HandleEvents( RWS::CMsg &msg );
};