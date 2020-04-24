#pragma once

#include "ceventhandler.h"

#include "NtlDebug.h"

#include "NtlWorldRpLightManager.h"

#include "NtlPLAttach.h"
#include "NtlPLDecal.h"

struct SPLLightObjectCreateParam : public SPLEntityCreateParam
{
	RpLightType		rpLightType;
	RwRGBAReal		rgbafColor;
	RwV3d			vRotate;
	RwReal			fRadius;
	RwReal			fAngle;
	RwBool			bRealLight;

	RwBool			bDecal;			// Decal을 출력 할지
	RwBool			bWorldLight;	// WorldLightColor 연산에 포함 시킬지
};

class CNtlPLLightObject : public CNtlPLAttach, public CNtlWorldRpLightProxy
{
	static RwReal s_fPLLightObjectIpTimeIn;
	static RwReal s_fPLLightObjectIpTimeOut;

public:
	CNtlPLLightObject();
	virtual ~CNtlPLLightObject();

	virtual RwBool				SetProperty(const CNtlPLProperty *pData);

	virtual RwBool				Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void				Destroy();
	virtual RwBool				Update(RwReal fElapsed);
	virtual RwBool				Render();

	virtual RwV3d				GetPosition()	{ return m_vPosition; }
	virtual RwV3d*				GetRotate()		{ return &m_vRotate; }

	virtual void				SetPosition(const RwV3d *pPos);
	virtual void				SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle);

	virtual void				OnUpdate(RpLight* pLight, RwReal fElapsed);
	virtual void				OnRender(RpLight* pLight);
	virtual void				OnRequestRemoveRpLight(RpLight* pRpLight);

	virtual void				OnAddRpLight(RpLight* pRpLight);
	virtual void				OnRemoveRpLight(RpLight* pRpLight);

	virtual void				SetMatrix(RwMatrix& matWorld);
	virtual RwMatrix&			GetMatrix(void);

public:
	RwRGBAReal					GetColor()		{ return m_rgbafColor; }
	RwReal						GetAngle()		{ return m_fAngle; }
	RwReal						GetRadius()		{ return m_fRadius[2]; }
	RpLightType					GetLightType()	{ return m_rpLightType; }
	RwBool						IsRealLight()	{ return m_bRealLight; }
	
	VOID						SetRealLight(RwBool bBool);
	VOID						SetRadius(RwReal fRadius);
	VOID						SetAngle(RwReal fAngle);
	VOID						SetColor(RwRGBAReal& rgbafColor);

	void						SetEnableWorldLight(RwBool bWorldLight) { if (m_bRealLight) { m_bWorldLight = bWorldLight; } }

	void						CreateDecal();
	void						DeleteDecal();
	void						UpdateDecal();
	void						UpdateDecalPoint();
	void						UpdateDecalSpot();
	void						UpdateDecalSoftSpot();

	RwBool						IsEnabledDecal();
	RwBool						IsEnabledWorldLight();

	virtual RwBool				GetWorldLightColorf(const RwV3d* pvPos, RwRGBAReal* pclrfOut, void* pUserData);

#ifdef dNTL_WORLD_TOOL_MODE
	static CNtlPLLightObject*	s_pPLLightObjectSelected;
	static RwBool				s_bVisibleLine;
	static RwBool				s_bVisiblePlane;
	static RwReal				s_fDummyScale;
	
	static void					SetToolUI(CNtlPLLightObject* pPLLightObject);
	static void					SetVisibleUI(RwBool bLine, RwBool bPlane);
	static void					GetVisibleUI(RwBool& bLine, RwBool& bPlane);
	static void					SetDummyScale(RwReal fScale);
	static RwReal				GetDummyScale();
	static void					SaveSwapFile(RwReal x, RwReal y, RwReal z);
	static void					LoadSwapFile(RwReal x, RwReal y, RwReal z);
	RwBool						Pick(RwReal& fDist);
#endif

protected:
	virtual void				SetTransform();

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

	RwMatrix		m_matParant;

	// Decal
	CNtlPLDecal*	m_pNtlPLDecal;
	RwBool			m_bWorldLight;

#ifdef dNTL_WORLD_TOOL_MODE
	RpAtomic*		m_pAtomicCtlPt;
	RwReal			m_fRealLightColorTime;
	RwBool			m_bRealLightColor;
#endif
};