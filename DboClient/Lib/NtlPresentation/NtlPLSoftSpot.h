#ifndef __NTL_PLSPOT_H__
#define __NTL_PLSPOT_H__
// CZ-SS
// #include <RwCore.h>
// 
// #include "NtlDebug.h"
// 
// #include "NtlPLAttach.h"
// #include "NtlPLApi.h"
// 
// #include "NtlWorldCommon.h"
// #include "NtlWorldRpLightManager.h"
// 
// #include "ceventhandler.h"
// 
// #define dCONE_NUM_VERTICES	(10)
// #define dCONE_RADIUS		(30.0f)
// #define dCONE_ANGLE			(30.0f)
// #define dDUMMY_HEIGHT		(20.0f)
// #define dSPOT_POPUP_TIME	(3.0f)
// 
// class CNtlPLSpot : public CNtlPLEntity, public CNtlWorldRpLightProxy
// {
// public:
// 	CNtlPLSpot(void);
// 	virtual	~CNtlPLSpot(void);
// 
// private:
// 	static RwReal	s_fPLSpotIpTimeIn;
// 	static RwReal	s_fPLSpotIpTimeOut;
// 
// 	RpLight*		m_pLight;
// 	RwRGBAReal		m_rgbafColor;
// 	RwV3d			m_vRotate;
// 	RwV3d			m_vPosition;
// 	RwReal			m_fRadius[3]; // cur / start / original
// 	RwReal			m_fAngle;
// 	RwReal			m_fInterpolationTime;
// 
// 	RwInt32			m_eFadeType;
// 
// #ifdef dNTL_WORLD_TOOL_MODE
// 
// 	RwIm3DVertex	m_Cone[dCONE_NUM_VERTICES + 1];
// 	RwImVertexIndex m_Indices[dCONE_NUM_VERTICES * 3];
// 
// #endif
// 
// public:
// 	virtual RwBool	SetProperty(const CNtlPLProperty *pData);
// 
// 	virtual RwBool	Create(const SPLEntityCreateParam* pParam = NULL);
// 	virtual void	Destroy();
// 	virtual RwBool	Update(RwReal fElapsed);
// 	virtual RwBool	Render();
// 
// 	virtual void	OnUpdate(RpLight* pLight, RwReal fElapsed);
// 	virtual void	OnRender(RpLight* pLight);
// 	virtual void	OnRequestRemoveRpLight(RpLight* pRpLight);
// 
// 	virtual void	OnAddRpLight(RpLight* pRpLight);
// 	virtual void	OnRemoveRpLight(RpLight* pRpLight);
// 
// public:
// 	virtual RwV3d	GetPosition()	{ return m_vPosition; }
// 	virtual RwV3d*	GetRotate()		{ return &m_vRotate; }
// 
// 	virtual void	SetPosition(const RwV3d *pPos);
// 	virtual void	SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle);
// 
// 	RwRGBAReal		GetColor()		{ return m_rgbafColor; }
// 	RwReal			GetAngle()		{ return m_fAngle; }
// 	RwReal			GetRadius()		{ return m_fRadius[2]; }
// 
// 	VOID			SetRealLight(RwBool bBool);
// 	VOID			SetRadius(RwReal fRadius);
// 	VOID			SetAngle(RwReal fAngle);
// 	VOID			SetColor(RwRGBAReal& rgbafColor);
// 
// #ifdef dNTL_WORLD_TOOL_MODE
// 	static CNtlPLSpot*	s_pPLSpotSelected;
// 
// 	static void		SetToolUI(CNtlPLSpot* pPLSpot);
// 	RwBool			Pick(RwReal& fDist);
// #endif
// 
// protected:
// 	virtual void	SetTransform();
// };

#endif //__NTL_PLSPOT_H__
