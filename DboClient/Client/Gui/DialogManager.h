/******************************************************************************
* File			: DialogManager.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 8. 11
* Abstract		: 
*****************************************************************************
* Desc			: Dialog Manager
*****************************************************************************/

#pragma once

#include <map>
#include <set>

// core
#include "NtlDebug.h"
#include "ceventhandler.h"
#include "NtlCallbackWidget.h"

// gui
#include "gui_guimanager.h"

// simulation
#include "NtlSLDef.h"

// Dbo
#include "DialogDefine.h"
#include "DialogPriority.h"

class CNtlPLGui;
class CNtlSob;
class CNtlSobItem;

#define dREGULARDIALOG_FIRST_X		37
#define dREGULARDIALOG_GAP			10
#define dCHARM_Y					450
#define dNEED_SIZE_MORE_REGULAR		379	///< Regular Dialog를 하나더 열 수 있기 위해 필요한 여유폭
#define dDAFAULT_REGULAR_COUNT		2	///< 기본적으로 열 수 있는 레귤러 다이얼로그의 갯수



class CDialogManager : public RWS::CEventHandler
{
public:
	static CDialogManager*	GetInstance();

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

	struct sOpenTarget
	{
		CNtlSob*				pOpenedTarget;	///< NPC Pointer to the target that opened the window
		RwV3d					v3Pos;
	};

	struct sRequestedOpenDialog
	{
		RwInt32					eRequestedDialogType;
		SERIAL_HANDLE			hSerial;
		RwBool					bSound;
	};

	typedef std::list<eDialogType>						LIST_OPENED_REGULAR_DIALOG;
	typedef std::list<eDialogType>::iterator			LIST_OPENED_REGULAR_ITER;

	typedef std::map<int, DialogInfo>					DIALOG_MAP;
	typedef std::map<int, DialogInfo>::iterator			DIALOG_MAP_ITER;
	typedef std::map<int, DialogInfo>::value_type		DIALOG_MAP_VALUE;

	typedef std::map<eDialogType, RwUInt32>				REGULAR_MAP;
	typedef std::map<eDialogType, RwUInt32>::iterator	REGULAR_MAP_ITER;

	typedef std::list<sRequestedOpenDialog>				LIST_REQUESTED_OPENDIALOG_IN_NARRATION;
	typedef std::list<sRequestedOpenDialog>::iterator	LIST_REQUESTED_OPENDIALOG_IN_NARRATION_ITER;

	// Entire Focusing Dialog
	typedef REGULAR_MAP									FE_MAP;
	typedef REGULAR_MAP_ITER							FE_MAP_ITER;

	typedef std::list<int>								DIALOG_RAISE;

	static CDialogManager* m_pInstance;

	virtual ~CDialogManager();
	
	static VOID	CreateInstance(gui::CGUIManager* pGuiManager);								///< 매니저 초기화	
	static VOID	DestoryInstance();

	VOID		Update(RwReal fElapsed);

	template <class Callbackclass>
	VOID		RegistDialog(int iDialog, CNtlPLGui* pDialog, int (Callbackclass::*callback)(bool bOpen));	///< 다이얼로그를 등록한다.	
	VOID		UnRegistDialog(int iDialog);		///< 해당 다이얼로그를 해제한다.	

	VOID		SwitchBag(RwBool bOpen);			///< 가방 보여주기 On/Off
	VOID		SwitchBagByIndex(RwInt32 nIndex);
	RwBool		SwitchDialog(int iDialog);			///< 다이얼로그를 열거나 닫는다.	

	VOID		OpenGMDialog();						///< GM 다이얼로그를 전부 연다
	VOID		OpenDefaultDialog();				///< 기본 다이얼로그를 연다
	RwBool		OpenDialog(int iDialog, SERIAL_HANDLE hSerial = INVALID_SERIAL_ID, RwBool bPlaySound = TRUE); ///< 다이얼로그를 연다, uiNPCSerial : NPC 다이얼로그일 사용

