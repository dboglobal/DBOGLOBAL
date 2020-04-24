/******************************************************************************
* File			: DojoUpgradeGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 4. 6
* Abstract		: 
*****************************************************************************
* Desc			: 도장 업그레이드 GUI
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// client
#include "SurfaceGui.h"
#include "Windowby3.h"
#include "SlotGui.h"

struct sDOJO_INFO;
struct sDBO_DOJO_NPC_INFO;


class CDojoUpradeGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eDOJO_VISUAL_LEVEL
	{
		DOJO_VISUAL_LEVEL_1,
		DOJO_VISUAL_LEVEL_2,
		DOJO_VISUAL_LEVEL_3,
		DOJO_VISUAL_LEVEL_4,

		NUM_DOJO_VISUAL_LEVEL
	};

	enum eINFO_WINDOW_TYPE
	{
		INFO_WINDOW_TYPE_SLOT,
		INFO_WINDOW_TYPE_LEFT_DOJO_VISUAL,
		INFO_WINDOW_TYPE_RIGHT_DOJO_VISUAL,

		NUM_INFO_WINDOW_TYPE,

		INVALID_INFO_WINDOW_TYPE
	};

public:
	CDojoUpradeGui(const RwChar* pName);
	virtual ~CDojoUpradeGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CDojoUpradeGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			Clear();
	RwBool			SetHoipoiRock(SERIAL_HANDLE hItem, RwUInt32 uiCount);
	VOID			UnsetHoipoiRock();

	VOID			SetDojoName(WCHAR* pwcGuildName);

	RwUInt32		GetNeedGuildPoint(sDOJO_INFO* pDOJO_INFO);
	RwUInt32		GetNeedZennyPoint(sDOJO_INFO* pDOJO_INFO);

	VOID			SetNeedGuildPoint(sDOJO_INFO* pDOJO_INFO);
	VOID			SetNeedZennyPoint(sDOJO_INFO* pDOJO_INFO);
	VOID			SetConsumptionGuildPoint(sDOJO_INFO* pDOJO_INFO);	
	VOID			SetConsumptionZennyPoint(sDOJO_INFO* pDOJO_INFO);
	VOID			SetNeedItem(sDOJO_INFO* pDOJO_INFO);
	VOID			SetDojoLevel(sDOJO_INFO* pDOJO_INFO);

	RwUInt8			ConvertDojoLeveltoGuildFunction(RwUInt8 byDojoLevel);

	VOID			CheckInfoWindow();
	RwInt8			PtinSlot(RwInt32 iX, RwInt32 iY);

	VOID			OnClicked_HelpButton( gui::CComponent* pComponent );
	VOID			OnClicked_ExitButton( gui::CComponent* pComponent );
	VOID			OnClicked_UpgradeButton(gui::CComponent* pComponent);

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();
	VOID			OnCaptureMouseDown(const CKey& key);

protected:
	gui::CSlot			m_slotHelpButton;
	gui::CSlot			m_slotExitButton;
	gui::CSlot			m_slotUpgradeButton;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCaptureMouseDown;

	gui::CButton*		m_pBtnHelp;
	gui::CButton*		m_pBtnExit;
	gui::CButton*		m_pBtnUpgrade;

	gui::CStaticBox*	m_pDialogName;
	gui::CStaticBox*	m_pDojoNameStatic;					///< '도장 이름'
	gui::CStaticBox*	m_pDojoName;
	gui::CStaticBox*	m_pDojoCurLevel;
	gui::CStaticBox*	m_pDojoNextLevel;
	gui::CStaticBox*	m_pNeedGuildPointStatic;			///< '필요한 유파 포인트'
	gui::CStaticBox*	m_pNeedGuildPoint;
	gui::CStaticBox*	m_pConsumptionGuildPointStatic;		///< '소모 유파 포인트'
	gui::CStaticBox*	m_pConsumptionGuildPoint;
	gui::CStaticBox*	m_pNeedZennyStatic;					///< '필요한 제니'
	gui::CStaticBox*	m_pNeedZenny;
	gui::CStaticBox*	m_pConsumptionZennyStatic;			///< '소모 제니'
	gui::CStaticBox*	m_pConsumptionZenny;
	gui::CStaticBox*	m_pNeedItemStatic;					///< '필요한 아이템'
	gui::CStaticBox*	m_pNeedItem;

	CSurfaceGui			m_srfDojo[NUM_DOJO_VISUAL_LEVEL];
	CSurfaceGui			m_srfNextArrowBar;
	CSurfaceGui			m_srfNeedGuildPoint;
	CSurfaceGui			m_srfMyGuildPoint;
	CSurfaceGui			m_srfNeedZenny;
	CSurfaceGui			m_srfCumptionZenny;
	CSurfaceGui			m_srfSlot;
	CSurfaceGui			m_srfSlotFocus;

	CWindowby3			m_DojoNameBackground;
	CWindowby3			m_NeedGuildPointBackground;
	CWindowby3			m_NeedZennyBackground;
	CWindowby3			m_NeedItemBackground;

	CRegularSlotGui			m_StuffSlot;

	SERIAL_HANDLE		m_hNPCSerial;

	RwInt8				m_byLeftMouseDown;
	RwInt8				m_byRightMouseDown;

	RwBool				m_bSlotFocus;
	RwBool				m_bSlotDest;

	RwBool				m_bExpectServerAnswer;

	RwUInt32			m_uiDojoTableIndex;
	eINFO_WINDOW_TYPE	m_eInfoWindowType;

	eDOJO_VISUAL_LEVEL	m_eDojoVisualLevel;	
};