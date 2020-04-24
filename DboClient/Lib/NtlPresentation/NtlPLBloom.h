#pragma once


#include <RwCore.h>
#include "NtlDebug.h"
#include "NtlPLAttach.h"
#include "NtlPLApi.h"
#include "ceventhandler.h"

#define dBLOOM_SWITCHING_TIME (1.0f)

class CNtlPLBloom : public CNtlPLEntity, public RWS::CEventHandler
{
public:
	CNtlPLBloom(void);
	virtual ~CNtlPLBloom(void);

private:
	RwReal	m_fTotalMonoPower;

	// 0 : current, 1 : src, 2 : dst
	RwReal	m_MonoPower[3];

	RwReal	m_RestTime4Change;

	// Lightning
	RwReal	m_fTimeFadeIn;	
	RwReal	m_fTimeFadeOut;
	RwReal	m_fTimeLightning;

public:
	virtual RwBool	Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void	Destroy();
	virtual void	HandleEvents(RWS::CMsg &pMsg);
	virtual RwBool	SetProperty(const CNtlPLProperty *pData);
	virtual RwBool	Update(RwReal fElapsed);
	virtual RwBool	Render(void);

public:
	virtual RwReal	GetMonoFactor() { return m_fTotalMonoPower; }

	// Lightning
	virtual void	RunableLightning(RwReal fTimeFadeIn, RwReal fTimeFadeOut);
};
