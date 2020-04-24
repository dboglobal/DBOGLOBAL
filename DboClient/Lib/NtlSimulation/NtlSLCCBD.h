/******************************************************************************
* File			: NtlSLCCBD.h
* Author		: Daneos
* Copyright		: Daneos
* Date			: 2019. 1. 15
* Abstract		:
*****************************************************************************
* Desc			: ccbd information
*****************************************************************************/
#pragma once

// core
#include "ceventhandler.h"

class CNtlCCBD : public RWS::CEventHandler
{

public:

	CNtlCCBD();
	virtual ~CNtlCCBD();

	virtual RwBool		Create(void);
	void				Update(RwReal fElapsed);
	void				Destroy();

	virtual void		HandleEvents(RWS::CMsg &msg);

private:

	void				Clear();

public:

	inline BYTE			GetStage() { return m_byStage; }

protected:

	BYTE				m_byStage;
};