/******************************************************************************
* File			: TutorialMessageGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 4. 23
* Abstract		: 
*****************************************************************************
* Desc			: 튜토리얼 진행중의 메세지를 표시한다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// dbo
#include "Windowby3.h"


class CTutorialMessageGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	struct sShowHide
	{
		RwReal				fRemainTime;
		RwReal				fElapsedTime;
		RwBool				bShow;
	};

	struct sChangeColor
	{
		RwReal				fRemainTime;
		RwReal				fElapsedTime;
		RwRGBA				srcColor;
		RwRGBA				destColor;
	};

	struct sDefaultMessageStyle
	{
		std::string			strFontName;
		RwRGBA				FontColor;
		RwRGBA				FontBackgroundColor;
		RwUInt8				byFontSize;
		RwUInt8				byAlign;
		RwInt32				iOffsetX;
		RwInt32				iOffsetY;
		RwInt32				iOffsetBackgroundX;
		RwInt32				iOffsetBackgroundY;
	};

	CTutorialMessageGui(const RwChar* pName);
	virtual ~CTutorialMessageGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CTutorialMessageGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnClick_SkipButton(gui::CComponent* pComponent);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);

protected:
	gui::CSlot			m_slotSkipButton;
	gui::CSlot			m_slotMove;

	gui::CFlash*		m_pflashBackground;
	gui::CHtmlBox*		m_pMessage;
	gui::CButton*		m_pSkipButton;

	sShowHide			m_ShowHideMessage;	
	sChangeColor		m_ChangeColor;
	sDefaultMessageStyle m_DefaultMessageStyle;
};