	RwBool		CloseDialog(int iDialog, RwBool bPlaySound = TRUE);				///< 다이얼로그를 닫는다.
	VOID		CloseGMDialog();					///< GM 다이얼로그를 전부 닫는다
	VOID		CloseNotDefaultDialog();			///< 기본 다이얼로그를 제외한 모든 다이얼로그를 닫는다.
	VOID		CloseAll();							///< 모든 다이얼로그를 닫는다.


	VOID		RaiseDialogbyUser(RwInt32 iDialog);	///< 유저에 의해 다이얼로그가 화면에 나타나는 순간 가장 위에 그려지고 있음을 알린다.
	VOID		HideDialogbyUser(RwInt32 iDialog);	///< 유저에 의해 다이얼로그가 화면에서 사라지는 순간 화면에 그려지지 않음을 알린다.
		
	RwInt32		GetOpendNPCDIalog();				///< 열려있는 NPC 다이얼로그를 검사한다.	
	CNtlSob*	GetOpenedTarget();

	DialogInfo*	GetDialogInfo(int iDialog);			///< 해당 다이얼로그의 정보를 반환한다.
	CNtlPLGui*	GetDialog(int iDialog);			///< 해당 다이얼로그의 포인터를 반환한다.
	RwInt32		GetTopDialog();						///< 가장 위에서 그려지고 있는 다이얼로그의 인덱스를 반환한다.

	int			HandleDialogAction(unsigned int iAction); ///< 액션을 처리한다. by Kell
	VOID		HandleMouse(const CKey& key);		///< 마우스를 처리한다

	VOID		OnMode(eDialogMode eMode);			///< 모드 설정
	VOID		OffMode();							///< 모드를 해제한다
	eDialogMode	GetMode();
	RwInt32		HowManyOpenMaxRegular();			///< 현재 클라이언트의 넓이에서 열 수 있는 최대 레귤러 다이얼로그의 갯수를 반환한다
	RwInt32		GetPosY_of_HalfRegular();			///< 길이가 절반정도인 레귤러 다이얼로그의 Y 좌표를 반환한다
	VOID		Disable_OnObserver(RwBool bVisible);	///< Observer 모드에서 일부 다이얼로그를 보여줄지 여부를 정한다

	VOID		LocateBag(RwInt32 iWidth, RwInt32 iHeight, RwBool bForce = FALSE);///< 가방을 알맞은 위치로 정렬한다
	VOID		LocationDialogs(RwInt32 iWidth, RwInt32 iHeight);
	VOID		Locate_MovableDefaultPosition();	///< 다이얼로그가 움직일 수 있을 때의 기본 위치를 지정

	RwBool		IsExistDialog(RwInt32 iDialog);		///< 다이얼로그가 존재하는지 여부
	RwBool		CanMoveItem_in_Capsule();			///< 캡슐안의 아이콘을 움직일 수 있는지 판단
	RwBool		CanPickupItem_in_Capsule();			///< 캡슐안의 아이콘을 집을 수 있는지 판단
	RwBool		IsOpenDialog(int iDialog);			///< 다이얼로그가 열려있는지 검사한다.
	RwBool		IsBagOpen();						///< t : 가방이 하나 이상 열려있다.
	RwBool		IsOpenNPCDialog();					///< NPC 관련 다이얼로그가 열려있는지 검사한다.
	RwBool		IsNPCDialog(RwInt32 iDialog);		///< NPC 관련 다이얼로그인지 검사한다.
	RwBool		IsTriggerSystemDialogOpen();		///< Quest, Trigger관련 창이 열려있는지 검사한다. by peessi
	RwBool		IsMode(eDialogMode eMode);			///< 다이얼로그 관련 모드를 알아본다
	RwBool		IsRegularDialog(eDialogType eType);
	RwBool		IsEFDialog(eDialogType eType);
	RwBool		IsOpenEFDialog();
	RwBool		IsHaveAttribute(eDialogType eType, RwUInt32 uiAttr);
	RwBool		CanOpenRegularDialog();
	RwBool		IsDialogMovable();
	//// Test Func				/// woosungs_test 20090723
	CNtlPLGui* const GetpDialogTEST(const char* szFrmFileName);
	RwBool		ShowDialogTEST(RwInt32 iDialog, bool bOpen = true);
	VOID		ShowAllDialogTEST(bool bOpen = true);

