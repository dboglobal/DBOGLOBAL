/******************************************************************************
* File			: SideDialogManager.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 11. 6
* Abstract		: 
*****************************************************************************
* Desc			: 클라이언트의 오른쪽 중앙에 나타나는 다이얼로그를 관리한다
*				  높이가 겹치는 경우 Open/close 이펙트가 없이 화면에 표시중이면
*				  겹치는 모든 기존의 SideDialog를 닫는다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"
#include "NtlCallbackWidget.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "DialogPriority.h"
#include "SideDialogDefine.h"

class CSideDialogManager : public CNtlPLGui, public RWS::CEventHandler
{
public:
	static CSideDialogManager*	GetInstance();

	enum eOpenType
	{
		OPENTYPE_OPEN,							///< 현재 화면에 보여지고 있는 상태
		OPENTYPE_MINIMAM,						///< 현재 화면에 보여지고 있으나 최소화된 상태
		OPENTYPE_CONCEAL,						///< 다른 창에 가려져 있는 상태
	};

	struct DialogInfo
	{
		CNtlPLGui*				pDialog;		///< 다이얼로그로의 포인터
		CNtlCallbackParam1bool* pCallSwitch;	///< 다이얼로그 열고 닫는 함수로의 콜백

		DialogInfo()
		{
			pDialog = NULL;
			pCallSwitch = NULL;
		}
	};

	struct sPresentationDialog
	{
		eSideDialogType			eSDialog;
		DialogInfo*				pDialogInfo;	
		eOpenType				openType;
	};

	typedef std::map<int, DialogInfo>							MAP_SDIALOG;
	typedef std::map<int, DialogInfo>::iterator					MAP_SDIALOG_ITER;

	typedef std::map<RwInt32, sPresentationDialog>				MAP_PRESENTATION;
	typedef std::map<RwInt32, sPresentationDialog>::iterator	MAP_PRESENTATION_ITER;

	typedef std::map<RwInt32, RwUInt32>							MAP_ATTRIBUTE;
	typedef std::map<RwInt32, RwUInt32>::iterator				MAP_ATTRIBUTE_ITER;


	CSideDialogManager(const RwChar* pName);
	virtual ~CSideDialogManager();

	static VOID CreateInstance();
	static VOID	DestoryInstance();

	template <class Callbackclass>
	VOID		RegistDialog(int iSideDialog, CNtlPLGui* pDialog, int (Callbackclass::*callback)(bool bOpen));
	VOID		UnRegistDialog(int iSideDialog);

	VOID		Update(RwReal fElapsed);
	RwInt32		SwitchDialog(bool bOpen);
	RwBool		SwitchDialog(int iDialog);			///< 사이드 다이얼로그를 열거나 닫는다

	VOID		OpenDefaultDialog();
	RwBool		OpenDialog(RwInt32 iSideDialog, RwBool bPlaySound = TRUE);

	RwBool		CloseDialog(RwInt32 iSideDialog, RwBool bPlaySound = TRUE);
	VOID		CloseAll();

	DialogInfo*	GetDialogInfo(int iSideDialog);
	CNtlPLGui*	GetDialog(RwInt32 iSideDialog);

	RwBool		IsOpenDialog(int iSideDialog);
	RwBool		IsConcealDialg(int iSideDialog);
	RwBool		IsDefaultDialog(int iSideDialog);
	RwBool		IsEventDialog(int iSideDialog);
	RwBool		IsNormalDialog(int iSideDialog);
	//// Test Func		/// woosungs_test 20090723
	CNtlPLGui* const GetpDialogTEST(const char* szFrmFileName);
	RwBool		ShowDialogTEST(RwInt32 iDialog, bool bOpen = true);
	VOID		ShowAllDialogTEST(bool bOpen = true);
protected:
	CSideDialogManager() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	RwBool		IsAttribute(RwInt32 iSideDialog, RwUInt32 uiFlag);

	VOID		PlayOpenSound(RwInt32 iSideDialog);
	VOID		PlayCloseSound(RwInt32 iSideDialog);

	VOID		RegisterAttribute();

	VOID		OnMove(RwInt32 iOldX, RwInt32 iOldY);

protected:
	static CSideDialogManager* m_pInstance;

	gui::CSlot				m_slotMove;

	MAP_SDIALOG				m_mapDialog;					///< 등록된 다이얼로그들의 포인터 모음
	MAP_PRESENTATION		m_mapPresentDialog;				///< 현재 보여지고 있는 다이얼로그의 정보

	MAP_ATTRIBUTE			m_mapAttribute;
};

static CSideDialogManager* GetSideDialogManager()
{
	return CSideDialogManager::GetInstance();
}

template <class Callbackclass>
VOID CSideDialogManager::RegistDialog(int iSideDialog, CNtlPLGui* pDialog, int (Callbackclass::*callback)(bool bOpen))
{
	DialogInfo info;

	info.pDialog = pDialog;
	info.pCallSwitch = NTL_NEW CNtlCallbackWidget1bool<Callbackclass>(reinterpret_cast<Callbackclass*>(pDialog), callback);;

	m_mapDialog[iSideDialog] = info;

	info.pDialog->GetDialog()->SetPriority(dDIALOGPRIORITY_SIDE_DIALOG);
}