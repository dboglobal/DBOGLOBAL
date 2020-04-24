#ifndef __NTL_PLLIGHT_H__
#define __NTL_PLLIGHT_H__


#include <RwCore.h>
#include "NtlDebug.h"
#include "NtlPLAttach.h"
#include "NtlPLApi.h"
#include "ceventhandler.h"
#include "ntlworldcommon.h"

#include "NtlWorldRpLightManager.h"

#define dLIGHT_EFFECT_SWITCHING_TIME (3.0f)

class CNtlPLLight : public CNtlPLEntity, public RWS::CEventHandler, public CNtlWorldRpLightProxy
{
public:
	CNtlPLLight(void);
	virtual	~CNtlPLLight(void);

private:
	RpLight*	m_pLtAmbient;
	RpLight*	m_pLtDirectional[dNTL_PL_DIRECTIONAL_LIGHT_NUM];
/*
	// test
	RpLight*	m_pLtOmni;
	RwV3d		m_woody;
	*/

	// 0 : current, 1 : src, 2 : dst
	RwRGBAReal	m_ClrAmbient[3];
	RwRGBAReal	m_ClrDirectional[dNTL_PL_DIRECTIONAL_LIGHT_NUM][3];
	RwV3d		m_AngDirectional[dNTL_PL_DIRECTIONAL_LIGHT_NUM][3];
	RwReal		m_RestTime4Change[dNTL_PL_DIRECTIONAL_LIGHT_NUM + 1]; // last array rest time = Ambient time
	RwRGBAReal	m_ClrAmbientBK, m_ClrDirectionalBK[dNTL_PL_DIRECTIONAL_LIGHT_NUM];

public:
	virtual RwBool	Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void	Destroy();

	virtual void	HandleEvents(RWS::CMsg &pMsg);

	virtual RwBool	SetProperty(const CNtlPLProperty *pData);

	virtual RwBool	Update(RwReal fElapsed);
	virtual RwBool	Render(void);

#ifdef dNTL_WORLD_TOOL_MODE
	virtual void	SetRpLightDirectinalColor(RwInt32 iIdx, RwRGBAReal rgbaf);
	virtual void	SetRpLightDirectinalAngle(RwInt32 iIdx, RwV3d& vAngle);
#endif

protected:
	RwBool			UpdateAmbient(RwReal fElapsed);
	RwBool			UpdateDirectional(RwReal fElapsed);
};

#endif //__NTL_PLLIGHT_H__