	void			SetClickedItem(CNtlSobItem* pItem) { m_pClickedItem = pItem; }
	CNtlSobItem*	GetClickedItem() { return m_pClickedItem; }

protected:
	CDialogManager();
	CDialogManager(const CDialogManager& dialog) {};

	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID		RegisteRegularDialog();
	VOID		RegisteEntireFocusingDialog();
	
	VOID		ProcessESC();						///< ESC키를 눌렀을 때 처리	
	VOID		ProcessOpenRegular(eDialogType eDialog);
	VOID		ProcessLayer(eDialogType eDialog);		///< 다이얼로그의 레이어를 정리한다
	VOID		ProcessCloseRegular(eDialogType eDialog);
	VOID		ProcessMovableDialogs(RwBool bMovable);

	VOID		Locate_AutoPosition(RwInt32 iWidth, RwInt32 iHeight);
	VOID		Locate_UserPosition(RwInt32 iWidth, RwInt32 iHeight);
	VOID		Locate_UserDefaultPosition(RwInt32 iWidth, RwInt32 iHeight);	

	VOID		PlayOpenSound(RwInt32 iDialog);		///< 유저가 다이얼로그를 열 때의 효과음
	VOID		PlayCloseSound(RwInt32 iDialog);	///< 유저가 다이얼로그를 닫을 때의 효과음

protected:
	gui::CSlot		m_slotMouse;

	DIALOG_MAP		m_mapDialog;					///< 등록된 다이얼로그들의 포인터 모음
	sOpenTarget		m_OpenedTarget;
	
	eDialogMode		m_eMode;						///< 모드
	RwInt32			m_iRegularStrartY;
	RwInt32			m_iOldWidth;
	RwInt32			m_iOldHeight;

	RwBool			m_bExpectingCloseNPCDialog;
	RwBool			m_bESCSkip;
	RwBool			m_bDialogMovable;
	RwBool			m_bCanVisible_OnOberserver;

	LIST_OPENED_REGULAR_DIALOG m_listOpenedRegularDilaog;
	LIST_REQUESTED_OPENDIALOG_IN_NARRATION m_listRequestedOpenDialogInNarration;	
	
	DIALOG_RAISE	m_listRaiseDialogbyUser;		///< 유저의 조작에 의해 화면에 나타나는 GUI의 모음
													///< 가장 나중에 그리는 다이얼로그 순으로 오름차순 정렬

	REGULAR_MAP		m_mapRegular;					///< 화면의 지정된 위치에 열리는 다이얼로그의 모음
	FE_MAP			m_mapEntireFocuing;				///< 화면의 전체 영역을 이용하며 유저의 시점을 집중시키기 위한 다이얼로그

	// If we clicked an item (like dogi ball)
	CNtlSobItem*	m_pClickedItem;
};

static CDialogManager* GetDialogManager(VOID)
{
	return CDialogManager::GetInstance();
}

template <class Callbackclass>
VOID CDialogManager::RegistDialog(int iDialog, CNtlPLGui* pDialog, int (Callbackclass::*callback)(bool bOpen))
{
	DialogInfo* pDialogInfo = GetDialogInfo(iDialog);
	if (pDialogInfo)
	{
		return;
	}

	DialogInfo info;

	pDialog->SetMovable(m_bDialogMovable);

	info.pDialog = pDialog;
	info.pCallSwitch = NTL_NEW CNtlCallbackWidget1bool<Callbackclass>(reinterpret_cast<Callbackclass*>(pDialog), callback);

	m_mapDialog[iDialog] = info;

	// EFDialog 우선순위 결정
	if( IsEFDialog((eDialogType)iDialog) )
		info.pDialog->GetDialog()->SetPriority(dDIALOGPRIORITY_EFDIALOG);

	// Regular Dialog, 가방, 사이드 다이얼로그는 dDIALOGPRIORITY_EFDIALOG 를 쓴다
}