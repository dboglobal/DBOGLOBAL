#pragma once

#include "NtlDebug.h"
#include "NtlPLEntity.h"
#include "NtlWorldRpLightManager.h"

#include "ceventhandler.h"

struct SPLLightIndoorCreateParam : public SPLEntityCreateParam
{
	RpLightType		rpLightType;
	RwRGBAReal		rgbafColor;
	RwV3d			vRotate;
	RwReal			fRadius;
	RwReal			fAngle;
	RwBool			bRealLight;
};

class CNtlPLLightIndoor : public CNtlPLEntity, public CNtlWorldRpLightProxy
{
	static RwReal s_fPLLightIndoorIpTimeIn;
	static RwReal s_fPLLightIndoorIpTimeOut;

public:
	CNtlPLLightIndoor();
	virtual ~CNtlPLLightIndoor();

	virtual RwBool	SetProperty(const CNtlPLProperty *pData);

	virtual RwBool	Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void	Destroy();
	virtual RwBool	Update(RwReal fElapsed);
	virtual RwBool	Render();

	virtual void	OnUpdate(RpLight* pLight, RwReal fElapsed);
	virtual void	OnRender(RpLight* pLight);
	virtual void	OnRequestRemoveRpLight(RpLight* pRpLight);

	virtual void	OnAddRpLight(RpLight* pRpLight);
	virtual void	OnRemoveRpLight(RpLight* pRpLight);

public:
	virtual RwV3d	GetPosition()	{ return m_vPosition; }
	virtual RwV3d*	GetRotate()		{ return &m_vRotate; }

	virtual void	SetPosition(const RwV3d *pPos);
	virtual void	SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle);
	
	RwRGBAReal		GetColor()		{ return m_rgbafColor; }
	RwReal			GetAngle()		{ return m_fAngle; }
	RwReal			GetRadius()		{ return m_fRadius[2]; }
	RpLightType		GetLightType()	{ return m_rpLightType; }
	RwBool			IsRealLight()	{ return m_bRealLight; }
	
	VOID			SetRealLight(RwBool bBool);
	VOID			SetRadius(RwReal fRadius);
	VOID			SetAngle(RwReal fAngle);
	VOID			SetColor(RwRGBAReal& rgbafColor);

#ifdef dNTL_WORLD_TOOL_MODE
	static CNtlPLLightIndoor*	s_pPLLightIndoorSelected;
	static RwBool				s_bVisibleLine;
	static RwBool				s_bVisiblePlane;
	
	static void		SetToolUI(CNtlPLLightIndoor* pPLLightIndoor);
	static void		SetVisibleUI(RwBool bLine, RwBool bPlane);
	static void		GetVisibleUI(RwBool& bLine, RwBool& bPlane);
	static void		SaveSwapFile(RwReal x, RwReal y, RwReal z);
	static void		LoadSwapFile(RwReal x, RwReal y, RwReal z);
	RwBool			Pick(RwReal& fDist);
#endif

protected:
	virtual void	SetTransform();

protected:
	RpLightType		m_rpLightType;
	RwBool			m_bRealLight;

	RpLight*		m_pLight;
	RwRGBAReal		m_rgbafColor;
	RwV3d			m_vRotate;
	RwV3d			m_vPosition;
	RwReal			m_fRadius[3]; // cur / start / original
	RwReal			m_fAngle;
	RwReal			m_fInterpolationTime;

	RwInt32			m_eFadeType;

#ifdef dNTL_WORLD_TOOL_MODE
	RpAtomic*		m_pAtomicCtlPt;
	RwReal			m_fRealLightColorTime;
	RwBool			m_bRealLightColor;
#endif
};