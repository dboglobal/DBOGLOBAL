#include "precomp_dboclient.h"
#include "SkillAbilityUseGui.h"

// core
#include "NtlDebug.h"

/// Sound
#include "GuiSoundDefine.h"

// shared
#include "NtlSkill.h"

// presentation
#include "NtlPLGuiManager.h"

// simualtion
#include "NtlSLLogic.h"
#include "NtlSobSkill.h"
#include "NtlSobSkillAttr.h"
#include "NtlSobSkillIcon.h"
#include "InputActionMap.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"

// result
#include "NtlResultCode.h"

// Dbo
#include "DboLogic.h"
#include "DboEvent.h"
#include "DialogManager.h"
#include "DialogPriority.h"
#include "DboGlobal.h"

CSkillAbilityUseIcon::CSkillAbilityUseIcon()
: m_pSkillAbilityUseGui(NULL)
, m_pStbNumber(NULL)
, m_pFlaUseIcon(NULL)
, m_bSelected(FALSE)
, m_bEnable(FALSE)
, m_byAbilityType(FALSE)
, m_byIconIndex(INVALID_BYTE)
{
}

CSkillAbilityUseIcon::~CSkillAbilityUseIcon()
{

}

VOID CSkillAbilityUseIcon::Create( CSkillAbilityUseGui* pGui, gui::CFlash* pComponent, RwUInt8 byAbilityType )
{
	m_pSkillAbilityUseGui = pGui;
	m_pFlaUseIcon = pComponent;
	m_byAbilityType = byAbilityType;

	CRectangle rect;
	rect.SetRectWH( 44, 44, 20, 30 );
	
	m_pStbNumber = NTL_NEW gui::CStaticBox( rect, pGui->GetDialog(), GetNtlGuiManager()->GetSurfaceManager() );	
	m_pStbNumber->CreateFontStd( DEFAULT_FONT, 120, DEFAULT_FONT_ATTR);
	m_pStbNumber->SetTextStyle( COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbNumber->SetEffectMode(TE_SHADOW);
	m_pStbNumber->SetEffectColor(DEFAULT_SHADOW_EFFECT_VALUE);
	m_pStbNumber->SetText(L"");
	m_pStbNumber->Enable( false );

	// Callback
	m_slotFSCallBack = m_pFlaUseIcon->SigFSCallBack().Connect( this, &CSkillAbilityUseIcon::OnFSCallBack );
	m_slotMovieEnd = m_pFlaUseIcon->SigMovieEnd().Connect( this, &CSkillAbilityUseIcon::OnMovieEnd );
	m_slotMouseDown = m_pFlaUseIcon->SigMouseDown().Connect(this, &CSkillAbilityUseIcon::OnMouseDown);
}

VOID CSkillAbilityUseIcon::Destroy()
{
	NTL_DELETE( m_pStbNumber );
}

VOID CSkillAbilityUseIcon::Update( RwReal fElapsed )
{
	m_pFlaUseIcon->Update( fElapsed );
}

VOID CSkillAbilityUseIcon::GotoStartFrame()
{
	if( m_pFlaUseIcon )
	{
		m_pFlaUseIcon->Invoke( "gotoAndPlay", "%s", "start" );
		m_pFlaUseIcon->RestartMovie();
	}
}

VOID CSkillAbilityUseIcon::GotoEndFrame()
{
	if( m_pFlaUseIcon )
	{
		m_pFlaUseIcon->Invoke( "gotoAndPlay", "%s", "end" );
	}
}

VOID CSkillAbilityUseIcon::GotoAutoFrame()
{
	if( m_pFlaUseIcon )
	{
		m_pFlaUseIcon->Invoke( "gotoAndPlay", "%s", "auto" );
	}
}

VOID CSkillAbilityUseIcon::GotoDisableFrame()
{
	if( m_pFlaUseIcon )
	{
		m_pFlaUseIcon->Invoke( "gotoAndPlay", "%s", "disable" );
	}
}

VOID CSkillAbilityUseIcon::OnFSCallBack( const char* pChar1, const char* pChar2 )
{
	if( !strcmp( pChar1, "push" ) )
	{
		if( m_pFlaUseIcon )
		{
			// Selected
			m_pSkillAbilityUseGui->SelectedSkillAbility( m_byAbilityType );
			m_pSkillAbilityUseGui->ShowAllIcon( false );
		
			Show( true );
			Select( TRUE );
		}
	}
}

VOID CSkillAbilityUseIcon::OnMovieEnd( gui::CComponent* pComponent )
{
	if( m_bSelected )	
		GetDialogManager()->CloseDialog( DIALOG_SKILLABILITYUSE );
}

void CSkillAbilityUseIcon::OnMouseDown(const CKey & key)
{
	if (m_pFlaUseIcon && IsEnable())
	{
		// Selected
		m_pSkillAbilityUseGui->SelectedSkillAbility(m_byAbilityType);
		m_pSkillAbilityUseGui->ShowAllIcon(false);

		Show(true);
		Select(TRUE);

		GotoEndFrame();
	}
}

VOID CSkillAbilityUseIcon::PlayMovie( RwBool bPlay )
{
	m_pFlaUseIcon->PlayMovie( bPlay );
}

VOID CSkillAbilityUseIcon::Show( RwBool bShow )
{
	if( bShow )
	{
		Select( FALSE );
	}
	else
	{
		m_pStbNumber->Clear();
	}

	m_pFlaUseIcon->Show( B2b(bShow) );
}

VOID CSkillAbilityUseIcon::Select( RwBool bSelect )
{
	m_bSelected = bSelect;
}

VOID CSkillAbilityUseIcon::Enable( RwBool bEnable )
{
	m_bEnable = bEnable;
}

RwBool CSkillAbilityUseIcon::IsEnable()
{
	return m_bEnable;
}

RwUInt8 CSkillAbilityUseIcon::GetIconIndex()
{
	return m_byIconIndex;
}

RwBool CSkillAbilityUseIcon::IsVisible()
{
	return m_pFlaUseIcon->IsVisible();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSkillAbilityUseGui
//

CSkillAbilityUseGui::CSkillAbilityUseGui( const RwChar* pName )
: CNtlPLGui( pName )
, m_fElapsedTime(0.0f)
, m_byShowIconNum(0)
, m_pSkillIcon(NULL)
, m_bActive(FALSE)
, m_bInput(FALSE)
, m_bSelected(FALSE)
{
}

CSkillAbilityUseGui::~CSkillAbilityUseGui()
{

}

RwBool CSkillAbilityUseGui::Create( VOID )
{
	NTL_FUNCTION( "CSkillAbilityUseGui::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\SkillAbilityUse.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis		= (gui::CDialog*)GetComponent( "dlgMain" );
	m_pThis->SetPriority( dDIALOGPRIORITY_DEFAULT );

	m_slotKeyDown = m_pThis->GetGuiManager()->SigCaptureKeyDown().Connect( this, &CSkillAbilityUseGui::OnKeyDown );

	m_UseIcon[DBO_RP_BONUS_TYPE_KNOCKDOWN].Create( this, (gui::CFlash*)GetComponent("swfKnockDown"), DBO_RP_BONUS_TYPE_KNOCKDOWN );
	m_UseIcon[DBO_RP_BONUS_TYPE_RESULT_PLUS].Create( this, (gui::CFlash*)GetComponent("swfResult"), DBO_RP_BONUS_TYPE_RESULT_PLUS );
	m_UseIcon[DBO_RP_BONUS_TYPE_EP_MINUS].Create( this, (gui::CFlash*)GetComponent("swfEp"), DBO_RP_BONUS_TYPE_EP_MINUS );
	m_UseIcon[DBO_RP_BONUS_TYPE_KEEP_TIME_PLUS].Create( this, (gui::CFlash*)GetComponent("swfKeepTime"), DBO_RP_BONUS_TYPE_KEEP_TIME_PLUS );
	m_UseIcon[DBO_RP_BONUS_TYPE_CASTING_TIME_MINUS].Create( this, (gui::CFlash*)GetComponent("swfCastingTime"), DBO_RP_BONUS_TYPE_CASTING_TIME_MINUS );
	m_UseIcon[DBO_RP_BONUS_TYPE_COOL_TIME_MINUS].Create( this, (gui::CFlash*)GetComponent("swfCoolTime"), DBO_RP_BONUS_TYPE_COOL_TIME_MINUS );
	m_UseIcon[DBO_RP_BONUS_TYPE_GUARD_CRASH].Create( this, (gui::CFlash*)GetComponent("swfGuardCrash"), DBO_RP_BONUS_TYPE_GUARD_CRASH );

	Show( false );
	ShowAllIcon( false );

	LinkMsg( g_EventRpBonusSelect, 0 );
	LinkMsg( g_EventRpBonusAuto, 0 );

	GetNtlGuiManager()->AddUpdateFunc( this );

	NTL_RETURN( TRUE );
}

VOID CSkillAbilityUseGui::Destroy( VOID )
{
	NTL_FUNCTION( "CSkillAbiltiyUseGui::Destroy" );

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	for( RwInt32 i =0; i<DBO_RP_BONUS_TYPE_COUNT; ++i )
	{
		m_UseIcon[i].Destroy();
	}

	UnLinkMsg( g_EventRpBonusSelect );
	UnLinkMsg( g_EventRpBonusAuto );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CSkillAbilityUseIcon::InitIcon()
{
	m_byIconIndex = INVALID_BYTE;
}

VOID CSkillAbilityUseIcon::SetIconPos( RwUInt8 byIndex  )
{
	m_byIconIndex = byIndex;
	m_pFlaUseIcon->SetPosition( m_byIconIndex * dSKILL_ABILITY_WIDTH_OFFSET, 0 );
	m_pStbNumber->SetPosition( m_byIconIndex * dSKILL_ABILITY_WIDTH_OFFSET, 0 );
}

void CSkillAbilityUseIcon::SetIconNumber()
{
	m_pStbNumber->SetText(m_byIconIndex + 1);
}


VOID CSkillAbilityUseGui::Update( RwReal fElapsed )
{
	if( !m_bActive )
		return;

	m_fElapsedTime += fElapsed;

	for( int i=0; i < DBO_RP_BONUS_TYPE_COUNT; ++i )
		m_UseIcon[i].Update( fElapsed );
	
	if( m_fElapsedTime > dSKILL_ABILITY_ELAPSED_TIME && m_bSelected == FALSE )
	{
		GetDialogManager()->CloseDialog( DIALOG_SKILLABILITYUSE );
	}
}

VOID CSkillAbilityUseGui::HandleEvents( RWS::CMsg &msg )
{
	if( msg.Id == g_EventRpBonusSelect )
	{
		if( GetDialogManager()->IsOpenDialog( DIALOG_SKILL_HTB_RPUSE ) )
			return;
		if( m_bActive )
			GetDialogManager()->CloseDialog( DIALOG_SKILLABILITYUSE );

		m_pSkillIcon = reinterpret_cast<CNtlSobSkillIcon*>(msg.pData);
		CNtlSobSkill* pSkill = (CNtlSobSkill*)m_pSkillIcon->GetSobObj();

		if( m_pSkillIcon->GetIconState() == CNtlSobIcon::ICON_STATE_COOLING )
		{
			CNtlSLEventGenerator::SysMsg( pSkill->GetOwnerID(), "GAME_SKILL_NOT_READY_TO_BE_CAST" );
			return;
		}

		CNtlSobSkillAttr* pSkillAttr = (CNtlSobSkillAttr*)pSkill->GetSobAttr();
		sSKILL_TBLDAT* pSkillTbl = pSkillAttr->GetSkillTbl();

		GetDialogManager()->OpenDialog( DIALOG_SKILLABILITYUSE );

		InitIcon();
		ShowAllIcon( FALSE );

		RwUInt8 byIconPos = 0;
		for( int i=0; i< DBO_MAX_RP_BONUS_COUNT_PER_SKILL; ++i )
		{
			/*if( pSkillTbl->abyRpEffect[i] != 0xFF )
			{*/
				SetAbility( byIconPos, pSkillTbl->abyRpEffect[i], pSkillTbl );
				byIconPos++;
			/*}*/
		}

		SetActive( TRUE );
		m_fElapsedTime = 0.0f;
		m_bSelected = FALSE;
		m_bInput = TRUE;

		SetAdjustGui();
	}
	else if( msg.Id == g_EventRpBonusAuto )
	{
		if( m_bActive )
			GetDialogManager()->CloseDialog( DIALOG_SKILLABILITYUSE );

		SDboEventRpBonusAuto* sAuto = reinterpret_cast<SDboEventRpBonusAuto*>( msg.pData );

		GetDialogManager()->OpenDialog( DIALOG_SKILLABILITYUSE );

		InitIcon();
		ShowAllIcon( FALSE );
	
		m_UseIcon[ sAuto->byRpBonusType ].SetIconPos( 0 );
		m_UseIcon[ sAuto->byRpBonusType ].Show( TRUE );
		AddShowIcon(1);

		SetAdjustGui();
		
		m_UseIcon[ sAuto->byRpBonusType ].PlayMovie( TRUE );
		m_UseIcon[ sAuto->byRpBonusType ].GotoAutoFrame();
		m_UseIcon[ sAuto->byRpBonusType ].Select( TRUE );

		SetActive( TRUE );
		m_bSelected = TRUE;
		m_bInput = FALSE;
		m_fElapsedTime = 0.0f;

	}
}

void CSkillAbilityUseGui::ShowAllIcon(RwBool bShow )
{
	for( int i=0; i < DBO_RP_BONUS_TYPE_COUNT; ++i )
	{
		m_UseIcon[i].Show( bShow );
	}
}

RwInt32 CSkillAbilityUseGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		Logic_PlayGUISound(GSD_SYSTEM_ABILITY_UI_OPEN);		
	}	
	else
	{
		m_fElapsedTime = 0.0f;
		SetActive( FALSE );
		m_bSelected = FALSE;
		ShowAllIcon( FALSE );
		m_bInput = FALSE;
	}

	Show( bOpen );

	return 1;
}

VOID CSkillAbilityUseGui::SetActive( RwBool bActive )
{
	m_bActive = bActive;
}

VOID CSkillAbilityUseGui::AddShowIcon( RwUInt8 byAddIconNum )
{
	m_byShowIconNum = (RwUInt8)(m_byShowIconNum + byAddIconNum);
}	

VOID CSkillAbilityUseGui::SetAdjustGui()
{
	RwUInt8 byIconMax = 0;
	for(int i=0; i < DBO_RP_BONUS_TYPE_COUNT; ++i )
	{	
		RwUInt8 byIconCur = m_UseIcon[i].GetIconIndex();
		if( byIconCur == 0xFF )
			continue;

		if( byIconMax < byIconCur )
			byIconMax = byIconCur;
	}

	int nWidth = (byIconMax*dSKILL_ABILITY_WIDTH_OFFSET) + dSKILL_ABILITY_WIDTH_OFFSET;
	int nHeight = dSKILL_ABILITY_ICON_HEIGHT;

	m_pThis->SetSize( nWidth, nHeight );

	CRectangle rect = m_pThis->GetScreenRect();

	int nNewPosX = (RwInt32)((RwReal)GetDboGlobal()->GetScreenWidth()*0.5f) - (RwInt32)((RwReal)rect.GetWidth()*0.5f) ;
	int nNewPosY = (RwInt32)((RwReal)GetDboGlobal()->GetScreenHeight()*0.7f) - (RwInt32)((RwReal)rect.GetHeight()*0.5f) ;

	m_pThis->SetPosition( nNewPosX, nNewPosY );
}

VOID CSkillAbilityUseGui::InitIcon()
{
	m_byShowIconNum = 0;

	for( int i=0; i< DBO_RP_BONUS_TYPE_COUNT; ++i )
	{
		m_UseIcon[i].InitIcon();
		m_UseIcon[i].Enable( FALSE );
	}
}

VOID CSkillAbilityUseGui::SetAbility( RwUInt8 byIconIndex, RwUInt8 byAbilityType, sSKILL_TBLDAT* pSkillTbl, RwBool bEnable /*= TRUE */ )
{
	if( byAbilityType == DBO_RP_BONUS_TYPE_INVALID )
	{
		if( pSkillTbl->dwNextSkillTblidx == INVALID_DWORD )
		{
			return;
		}

		sSKILL_TBLDAT* pNextSkillTbl = Logic_GetSkillDataFromTable( pSkillTbl->dwNextSkillTblidx );

		SetAbility( byIconIndex, pNextSkillTbl->abyRpEffect[ byIconIndex ], pNextSkillTbl, FALSE );
	}
	else
	{
		if( byAbilityType >= DBO_RP_BONUS_TYPE_COUNT )
		{
			DBO_FAIL( "CSkillAbilityUseGui::SetAbility - byAbilityType over DBO_RP_BONUS_TYPE" );
			return;
		}
		
		if( byIconIndex >= CSkillAbilityUseIcon::ICONPOS_NUMS )
		{
			DBO_FAIL( "CSkillAbilityUSeGui::SetAbility - byIconIndex over ICONPOS");
			return;
		}

		m_UseIcon[byAbilityType].SetIconPos( byIconIndex );
		m_UseIcon[byAbilityType].Show( true );
		m_UseIcon[byAbilityType].Enable( bEnable );

		if(bEnable)
			m_UseIcon[byAbilityType].SetIconNumber();
		
		AddShowIcon(1);

		m_UseIcon[byAbilityType].PlayMovie( TRUE );

		if( bEnable )
			m_UseIcon[byAbilityType].GotoStartFrame();
		else
			m_UseIcon[byAbilityType].GotoDisableFrame();
	}
}

VOID CSkillAbilityUseGui::SelectedSkillAbility( RwUInt8 byAbilityType )
{
	if( m_pSkillIcon )
	{
		CNtlSobSkill* pSkill = (CNtlSobSkill*)m_pSkillIcon->GetSobObj();
		Logic_UseProc( pSkill->GetSerialID(), byAbilityType );

		m_bSelected = TRUE;
	}
}

VOID CSkillAbilityUseGui::OnKeyDown( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key )
{
	if( !m_bActive  || !m_bInput || m_bSelected)
		return;

	if( key.m_nState == CKey::PRESSED &&
		key.m_nID & 0x40000000 )
		return;

	for( RwInt32 i=0; i < DBO_RP_BONUS_TYPE_COUNT; ++i )
	{
		if( m_UseIcon[i].IsEnable() )
		{
			if( ( key.m_dwVKey - (m_UseIcon[i].GetIconIndex()+49) ) == 0 )
			{
				GetInputActionMap()->RegisterIgnoreKey( (RwUInt8)key.m_dwVKey, 1 );

				m_UseIcon[i].OnFSCallBack("push", "");
				m_UseIcon[i].GotoEndFrame();
				m_bInput = FALSE;
			}
		}
	}

}