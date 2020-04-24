/******************************************************************************
* File			: LogoGui.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 2008. 11. 12
* Abstract		: 
* Update		: 
*****************************************************************************
* Desc			: NTL Logo
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

class CLogoGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CLogoGui(const RwChar* pName);
	virtual ~CLogoGui();

	RwBool			Create();
	virtual VOID	Destroy();

protected:
	CLogoGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	int				CallBackMoviePlayStop();

protected:
};