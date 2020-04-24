/******************************************************************************
* File			: NtlPLSceneChanger.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 9. 13
* Update		: 2007. 12. 13 ( Dbo 단에서 Pl 단으로 클래스 이동 )
* Abstract		: 
*****************************************************************************
* Desc			: 지정된 방법에 따라 장면 전환을 위해 GUI를 그린다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"


class CNtlPLSceneChangerGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eFadeInOut
	{
		EFADE_NONE,
		EFADE_IN,
		EFADE_OUT,
	};

	struct sInterpolationData
	{
		RwReal		fUpdateTime;
		RwReal		fCurTime;
		RwUInt8		byStartRed;
		RwUInt8		byStartGreen;
		RwUInt8		byStartBlue;
		RwUInt8		byStartAlpha;
		RwUInt8		byEndRed;
		RwUInt8		byEndGreen;
		RwUInt8		byEndBlue;
		RwUInt8		byEndAlpha;
	};

	CNtlPLSceneChangerGui(const RwChar* pName);
	virtual ~CNtlPLSceneChangerGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();	

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CNtlPLSceneChangerGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	VOID			SetAlpha(RwUInt8 byAlpha);

	VOID			OnPaint();
	VOID			OnPaintShutDownTV();
	VOID			OnPaintScreenPanel();

protected:	
	gui::CSlot			m_slotPaint;

	gui::CSurface		m_srfSurface;

	sInterpolationData	m_InterData;
	RwUInt8				m_byTransitionMode;
	eFadeInOut			m_eFadeInOut;
};