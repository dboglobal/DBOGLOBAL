#pragma once

#include "NtlPLEntity.h"

#include "NtlPLAttach.h"
#include "NtlPLObject.h"

#include "NtlPLEmblemMaker.h"


// 꼭 지켜야 하는 규칙.
// Update Return Value는 Controller 작업이 완료되면 FALSE, 작업 중이면 TRUE를 retrun 한다.
class CNtlPLObjectController
{
public:
	CNtlPLObjectController();
	virtual ~CNtlPLObjectController();

	virtual void	Init(CNtlPLObject* pNtlPLOjbect);
	virtual RwBool	Update(RwReal fElapsed);
	virtual void	Free();
	
protected:
	CNtlPLObject* m_pNtlPLObject;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlPLObjectControllerFade : public CNtlPLObjectController
{
public:
	CNtlPLObjectControllerFade();
	virtual ~CNtlPLObjectControllerFade();

	virtual RwBool	Update(RwReal fElapsed);
	virtual void	Free();

	void			SetParam(RwReal fTime, RwReal fColorS, RwReal fColorE, RwBool bAutoDelete);

protected:
	RwBool			m_bAutoDelete;

	RwReal			m_fTime;
	RwReal			m_fTimeCur;
	RwReal			m_fColorS;
	RwReal			m_fColorE;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define dPL_DOJO_RES_NAME_LEN	(128)
#define dPL_DOJO_MAX_LEVEL		(8)

struct SPLDojoCreateParam : public SPLEntityCreateParam
{
	RwInt32	iDojoID;

	RwReal	fFadeInTime;
	RwReal	fFadeOutTime;

	RwChar	acObjectName[dPL_DOJO_MAX_LEVEL][dPL_DOJO_RES_NAME_LEN];
	RwChar	acEffectNameUp[dPL_DOJO_MAX_LEVEL][dPL_DOJO_RES_NAME_LEN];
	RwChar	acEffectNameDown[dPL_DOJO_MAX_LEVEL][dPL_DOJO_RES_NAME_LEN];
};

class CNtlPLDojo : public CNtlPLAttach
{
public:
	CNtlPLDojo();
	virtual ~CNtlPLDojo();

	virtual RwBool				Create(const SPLEntityCreateParam * pParam = NULL);
	virtual void				Destroy();

	virtual RwBool				SetProperty(const CNtlPLProperty *pData);

	virtual RwBool				Update(RwReal fElapsed);
	virtual RwBool				Render();

	virtual void				SetPosition(const RwV3d *pPos)			{ m_vPos = *pPos; UpdateTransform(); }
	virtual RwV3d				GetPosition(void)						{ return m_vPos; }

	virtual void				SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle) { m_vRot.x = fXAngle; m_vRot.y = fYAngle; m_vRot.z = fZAngle; UpdateTransform(); }
	virtual const RwV3d*		GetRotate(void) const					{ return &m_vRot;} 

	virtual void				SetScale(RwReal fScale)					{ m_vScale.x = fScale; m_vScale.y = fScale; m_vScale.z = fScale; UpdateTransform(); }
	virtual RwReal				GetScale(void)							{ return m_vScale.x; }


	virtual void				SetScaleVector(const RwV3d *pScale)		{ m_vScale = *pScale; UpdateTransform(); }
	virtual const RwV3d*		GetScaleVector(void) const				{ return &m_vScale; }

	virtual RwMatrix&			GetMatrix(void)							{ return m_matTrasform; }

public:
	virtual void				UpdateTransform();

	virtual RwBool				RefreshEmblem();

	virtual void				RefreshDojo();

	virtual void				RefreshResource(RwReal fFadeInTime = 0.0f, RwReal fFadeOutTime = 0.0f);

	virtual CNtlPLObject*		GetDojoObject();

	virtual void				SetLevel(RwInt32 iLevel);
	virtual RwInt32				GetLevel()								{ return m_iDojoLevel; }

	virtual	void				SetDojoID(RwInt32 iDojoID)				{ m_iDojoID = iDojoID; }
	virtual	RwInt32				GetDojoID()								{ return m_iDojoID; }

	virtual void				SetFadeInTime(RwReal fFadeTime)			{ m_fFadeInTime = fFadeTime; }
	virtual RwReal				GetFadeInTime()							{ return m_fFadeInTime; }

	virtual void				SetFadeOutTime(RwReal fFadeTime)		{ m_fFadeOutTime = fFadeTime; }
	virtual RwReal				GetFadeOutTime()						{ return m_fFadeOutTime; }

	virtual RwChar*				GetResObjectName(RwInt32 iLevel)		{ return m_acObjectName[iLevel]; }
	virtual RwChar*				GetResEffectNameUp(RwInt32 iLevel)		{ return m_acEffectNameUp[iLevel]; }
	virtual RwChar*				GetResEffectNameDown(RwInt32 iLevel)	{ return m_acEffectNameDown[iLevel]; }

	virtual void				SetResObjectName(RwInt32 iLevel, RwChar* pName)		{ sprintf_s(m_acObjectName[iLevel], dPL_DOJO_RES_NAME_LEN, pName); }
	virtual void				SetResEffectNameUp(RwInt32 iLevel, RwChar* pName)		{ sprintf_s(m_acEffectNameUp[iLevel], dPL_DOJO_RES_NAME_LEN, pName); }
	virtual void				SetResEffectNameDown(RwInt32 iLevel, RwChar* pName)	{ sprintf_s(m_acEffectNameDown[iLevel], dPL_DOJO_RES_NAME_LEN, pName); }
	

protected:
	RwInt32						m_iDojoID;
	RwInt32						m_iDojoLevel;
	
	RwReal						m_fFadeInTime;
	RwReal						m_fFadeOutTime;

	RwChar						m_acObjectName[dPL_DOJO_MAX_LEVEL][dPL_DOJO_RES_NAME_LEN];
	RwChar						m_acEffectNameUp[dPL_DOJO_MAX_LEVEL][dPL_DOJO_RES_NAME_LEN];
	RwChar						m_acEffectNameDown[dPL_DOJO_MAX_LEVEL][dPL_DOJO_RES_NAME_LEN];	

	CNtlPLObject*				m_pDojoObject;

	CNtlPLObjectControllerFade	m_FadeControllerUp;
	CNtlPLObjectControllerFade	m_FadeControllerDown;

	RwMatrix					m_matTrasform;

	RwV3d						m_vPos;
	RwV3d						m_vScale;
	RwV3d						m_vRot;

	RwBool						m_bEmblemNeedRefresh;

	sEmblemFactor				m_sEmblem;
	sEmblemFactor				m_sEmblemNew;
};