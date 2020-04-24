#pragma once

#include "NtlWorldCommon.h"
#include "NtlPLEvent.h"

class CNtlFieldColorManager : public RWS::CEventHandler
{
private:
	static CNtlFieldColorManager* g_pFieldColorManager;

public:
	CNtlFieldColorManager();
	virtual ~CNtlFieldColorManager();

	static CNtlFieldColorManager*	GetInstance() { return g_pFieldColorManager; }
	static void						CreateInstance();
	static void						DestroyInstance();
	virtual void					HandleEvents(RWS::CMsg &pMsg);

	RwBool							Create();
	void							Destroy();
	void							Update(RwReal fElapsed);
	
	const RwRGBAReal*				GetColorf() { return &m_clrf; }

protected:
	void							RunableFade(RwRGBAReal* pColorf, RwReal fTime);
	void							UpdateFade(RwReal fElapsed);

	void							SetFieldColor(RwRGBA* pColor);

protected:
	RwRGBAReal						m_clrf;
	RwRGBAReal						m_clrfOld;
	RwRGBAReal						m_clrfNew;
	RwReal							m_bFading;

	RwReal							m_fFadeTime;
	RwReal							m_fFadeElapsed;
};