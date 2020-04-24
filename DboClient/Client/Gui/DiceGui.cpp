#include "precomp_dboclient.h"
#include "DiceGui.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "InfoWndManager.h"
#include "DiceManager.h"
#include "DialogManager.h"
#include "AlarmManager.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"


#define dDICE_FLASH_NAME		"Dice_play.swf"
#define dDICE_LIMIT_TIME		((RwReal)(DBO_MAX_TIME_PARTY_DICE_ITEM/1000))
#define dDICE_WAIT_FOR_DISAPPEAR	3.f		///< 주사위 숫자가 표시되고 나서 사라지기 까지의 시간


//////////////////////////////////////////////////////////////////////////
//	CActionDiceGui
//////////////////////////////////////////////////////////////////////////

CActionDiceGui::~CActionDiceGui()
{

}

RwBool CActionDiceGui::Create(RwUInt8 byFunctionFlag)
{
	NTL_FUNCTION( "CActionDiceGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\Dice.srf", "gui\\ActionDice.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// 창닫기 버튼
	m_pExitButton = (gui::CButton*)GetComponent( "btnExit" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CActionDiceGui::OnClicked_CloseButton);

	// 주사위 플래쉬
	m_pflashDice = (gui::CFlash*)GetComponent("flaDice");
	m_slotMovieEnd = m_pflashDice->SigMovieEnd().Connect( this, &CActionDiceGui::OnMovieEnd );
	m_slotFSCallback = m_pflashDice->SigFSCallBack().Connect( this, &CActionDiceGui::OnFSCallback );

	// 액션 주사위 설명
	m_pActionDiceExplain = (gui::CStaticBox*)GetComponent("stbActionDiceExplain");	

	// 액션 주사위 설명2
	m_pActionDiceExplain2 = (gui::CStaticBox*)GetComponent("stbActionDiceExplain2");
	m_pActionDiceExplain2->SetTextColor( RGB(255, 192, 0) );

	// 주사위를 굴린 숫자
	m_pResultNum = (gui::CStaticBox*)GetComponent("stbResultNum");
	m_pResultNum->CreateFontStd(DEFAULT_FONT, 300, DEFAULT_FONT_ATTR);

	// default value
	m_byFunctionFlag = byFunctionFlag;

	if( BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLOSE_BUTTON) )
		m_pExitButton->ClickEnable(false);

	if( BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
		m_pflashDice->Enable(false);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CActionDiceGui::Destroy()
{
	NTL_FUNCTION("CActionDiceGui::Destroy");

	CheckInfoWindow();
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	m_pflashDice->Unload();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CActionDiceGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	m_pflashDice->Update(fElapsed);
}

RwBool CActionDiceGui::ReadyDice()
{
	if( m_bGenerateResultEvent )
		return FALSE;

	if( m_pflashDice->Load(dDICE_FLASH_NAME) == false )
	{
		DBO_FAIL("Not eixst dice flash file : " << dDICE_FLASH_NAME );		
		return FALSE;
	}

	CRectangle rtFlash = m_pflashDice->GetFrameResolution();

	m_pflashDice->SetSize(rtFlash.GetWidth(), rtFlash.GetHeight());

	return TRUE;
}

VOID CActionDiceGui::SetResultNum(RwUInt8 byNum)
{
	m_byResultNum = byNum;

	if( BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
	{
		m_pflashDice->Invoke("gotoAndPlay", "%s", "click");
	}
	else
	{
		m_pflashDice->Invoke("gotoAndPlay", "%s", "play");
	}	
}

VOID CActionDiceGui::SetExplain(const WCHAR* pwcText)
{
	m_pActionDiceExplain->SetText(pwcText);
}

VOID CActionDiceGui::SetExplain2(const WCHAR* pwcText)
{
	m_pActionDiceExplain2->SetText(pwcText);
}

VOID CActionDiceGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_DICE_MANAGER )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CActionDiceGui::OnClicked_CloseButton(gui::CComponent* pComponent)
{
	Switch(false);
	GetDiceManager()->LineUpDice();
}

VOID CActionDiceGui::OnMovieEnd(gui::CComponent* pComponent)
{
	// 주사위가 돌아가다가 멈추었을 때
	m_pResultNum->SetText(m_byResultNum);

	// 다시 주사위를 돌리기 위한 초기화
	ReadyDice();

	if( false == BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
	{
		m_pflashDice->Enable(true);
		m_pflashDice->RestartMovie();
	}

	m_bGenerateResultEvent = FALSE;
	CDboEventGenerator::DiceResultShow(Logic_GetAvatarHandle(), m_byWorkID, m_byResultNum);
}

VOID CActionDiceGui::OnFSCallback(const char* pcString, const char* pcString2)
{
	if( !pcString )
		return;

	// 주사위를 클릭했을 때
	CDboEventGenerator::DiceRequestResulttoServer(m_byWorkID);	

	m_pResultNum->Clear();
	m_pflashDice->Enable(false);
	m_bGenerateResultEvent = TRUE;
}

VOID CActionDiceGui::Switch(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
		GetNtlGuiManager()->AddUpdateFunc( this );
	}
	else
	{
		if( m_bGenerateResultEvent )
		{
			// 주사위 플래쉬 애니메이션이 끝나 결과를 보여주기 전에 주사위 GUI를 닫았다
			// 주사위 GUI 외에 주사위 결과를 보여주는 곳(채팅, 말풍선 등)을 위해 이벤트를 보낸다
			CDboEventGenerator::DiceResultShow(Logic_GetAvatarHandle(), m_byWorkID, m_byResultNum);
		}

		CheckInfoWindow();

		Show(false);
		GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

		if( false == BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
			m_pflashDice->Enable(true);

		m_pResultNum->Clear();
		m_pflashDice->Unload();
		m_byResultNum			= 0;
		m_bGenerateResultEvent	= FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
//	CItemDiceGui
//////////////////////////////////////////////////////////////////////////

CItemDiceGui::CItemDiceGui(const RwChar* pName, RwUInt8 byWorkID)
:CDiceGui(pName, byWorkID)
,m_fElapsed(0.f)
,m_pItemProfile(NULL)
{}

CItemDiceGui::~CItemDiceGui()
{

}

RwBool CItemDiceGui::Create(RwUInt8 byFunctionFlag)
{
	NTL_FUNCTION( "CItemDiceGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\Dice.srf", "gui\\ItemDice.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// 창닫기 버튼
	m_pExitButton = (gui::CButton*)GetComponent( "btnExit" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CItemDiceGui::OnClicked_CloseButton);

	// 주사위 플래쉬
	m_pflashDice = (gui::CFlash*)GetComponent("flaDice");
	m_slotMovieEnd = m_pflashDice->SigMovieEnd().Connect( this, &CItemDiceGui::OnMovieEnd );
	m_slotFSCallback = m_pflashDice->SigFSCallBack().Connect( this, &CItemDiceGui::OnFSCallback );

	// 자동 주사위 설명
	m_pAutoDiceExplain = (gui::CStaticBox*)GetComponent("stbAutoDiceExplain");
	
	// 주사위를 굴린 숫자
	m_pResultNum = (gui::CStaticBox*)GetComponent("stbResultNum");
	m_pResultNum->CreateFontStd(DEFAULT_FONT, 300, DEFAULT_FONT_ATTR);

	// 아이템 슬롯 배경
	m_srfItemSlot.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Dice.srf", "srfItemSlot" ) );
	m_srfItemSlot.SetPositionfromParent(228, 18);

	// 주사위 굴리기 시간 제한 배경
	m_srfTimeGaugeBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Dice.srf", "srfTimeGaugeBack" ) );
	m_srfTimeGaugeBack.SetPositionfromParent(21, 73);

	m_slot.Create(m_pThis, DIALOG_DICE_MANAGER, REGULAR_SLOT_ITEM_TABLE);
	m_slot.SetPosition_fromParent(230, 34);

	// 주사위 굴리기 시간 제한
	m_pTimeGauge = (gui::CProgressBar*)GetComponent("TimeGauge");	

	// default value
	m_byFunctionFlag = byFunctionFlag;

	if( BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLOSE_BUTTON) )
		m_pExitButton->ClickEnable(false);

	if( BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
		m_pflashDice->Enable(false);

	// sig
	m_slotMove			= m_pThis->SigMove().Connect( this, &CItemDiceGui::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CItemDiceGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CItemDiceGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CItemDiceGui::OnPaint );

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CItemDiceGui::Destroy()
{
	NTL_FUNCTION("CItemDiceGui::Destroy");

	CheckInfoWindow();

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	m_pflashDice->Unload();

	m_slot.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CItemDiceGui::Update(RwReal fElapsed)
{
	if( !IsShow() )
		return;

	m_pflashDice->Update(fElapsed);

	if( m_WaitDiceDisappear.bWaitDisappear )
	{
		if( m_WaitDiceDisappear.fElapsedDisappear != 0.f )
		{
			m_WaitDiceDisappear.fElapsedDisappear -= fElapsed;

			if( m_WaitDiceDisappear.fElapsedDisappear < 0.f )
			{
				m_WaitDiceDisappear.fElapsedDisappear = 0.f;
				GetDiceManager()->Cancel(m_byWorkID);
			}
		}
	}


	if( m_fElapsed == dDICE_LIMIT_TIME )
		return;

	m_fElapsed += fElapsed;
	if( m_fElapsed > dDICE_LIMIT_TIME )
	{
		m_fElapsed = dDICE_LIMIT_TIME;
		m_WaitDiceDisappear.bWaitDisappear = TRUE;
	}


	RwInt32 iMaxRange, iPos;

	m_pTimeGauge->GetRange(iPos, iMaxRange); // iPos는 여기서는 더미로 쓰인다

	iPos = (RwInt32)CNtlMath::Interpolation((RwReal)iMaxRange, 0.f, m_fElapsed/dDICE_LIMIT_TIME);
	m_pTimeGauge->SetPos(iPos);
}

RwBool CItemDiceGui::ReadyDice()
{
	m_fElapsed		= 0.f;

	m_WaitDiceDisappear.fElapsedDisappear	= dDICE_WAIT_FOR_DISAPPEAR;
	m_WaitDiceDisappear.bWaitDisappear		= FALSE;

	m_pflashDice->Enable(true);

	if( m_pflashDice->Load(dDICE_FLASH_NAME) == false )
	{
		DBO_FAIL("Not eixst dice flash file : " << dDICE_FLASH_NAME );		
		return FALSE;
	}

	CRectangle rtFlash = m_pflashDice->GetFrameResolution();

	m_pflashDice->SetSize(rtFlash.GetWidth(), rtFlash.GetHeight());
	
	return TRUE;
}

VOID CItemDiceGui::SetItemData(void* pItemData)
{
	if( !pItemData )
	{
		DBO_FAIL("Invalid item data pointer");
		return;
	}

	m_slot.Clear();

	sITEM_PROFILE* pItemProfile = (sITEM_PROFILE*)pItemData;

	if( pItemProfile->bNeedToIdentify )
	{
		m_slot.SetSerialType(REGULAR_SLOT_ITEM_NOT_IDENTIFICATION);
	}
	else
	{
		m_slot.SetSerialType(REGULAR_SLOT_ITEM_TABLE);
	}

	m_slot.SetIcon(pItemProfile->tblidx);
	m_pItemProfile = pItemProfile;
}

VOID CItemDiceGui::SetExplain(const WCHAR* pwcText)
{
	m_pAutoDiceExplain->SetText(pwcText);
}

VOID CItemDiceGui::SetResultNum(RwUInt8 byNum)
{
	m_byResultNum = byNum;

	if( BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
	{
		m_pflashDice->Invoke("gotoAndPlay", "%s", "click");
	}
	else
	{
		m_pflashDice->Invoke("gotoAndPlay", "%s", "play");
	}
}

VOID CItemDiceGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_DICE_MANAGER )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CItemDiceGui::OnClicked_CloseButton(gui::CComponent* pComponent)
{
	Switch(false);
	GetDiceManager()->LineUpDice();
}

VOID CItemDiceGui::OnMovieEnd(gui::CComponent* pComponent)
{
	// 주사위가 돌아가다가 멈추었을 때
	m_pResultNum->SetText(m_byResultNum);

	if( false == BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
	{
		m_pflashDice->Enable(true);
	}

	m_bGenerateResultEvent = FALSE;
	CDboEventGenerator::DiceResultShow(Logic_GetAvatarHandle(), m_byWorkID, m_byResultNum);
}

VOID CItemDiceGui::OnFSCallback(const char* pcString, const char* pcString2)
{
	if( !pcString )
		return;

	CDboEventGenerator::DiceRequestResulttoServer(m_byWorkID);	

	m_pResultNum->Clear();
	m_pflashDice->Enable(false);
	m_bGenerateResultEvent = TRUE;
}

VOID CItemDiceGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_srfItemSlot.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfTimeGaugeBack.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_slot.SetParentPosition(rtScreen.left, rtScreen.top);

	CheckInfoWindow();
}

VOID CItemDiceGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( m_slot.GetSerial() != INVALID_SERIAL_ID )
	{
		if( m_slot.PtInRect(nX, nY) )
		{
			if( GetInfoWndManager()->GetRequestGui() == DIALOG_DICE_MANAGER )
				return;

			CRectangle rtScreen = m_pThis->GetScreenRect();

			if( m_slot.GetSerialType() == REGULAR_SLOT_ITEM_NOT_IDENTIFICATION )
			{
				GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM,
													rtScreen.left + m_slot.GetX_fromParent(),
													rtScreen.top + m_slot.GetY_fromParent(),
													NULL, DIALOG_DICE_MANAGER );
			}
			else
			{
				if( m_pItemProfile )
				{
					GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_PROFILE_ITEM,
													rtScreen.left + m_slot.GetX_fromParent(),
													rtScreen.top + m_slot.GetY_fromParent(),
													m_pItemProfile, DIALOG_DICE_MANAGER );
				}
				else
				{
					DBO_FAIL("Invalid item profile");
				}
			}

			return;
		}
	}

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CItemDiceGui::OnMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CItemDiceGui::OnPaint()
{
	m_srfItemSlot.Render();
	m_srfTimeGaugeBack.Render();

	if( m_slot.GetSerial() != INVALID_SERIAL_ID )
		m_slot.Paint();
}

VOID CItemDiceGui::Switch(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
		GetNtlGuiManager()->AddUpdateFunc( this );		
	}
	else
	{
		if( m_bGenerateResultEvent )
		{
			// 주사위 플래쉬 애니메이션이 끝나 결과를 보여주기 전에 주사위 GUI를 닫았다
			// 주사위 GUI 외에 주사위 결과를 보여주는 곳(채팅, 말풍선 등)을 위해 이벤트를 보낸다
			CDboEventGenerator::DiceResultShow(Logic_GetAvatarHandle(), m_byWorkID, m_byResultNum);
		}

		CheckInfoWindow();

		Show(false);
		GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

		if( false == BIT_FLAG_TEST(m_byFunctionFlag, dDICE_FUNCTION_DISABLE_CLICK_DICE) )
			m_pflashDice->Enable(true);

		m_slot.Clear();
		m_pResultNum->Clear();
		m_pflashDice->Unload();
		m_byResultNum			= 0;
		m_bGenerateResultEvent	= FALSE;
		m_pItemProfile			= NULL;
	}
}