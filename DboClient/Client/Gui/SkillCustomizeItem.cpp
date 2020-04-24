#include "precomp_dboclient.h"
#include "SkillCustomizeItem.h"

// core
#include "NtlDebug.h"

// table
#include "SkillTable.h"
#include "PCTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLGlobal.h"
#include "NtlSkillContainer.h"
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"
#include "NtlSobIcon.h"
#include "NtlSobSkillAttr.h"

// dbo
#include "DboGlobal.h"
#include "DboPacketGenerator.h"
#include "DboPacketGenerator.h"
#include "DboEventGenerator.h"
#include "DboLogic.h"
#include "IconMoveManager.h"
#include "InfoWndManager.h"
#include "DisplayStringManager.h"
#include "DialogManager.h"
#include "SkillCustomizeParser.h"
#include "SkillCommon.h"

//#define SKILLWND_ITEM_RPTYPE_X		0
//#define SKILLWND_ITEM_RPTYPE_Y		0
#define SKILLWND_ITEM_RPTYPE_WIDTH	12
#define SKILLWND_ITEM_RPTYPE_HEIGHT	12

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSkillCustomizeItemGroup

CSkillCustomizeItem::CSkillCustomizeItem( CSkillCustomizeSkillItem* pParsedItem )
: m_pItem( pParsedItem ), m_pSobIcon( NULL ), m_bUpgradable( FALSE ), m_eLClickIcon( STATE_NONE ), m_eRClickIcon( STATE_NONE ), m_bPushDownIcon( FALSE ), m_fRightClickTime( 0.0f )
{
	// RPType
	m_surRPType.Show( FALSE );

	// Effect
	CRectangle rtIcon = m_pItem->GetSkillIcon()->GetScreenRect();
	m_CoolTimeEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );
	m_surFocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );	
	m_surPickedUp.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );

	m_surLearnAbleEffect.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("gui\\SkillCustomize.srf", "srfLearnableSkillBack"));
	m_surLearnAbleEffect.SetClippingMode(TRUE);
	m_surLearnAbleEffect.Show(FALSE);

	m_surFocusEffect.SetSize( DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_surPickedUp.SetSize( DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );

	m_surFocusEffect.Show( FALSE );
	m_surPickedUp.Show( FALSE );
	m_surRPType.Show( FALSE );

	m_surFocusEffect.SetClippingMode( TRUE );
	m_surPickedUp.SetClippingMode( TRUE );
	m_surRPType.SetClippingMode( TRUE );	
	m_CoolTimeEffect.SetClippingMode( TRUE );

	// CallBack;
	gui::CPanel* pIcon = m_pItem->GetSkillIcon();
	gui::CButton* pRPButton = m_pItem->GetRPButton();
	gui::CButton* pSPButton = m_pItem->GetSPButton();

	m_slotIconMouseDown = pIcon->SigMouseDown().Connect( this, &CSkillCustomizeItem::OnIconMouseDown );
	m_slotIconMouseUp = pIcon->SigMouseUp().Connect( this, &CSkillCustomizeItem::OnIconMouseUp );
	m_slotIconMouseEnter = pIcon->SigMouseEnter().Connect( this, &CSkillCustomizeItem::OnIconMouseEnter );
	m_slotIconMouseLeave = pIcon->SigMouseLeave().Connect( this, &CSkillCustomizeItem::OnIconMouseLeave );
	m_slotIconPaint = pIcon->SigPaint().Connect( this, &CSkillCustomizeItem::OnIconPaint );
	m_slotIconMove = pIcon->SigMove().Connect( this, &CSkillCustomizeItem::OnIconMove );

	if( pSPButton )
	{
		m_slotSPButtonEnter = pSPButton->SigMouseEnter().Connect( this, &CSkillCustomizeItem::OnSPButtonEnter );
		m_slotSPButtonLeave = pSPButton->SigMouseLeave().Connect( this, &CSkillCustomizeItem::OnSPButtonLeave );
		m_slotSPButtonClick = pSPButton->SigClicked().Connect( this, &CSkillCustomizeItem::OnSPButtonClick );		
	}

	if( pRPButton )
	{
		pRPButton->SetToolTip( GetDisplayStringManager()->GetString( "DST_SKILL_RPMODE" ) );
		pRPButton->ClickEnable( FALSE );
		m_slotRPButtonClick = pRPButton->SigClicked().Connect( this, &CSkillCustomizeItem::OnRPButtonClick );
	}	

	SetColor( SKILLWND_ITEM_NOT_LEARN_COLOR_RED, SKILLWND_ITEM_NOT_LEARN_COLOR_GREEN, SKILLWND_ITEM_NOT_LEARN_COLOR_BLUE );		

	if( m_pItem->GetSkillType() == CSkillCustomizeSkillItem::SKILL )
	{
		sSKILL_TBLDAT* pSkillTableData = reinterpret_cast<sSKILL_TBLDAT*>( m_pItem->GetSkillData() );
		if( pSkillTableData->bDefaultDisplayOff )
			Show( false );
		else
			Show( true );
	}

	m_pflaSelectEffect = NULL;
}

CSkillCustomizeItem::~CSkillCustomizeItem(VOID)
{

}

VOID CSkillCustomizeItem::Update( RwReal fElapsed )
{
	if( m_pSobIcon )
	{
		if( m_pSobIcon->GetIconState() == CNtlSobIcon::ICON_STATE_COOLING )
			m_CoolTimeEffect.Update( m_pSobIcon->GetCoolingTime() );

		if (m_pflaSelectEffect)
			m_pflaSelectEffect->Update(fElapsed);

		if( m_eRClickIcon == STATE_CLICK )
		{
			m_fRightClickTime += fElapsed;

			if( m_fRightClickTime > SKILL_RPBONUS_KEYTIME )
			{
				if( Logic_UseProcRpBonusSkill( m_pSobIcon->GetSobObj()->GetSerialID() ) )
				{
					m_eLClickIcon = STATE_NONE;
					m_eRClickIcon = STATE_NONE;
					ClickEffect( FALSE, FALSE );
					m_pItem->GetSkillIcon()->ReleaseMouse();
				}

				m_fRightClickTime = 0.0f;
			}
		}
	}	
}

VOID CSkillCustomizeItem::SetInitSkill(VOID)
{
	m_pSobIcon = NULL;
	m_pItem->SetInitSkill();

	m_surRPType.Show( FALSE );
	ShowPickedUp( FALSE );
	CoolTimeEffect( FALSE );

	SetColor( SKILLWND_ITEM_NOT_LEARN_COLOR_RED, SKILLWND_ITEM_NOT_LEARN_COLOR_GREEN, SKILLWND_ITEM_NOT_LEARN_COLOR_BLUE );		

	if( m_pItem->GetSkillType() == CSkillCustomizeSkillItem::SKILL )
	{
		sSKILL_TBLDAT* pSkillTableData = reinterpret_cast<sSKILL_TBLDAT*>( m_pItem->GetSkillData() );
		if( pSkillTableData->bDefaultDisplayOff )
			Show( false );
		else
			Show( true );
	}
}

VOID CSkillCustomizeItem::SetSobIcon( CNtlSobIcon* pSobIcon, sTBLDAT* pData )
{
	// 아이템 아이콘 세팅, 테이블 세팅.
	m_pSobIcon = pSobIcon;
	m_pItem->SetSkillData( pData );

	// 기존 아이콘 텍스쳐 해제 및 아이콘의 텍스쳐로 교체.
	m_pItem->SetIconTexture( reinterpret_cast<gui::CTexture*>( pSobIcon->GetImage() ) );

	// 쿨타임 체크.
	if( m_pSobIcon->GetIconState() == CNtlSobIcon::ICON_STATE_COOLING )
		CoolTimeEffect( TRUE );	

	// 이미지 
	SetActive( TRUE );
	m_pItem->SetLineActive( TRUE, LINE_TYPE_OPTION );	

	// RPButton 활성화
	if( m_pItem->GetRPButton() )
		m_pItem->GetRPButton()->ClickEnable( TRUE );	

	// RPMode 갱신
	if( m_pItem->GetSkillType() == CSkillCustomizeSkillItem::SKILL )
	{
		CNtlSobSkillAttr* pSobAttr = reinterpret_cast<CNtlSobSkillAttr*>( m_pSobIcon->GetSobObj()->GetSobAttr() );
		SetRPType( pSobAttr );
	}

	SetColor( SKILLWND_ITEM_LEARN_COLOR_RED, SKILLWND_ITEM_LEARN_COLOR_GREEN, SKILLWND_ITEM_LEARN_COLOR_BLUE );

	// SP 버튼 체크, 업그레이드 스킬 라인표시는 다른 함수에서 일괄적으로 호출되게 된다.

	// 기존 인포윈도우 체크
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_SKILL )
	{
		if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_SKILL_SPUPGRADE )
		{
			CheckSkillUpgrade();	// 나중에 한번 더 불려지겠지만 ShowSPInfownd 되기전에 수행되어야 한다. 
			ShowSPInfownd( TRUE );		
		}
		else
			ShowSkillInfownd( TRUE );
	}

	// 안보이던 스킬의 경우 보여야함
	Show( true );
}

VOID CSkillCustomizeItem::SetRPType( CNtlSobSkillAttr* pSobSkillAttr )
{
	if( pSobSkillAttr->m_bRPBonusAutoMode )
	{
		CRectangle rtScreen = m_pItem->GetSkillIcon()->GetScreenRect();
		m_surRPType.SetSurface( Logic_GetSmallRPTypeMark( pSobSkillAttr->m_byRPBonusType ) );
		m_surRPType.SetRectWH( rtScreen.left, rtScreen.top, SKILLWND_ITEM_RPTYPE_WIDTH, SKILLWND_ITEM_RPTYPE_HEIGHT );
		m_surRPType.Show( TRUE );
	}
	else
	{
		m_surRPType.Show( FALSE );
	}
}

VOID CSkillCustomizeItem::CheckSkillUpgrade(VOID)
{
	if( m_pItem->GetSkillType() != CSkillCustomizeSkillItem::SKILL )
		return;

	// 업그레이드 가능하거나, 셀프 트레인이 아닌 경우 리턴.
	if( !m_pItem->GetSPButton() )
		return;

	// 데이터 초기화
	m_stSkillUpgradeInfo.Init();

	// SP 버튼 활성화 여부.
	// 0. 마스터 했는가.
	// 1. SP가 충분한가.
	// 2. 배워야할 스킬을 배우고 있는가.
	// 3. 배웠는가.
	// 4. 	

	RwBool bActivateSPBtn = TRUE;
	CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
	sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( m_pItem->GetSkillData() );
	sSKILL_TBLDAT* pNextSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pSkillData->dwNextSkillTblidx ) );
	sSKILL_TBLDAT* pTestTblData = NULL;

	m_stSkillUpgradeInfo.byCurrentLevel = pSkillData->bySkill_Grade;
	m_stSkillUpgradeInfo.bSelfLearnEnable = pSkillData->bSelfTrain;

	if( m_pSobIcon )
	{
		m_stSkillUpgradeInfo.bLearn = TRUE;

		if( pSkillData->dwNextSkillTblidx == INVALID_TBLIDX )
		{
			m_stSkillUpgradeInfo.bMaster = TRUE;		

			// 결과 적용.
			m_pItem->GetSPButton()->ClickEnable( FALSE );	

			return;
		}
		else
		{
			//	m_stSkillUpgradeInfo.bMaster = FALSE;
			m_stSkillUpgradeInfo.uiNeedSP = pNextSkillData->wRequireSP;
			m_stSkillUpgradeInfo.RequireSkillIdx1 = pNextSkillData->uiRequire_Skill_Tblidx_Min_1;
			m_stSkillUpgradeInfo.RequireSkillIdx2 = pNextSkillData->uiRequire_Skill_Tblidx_Min_2;
			m_stSkillUpgradeInfo.byRequreAvatarLevel = pNextSkillData->byRequire_Train_Level;		
			pTestTblData = pNextSkillData;
		}
	}
	else
	{
		//m_stSkillUpgradeInfo.bLearn = FALSE;		

		if( !pSkillData->bSelfTrain )
			bActivateSPBtn = FALSE;

		m_stSkillUpgradeInfo.uiNeedSP = pSkillData->wRequireSP;
		m_stSkillUpgradeInfo.RequireSkillIdx1 = pSkillData->uiRequire_Skill_Tblidx_Min_1;
		m_stSkillUpgradeInfo.RequireSkillIdx2 = pSkillData->uiRequire_Skill_Tblidx_Min_2;
		m_stSkillUpgradeInfo.byRequreAvatarLevel = pSkillData->byRequire_Train_Level;	
		pTestTblData = pSkillData;
	}

	// SP 검사.
	if( pTestTblData->wRequireSP > Logic_GetSp() )
	{
		bActivateSPBtn = FALSE;
		m_stSkillUpgradeInfo.bSPOK = FALSE;
	}
	else
		m_stSkillUpgradeInfo.bSPOK = TRUE;

	// 스킬 검사
	CNtlSkillContainer* pSkillContainer = GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer();

	if (pTestTblData->uiRequire_Skill_Tblidx_Min_1 == INVALID_TBLIDX ||
		pTestTblData->uiRequire_Skill_Tblidx_Max_1 == INVALID_TBLIDX)
	{
		m_stSkillUpgradeInfo.bSkill1OK = TRUE;
	}
	else
	{
		if (pSkillContainer->SkillExists(pTestTblData->uiRequire_Skill_Tblidx_Min_1, pTestTblData->uiRequire_Skill_Tblidx_Max_1))
		{
			m_stSkillUpgradeInfo.bSkill1OK = TRUE;
		}
		else
		{
			bActivateSPBtn = FALSE;
			//m_stSkillUpgradeInfo.bSkill1OK = FALSE;
		}			
	}

	if( pTestTblData->uiRequire_Skill_Tblidx_Min_2 == INVALID_TBLIDX ||
		pTestTblData->uiRequire_Skill_Tblidx_Max_2 == INVALID_TBLIDX )
		m_stSkillUpgradeInfo.bSkill2OK = TRUE;			
	else
	{
		if( pSkillContainer->SkillExists( pTestTblData->uiRequire_Skill_Tblidx_Min_2, pTestTblData->uiRequire_Skill_Tblidx_Max_2 ) )
			m_stSkillUpgradeInfo.bSkill2OK = TRUE;
		else
		{
			bActivateSPBtn = FALSE;
			//m_stSkillUpgradeInfo.bSkill2OK = FALSE;
		}			
	}

	// 레벨 검사.
	if( pTestTblData->byRequire_Train_Level > Logic_GetLevel( GetNtlSLGlobal()->GetSobAvatar() ) )
	{
		//m_stSkillUpgradeInfo.bAvatarLevelOK = FALSE;
		bActivateSPBtn = FALSE;
	}
	else
	{
		m_stSkillUpgradeInfo.bAvatarLevelOK = TRUE;
	}

	// 클래스 검사
	CNtlSobAvatarAttr* pAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );

	if( !( pAttr->GetRaceFlag() & pTestTblData->dwPC_Class_Bit_Flag ) )
	{
		m_stSkillUpgradeInfo.bClassOK = FALSE;
		bActivateSPBtn = FALSE;
	}

	// 마스터리 검사
	if( pTestTblData->byPC_Class_Change != INVALID_BYTE ) 
	{
		if( Logic_IsFirstClass( GetNtlSLGlobal()->GetSobAvatar() ) )
		{
			if( pAttr->m_bCanChangeClass )
				m_stSkillUpgradeInfo.bMasteryOK = TRUE;
			else
			{
				m_stSkillUpgradeInfo.bMasteryOK = FALSE;
				bActivateSPBtn = FALSE;
			}
		}		
		else
		{
			if( pTestTblData->byPC_Class_Change != pAttr->GetPcTbl()->byClass )
			{
				m_stSkillUpgradeInfo.bClassOK = FALSE;
				bActivateSPBtn = FALSE;
			}
		}
	}	

	// 결과 적용.
	if( bActivateSPBtn )
	{
		m_pItem->GetSPButton()->ClickEnable( TRUE );	
		m_pItem->GetSPButton()->SetColor( SKILLWND_ITEM_LEARN_COLOR_RED, SKILLWND_ITEM_LEARN_COLOR_GREEN, SKILLWND_ITEM_LEARN_COLOR_BLUE );

		if (!m_pSobIcon) // check if not learned
		{
			SetLearnAbleSkillBack(true);
		}
		else // else - learned
		{
			SetLearnAbleSkillBack(false);
		}
	}
	else
	{
		SetLearnAbleSkillBack(false);

		m_pItem->GetSPButton()->ClickEnable( FALSE );	

		if( m_pSobIcon )
			m_pItem->GetSPButton()->SetColor( SKILLWND_ITEM_LEARN_COLOR_RED, SKILLWND_ITEM_LEARN_COLOR_GREEN, SKILLWND_ITEM_LEARN_COLOR_BLUE );
		else
			m_pItem->GetSPButton()->SetColor( SKILLWND_ITEM_NOT_LEARN_COLOR_RED, SKILLWND_ITEM_NOT_LEARN_COLOR_GREEN, SKILLWND_ITEM_NOT_LEARN_COLOR_BLUE );			
	}	

	// Apply upgrade line
	if( SkillCommonLogic::IsLearnable( CSkillGuiItem::SKILL, pSkillData ) )
		m_pItem->SetLineActive( TRUE, LINE_TYPE_UPGRADE );
}

VOID CSkillCustomizeItem::Show( bool bShow )
{
	m_pItem->Show( bShow );
}

VOID CSkillCustomizeItem::SetAlpha( RwUInt8 byAlpha )
{
	m_pItem->SetAlpha( byAlpha );
}

VOID CSkillCustomizeItem::SetColor( RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue )
{
	m_pItem->SetColor( byRed, byGreen, byBlue );

	m_surRPType.SetColorOnly( byRed, byGreen, byBlue );
	m_surFocusEffect.SetColorOnly( byRed, byGreen, byBlue );
	m_surPickedUp.SetColorOnly( byRed, byGreen, byBlue );	
}

VOID CSkillCustomizeItem::SetActive( RwBool bActive )
{
	m_pItem->SetActive( bActive );
}

VOID CSkillCustomizeItem::CoolTimeEffect( RwBool bStart )
{
	if( m_pSobIcon && bStart )
		m_CoolTimeEffect.StartProc( m_pSobIcon->GetMaxCoolingTime() );
	else
		m_CoolTimeEffect.EndProc();
}

void CSkillCustomizeItem::SelectEffect(bool bStart)
{
	if (bStart)
	{
		if (m_pflaSelectEffect == NULL)
		{
			CRectangle rect;
			rect.SetRectWH(-1, -1, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
			m_pflaSelectEffect = NTL_NEW gui::CFlash(rect, m_pItem->GetSkillIcon(), GetNtlGuiManager()->GetSurfaceManager(), "Skill_action.swf");
			m_pflaSelectEffect->PlayMovie(TRUE);
			m_pflaSelectEffect->Show(true);
			m_pflaSelectEffect->Enable(true);
		}
		else
		{
			DBO_WARNING_MESSAGE("Select effect already exist");
		}
	}
	else
	{
		if (m_pflaSelectEffect)
		{
			m_pflaSelectEffect->PlayMovie(FALSE);
			m_pflaSelectEffect->Show(false);
			NTL_DELETE(m_pflaSelectEffect);
		}
	}
}

VOID CSkillCustomizeItem::ClickEffect( RwBool bPush, RwBool bStillPush /* = TRUE  */ )
{
	// Icon Surface는 하나뿐.
	CRectangle rtScreen = m_pItem->GetSkillIcon()->GetScreenRect();
	gui::CSurface* pSurface = &(m_pItem->GetSkillIcon()->GetSurface()->front());
	CRectangle* rtSnapShot = &pSurface->m_SnapShot.rtRect;

	if( bPush )
	{
		pSurface->m_SnapShot.rtRect.SetRect( rtSnapShot->left + ICONPUSH_SIZEDIFF, rtSnapShot->top + ICONPUSH_SIZEDIFF,
			rtSnapShot->right - ICONPUSH_SIZEDIFF, rtSnapShot->bottom - ICONPUSH_SIZEDIFF );
	}
	else
	{
		pSurface->m_SnapShot.rtRect.SetRectWH( rtScreen.left, rtScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	}

	m_bPushDownIcon = bStillPush;
}

VOID CSkillCustomizeItem::ShowPickedUp( RwBool bShow )
{
	m_surPickedUp.Show( bShow );	
}

RwInt32 CSkillCustomizeItem::GetBottomPos(VOID)
{
	if( m_pItem->IsShow() )
		return m_pItem->GetBottomPos();	

	return 0;
}

/// woosungs_test 20090730
RwBool CSkillCustomizeItem::IsUpgradeble()
{
	if( m_pItem && m_pItem->GetSPButton() )
	{
		CheckSkillUpgrade();

		return m_pItem->GetSPButton()->IsClickEnable();
	}

	return FALSE;
}
/// woosungs_test 20090730
RwBool CSkillCustomizeItem::IsTrainableSkill()
{
	if( m_pItem )
	{
		sTBLDAT* pItemTblData = m_pItem->GetSkillData();

		if( false == ((sSKILL_TBLDAT*)pItemTblData)->bSelfTrain )	/// Master에게 배우는 스킬만 검사
			if( SkillCommonLogic::IsLearnable( (CSkillGuiItem::TYPE) m_pItem->GetSkillType(), pItemTblData ) )
			{
				return TRUE;
			}
	}

	return FALSE;
}
/// woosungs_test 20090730
RwBool CSkillCustomizeItem::IsSelfTrainableSkill()
{
	if( m_pItem )
	{
		sTBLDAT* pItemTblData = m_pItem->GetSkillData();

		if( true == ((sSKILL_TBLDAT*)pItemTblData)->bSelfTrain )	/// Self Trainning 스킬만 검사
			if( SkillCommonLogic::IsLearnable( (CSkillGuiItem::TYPE) m_pItem->GetSkillType(), m_pItem->GetSkillData() ) )
			{
				return TRUE;
			}
	}

	return FALSE;
}

void CSkillCustomizeItem::SetLearnAbleSkillBack(bool bActive)
{
	if (bActive)
	{
		m_surLearnAbleEffect.Show(TRUE);
	//	OnIconMove(0,0); // required to show up the surface once we level a skill
	}
	else
	{
		m_surLearnAbleEffect.Show(FALSE);
	}
}

bool CSkillCustomizeItem::CanSelect()
{
	if (m_pItem->GetSkillType() == CSkillCustomizeSkillItem::TYPE::SKILL)
	{
		sSKILL_TBLDAT* pSkillTableData = reinterpret_cast<sSKILL_TBLDAT*>(m_pItem->GetSkillData());
		if (!pSkillTableData)
			return false;

		if (pSkillTableData->bySkill_Class == NTL_SKILL_CLASS_PASSIVE)
			return false;
	}

	return true;
}

VOID CSkillCustomizeItem::ShowSkillInfownd( RwBool bShow )
{
	if( bShow )
	{
		CInfoWndManager::eINFOWNDSTATE eInfoState = CInfoWndManager::INFOWND_NOTSHOW;
		CRectangle rtScreen = m_pItem->GetSkillIcon()->GetScreenRect();

		if( m_pSobIcon )
		{
			switch( m_pItem->GetSkillType() )
			{
			case CSkillCustomizeSkillItem::SKILL:	eInfoState = CInfoWndManager::INFOWND_SKILL; break;
			case CSkillCustomizeSkillItem::HTB:		eInfoState = CInfoWndManager::INFOWND_HTBSKILL; break;
			case CSkillCustomizeSkillItem::ACTION:	eInfoState = CInfoWndManager::INFOWND_ACTIONSKILL; break;
			}

			GetInfoWndManager()->ShowInfoWindow( TRUE, eInfoState, rtScreen.left, rtScreen.top, m_pSobIcon->GetSobObj(), DIALOG_SKILL );
		}
		else
		{
			switch( m_pItem->GetSkillType() )
			{
			case CSkillCustomizeSkillItem::SKILL: eInfoState = CInfoWndManager::INFOWND_NOTLEARN_SKILL; break;
			case CSkillCustomizeSkillItem::HTB:   eInfoState = CInfoWndManager::INFOWND_NOTLEARN_HTBSKILL; break;
			}

			GetInfoWndManager()->ShowInfoWindow( TRUE, eInfoState, rtScreen.left, rtScreen.top, m_pItem->GetSkillData(), DIALOG_SKILL );
		}			
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		//	m_pItem->GetSkillIcon()->ReleaseMouse();		
	}
}

VOID CSkillCustomizeItem::ShowSPInfownd( RwBool bShow )
{
	if( bShow )
	{
		if( m_pItem->GetSPButton() )
		{
			CRectangle rtScreen = m_pItem->GetSPButton()->GetScreenRect();
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_SKILL_SPUPGRADE, rtScreen.left, rtScreen.top, (VOID*)&m_stSkillUpgradeInfo, DIALOG_SKILL );
		}		
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );		
	}
}

VOID CSkillCustomizeItem::OnIconMouseDown( const CKey& key )
{
	if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( m_pSobIcon ) 
		{
			if (m_pSobIcon->IsUsePossible())
			{
				m_eRClickIcon = STATE_CLICK;

				if (!GetIconMoveManager()->IsActive())
				{
					ClickEffect(TRUE);
				}
			}
			else
			{

			}
		}	
	}
	else if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( m_pSobIcon )
		{
		//	if (m_pSobIcon->IsUsePossible()) // if we have this check, we cant select skills while sitting
			if(CanSelect())
			{
				m_eLClickIcon = STATE_CLICK;

				if (!GetIconMoveManager()->IsActive())
				{
					ClickEffect(TRUE);
				}
			}
		}
	}

	m_pItem->GetSkillIcon()->CaptureMouse();

	if( m_eRClickIcon && m_eLClickIcon )
	{
		m_eLClickIcon = STATE_NONE;
		m_eRClickIcon = STATE_NONE;
		ClickEffect( FALSE, FALSE );
		m_pItem->GetSkillIcon()->ReleaseMouse();
	}
}

VOID CSkillCustomizeItem::OnIconMouseUp( const CKey& key )
{
	ClickEffect( FALSE, FALSE );
	m_pItem->GetSkillIcon()->ReleaseMouse();

	if( !m_pItem->GetSkillIcon()->IsVisibleTruly() )
	{
		m_eLClickIcon = STATE_NONE;
		m_eRClickIcon = STATE_NONE;
		return;
	}

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( m_eLClickIcon == STATE_CLICK && m_pSobIcon )
		{
			if( GetIconMoveManager()->IsActive() )
			{
				if( GetIconMoveManager()->GetSrcSerial() == m_pSobIcon->GetSobObj()->GetSerialID() )
					GetIconMoveManager()->IconMoveEnd();
			}
			else
			{
				if (GetIconMoveManager()->IconMovePickUp(m_pSobIcon->GetSobObj()->GetSerialID(), PLACE_SKILL, 0, 0, m_pSobIcon->GetImage()))
				{
					ShowPickedUp(TRUE);
				}
			}
		}	

		m_eLClickIcon = STATE_NONE;
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( m_eRClickIcon == STATE_CLICK && m_pSobIcon )
		{
			if (!GetIconMoveManager()->IsActive() && !GetDialogManager()->IsOpenDialog(DIALOG_SKILL_RPBONUS))
			{
				Logic_UseProc(m_pSobIcon->GetSobObj()->GetSerialID());
			}
		}

		m_eRClickIcon = STATE_NONE;		
		m_fRightClickTime = 0.0f;
	}
}

VOID CSkillCustomizeItem::OnIconMouseEnter( gui::CComponent* pComponent )
{
	ShowSkillInfownd( TRUE );

	// 왜 들어가는지 테스트.
	if( m_bPushDownIcon )
		ClickEffect( TRUE );

	m_surFocusEffect.Show( TRUE );

	if( m_eLClickIcon == STATE_CLICK_BUT_OUTSIDE )
		m_eLClickIcon = STATE_CLICK;
	if( m_eRClickIcon == STATE_CLICK_BUT_OUTSIDE )
		m_eRClickIcon = STATE_CLICK;
}

VOID CSkillCustomizeItem::OnIconMouseLeave( gui::CComponent* pComponent )
{
	ShowSkillInfownd( FALSE );

	if( m_bPushDownIcon )
		ClickEffect( FALSE, m_bPushDownIcon );

	m_surFocusEffect.Show( FALSE );

	if( m_eLClickIcon == STATE_CLICK )
		m_eLClickIcon = STATE_CLICK_BUT_OUTSIDE;
	if( m_eRClickIcon == STATE_CLICK )
		m_eRClickIcon = STATE_CLICK_BUT_OUTSIDE;
}

VOID CSkillCustomizeItem::OnIconPaint(VOID)
{
	m_surRPType.Render();
	m_surPickedUp.Render();
	m_surFocusEffect.Render();
	m_CoolTimeEffect.Render();

	m_surLearnAbleEffect.Render();
}

VOID CSkillCustomizeItem::OnIconMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_pItem->GetSkillIcon()->GetScreenRect();
	m_surRPType.SetPositionbyParent( rtScreen.left, rtScreen.top );
	m_surFocusEffect.SetPosition( rtScreen.left, rtScreen.top );
	m_surPickedUp.SetPosition( rtScreen.left, rtScreen.top );
	m_CoolTimeEffect.SetPosition( rtScreen.left, rtScreen.top );

	CRectangle rtThisScreen = m_pItem->GetSkillScreenRect();
	m_surLearnAbleEffect.SetPosition(rtThisScreen.left, rtThisScreen.top);

	CRectangle* rtClipping = m_pItem->GetSkillIcon()->GetClippingRect();
	m_surRPType.SetClippingRect( *rtClipping );
	m_surFocusEffect.SetClippingRect( *rtClipping );
	m_surPickedUp.SetClippingRect( *rtClipping );
	m_CoolTimeEffect.SetClippingRect( *rtClipping );

	CRectangle* rtItemClipping = m_pItem->GetSkillRectangle();
	m_surLearnAbleEffect.SetClippingRect(*rtItemClipping);
}

VOID CSkillCustomizeItem::OnSPButtonEnter( gui::CComponent* pComponent )
{
	ShowSPInfownd( TRUE );
}

VOID CSkillCustomizeItem::OnSPButtonLeave( gui::CComponent* pComponent )
{
	ShowSPInfownd( FALSE );	
}

VOID CSkillCustomizeItem::OnSPButtonClick( gui::CComponent* pComponent )
{
	if( m_pSobIcon )
	{// 스킬 업그레이드 하는 경우.
		CNtlSkillContainer* pSkillContainer = GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer();
		RwInt32 nSlotIdx = pSkillContainer->GetSkillSlotIdx( m_pSobIcon->GetSobObj()->GetSerialID() );
		NTL_ASSERT( nSlotIdx >= 0, "CSkillWindowItemDlg::OnUpgradeButtonClick : Invalid Skill Object" );

		GetDboGlobal()->GetGamePacketGenerator()->SendSkillUpgradeReq( (RwUInt8)nSlotIdx );	
	}	
	else
	{// SelfTrain 스킬을 배우는 경우.
		GetDboGlobal()->GetGamePacketGenerator()->SendSkillLearnReq( m_pItem->GetSkillData()->tblidx );
	}
}

VOID CSkillCustomizeItem::OnRPButtonClick( gui::CComponent* pComponent )
{
	if( m_pSobIcon )
	{
		CRectangle rtScreen = m_pItem->GetRPButton()->GetScreenRect();
		CDboEventGenerator::RpBonusSetup( rtScreen.right, rtScreen.bottom, m_pSobIcon );
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSkillCustomizeItemGroup

CSkillCustomizeItemGroup::CSkillCustomizeItemGroup( gui::CComponent* pParent, gui::CScrollBar* pScrollBar )
: m_pParser( NULL ), m_pDialog( NULL ), m_flagClass( 0 ), m_pScrollBar( pScrollBar )
{
	m_rtOriginal.SetRectWH( 0, 0, pParent->GetWidth(), pParent->GetHeight() );

	m_pDialog = NTL_NEW gui::CDialog( m_rtOriginal, pParent, GetNtlGuiManager()->GetSurfaceManager() );
	m_pDialog->SetClippingMode( TRUE );
}

CSkillCustomizeItemGroup::~CSkillCustomizeItemGroup(VOID)
{	
	ClearSkillItem();
	NTL_DELETE( m_pDialog );
}

VOID CSkillCustomizeItemGroup::GenerateSkillItem( const RwChar* szFileName, RwUInt32 flagClass )
{
	m_flagClass = flagClass;

	std::string strPath = "gui\\skill\\";
	strPath += szFileName;

	m_pParser = NTL_NEW CSkillCustomizeParser( strPath.c_str() );
	m_pParser->GenerateGuiItem( m_pDialog );

	std::string strError;
	m_pParser->ReadError( strError );
	NTL_ASSERT( strError.empty(), "CSkillCustomizeItemGroup Construcor : Parse Error File : " << szFileName );

	CSkillCustomizeParser::MAP_SKILLITEM& mapParsedSkill = m_pParser->GetComponentMap();
	CSkillCustomizeParser::MAP_SKILLITEM_ITER iter;

	for( iter = mapParsedSkill.begin() ; iter != mapParsedSkill.end() ; ++iter )
	{
		CSkillCustomizeSkillItem* pParsedItem = iter->second;

		m_mapSkillItem[pParsedItem->GetSkillData()->tblidx] = NTL_NEW CSkillCustomizeItem( pParsedItem );
	}

	CheckSkillGroupShowAndSize();
}

VOID CSkillCustomizeItemGroup::ClearSkillItem(VOID)
{
	MAP_SKILL_ITEM_ITER iter;

	for( iter = m_mapSkillItem.begin() ; iter != m_mapSkillItem.end() ; ++iter )
	{
		CSkillCustomizeItem* pItem = iter->second;
		NTL_DELETE( pItem );
	}

	m_mapSkillItem.clear();

	NTL_DELETE( m_pParser );
}

VOID CSkillCustomizeItemGroup::Update( RwReal fElapsed )
{
	MAP_SKILL_ITEM_ITER iter;

	for( iter = m_mapSkillItem.begin() ; iter != m_mapSkillItem.end() ; ++iter )
	{
		CSkillCustomizeItem* pItem = iter->second;
		pItem->Update( fElapsed );
	}
}

VOID CSkillCustomizeItemGroup::Show( bool bShow )
{
	m_pDialog->Show( bShow );
}

VOID CSkillCustomizeItemGroup::SetInitSkill(VOID)
{
	MAP_SKILL_ITEM_ITER iter;

	for( iter = m_mapSkillItem.begin() ; iter != m_mapSkillItem.end() ; ++iter )
	{
		CSkillCustomizeItem* pItem = iter->second;
		pItem->SetInitSkill();
	}

	m_pDialog->SetPosition( m_rtOriginal );
	CheckSkillGroupShowAndSize();
}

VOID CSkillCustomizeItemGroup::SetSkillItem( RwUInt32 uiBaseSkillIndex, CNtlSobIcon* pSobIcon, sTBLDAT* pData )
{
	MAP_SKILL_ITEM_ITER iter;
	iter = m_mapSkillItem.find( uiBaseSkillIndex );

	if( iter != m_mapSkillItem.end() )
	{
		CSkillCustomizeItem* pItem = iter->second;
		pItem->SetSobIcon( pSobIcon, pData );

		CheckSkillGroupShowAndSize();
	}
}

VOID CSkillCustomizeItemGroup::SetRPType( RwUInt32 uiBaseSkillIndex, CNtlSobSkillAttr* pSobSkillAttr )
{
	MAP_SKILL_ITEM_ITER iter;
	iter = m_mapSkillItem.find( uiBaseSkillIndex );

	if( iter != m_mapSkillItem.end() )
	{
		CSkillCustomizeItem* pItem = iter->second;
		pItem->SetRPType( pSobSkillAttr );
	}
}

VOID CSkillCustomizeItemGroup::CheckUpgradebleSkill(VOID)
{
	MAP_SKILL_ITEM_ITER iter;

	for( iter = m_mapSkillItem.begin() ; iter != m_mapSkillItem.end() ; ++iter )
	{
		CSkillCustomizeItem* pItem = iter->second;
		pItem->CheckSkillUpgrade();
	}

	// 조건 만족시 보여져야 하는 녀석이 있다면 이 밑에 주석을 푼다.
	// CheckSkillGroupShowAndSize();
}

VOID CSkillCustomizeItemGroup::CheckSkillGroupShowAndSize(VOID)
{
	MAP_SKILL_ITEM_ITER iter;
	RwInt32 nBaseCY = m_pDialog->GetHeight();
	RwInt32 nCurrentCY = 0;

	for( iter = m_mapSkillItem.begin() ; iter != m_mapSkillItem.end() ; ++iter )
	{
		CSkillCustomizeItem* pItem = iter->second;
		RwInt32 nBottomPos = pItem->GetBottomPos();

		if( nBottomPos > nCurrentCY )
			nCurrentCY = nBottomPos;
	}

	if( nCurrentCY > nBaseCY )		
	{
		m_pDialog->SetHeight( nCurrentCY + 15 );

		if( m_pDialog->IsVisibleTruly() )
			CalcScrollBar();
	}
}

VOID CSkillCustomizeItemGroup::SetOffset( RwInt32 nYOffset )
{
	CRectangle rtPos = m_pDialog->GetPosition();
	m_pDialog->SetPosition( rtPos.left, -nYOffset );
}

VOID CSkillCustomizeItemGroup::CalcScrollBar(VOID)
{
	m_pScrollBar->SetRange( 0, m_pDialog->GetHeight() - m_pDialog->GetParent()->GetHeight() );
	m_pScrollBar->SetValue( -m_pDialog->GetPosition().top );
}

VOID CSkillCustomizeItemGroup::HidePickedUp(VOID)
{
	MAP_SKILL_ITEM_ITER iter;

	for( iter = m_mapSkillItem.begin() ; iter != m_mapSkillItem.end() ; ++iter )
	{
		CSkillCustomizeItem* pItem = iter->second;
		pItem->ShowPickedUp( FALSE );
	}
}

VOID CSkillCustomizeItemGroup::CoolingEffectProc( RwUInt32 uiBasetblidx, RwBool bStart )
{
	MAP_SKILL_ITEM_ITER iter;
	iter = m_mapSkillItem.find( uiBasetblidx );

	if( iter != m_mapSkillItem.end() )
	{
		CSkillCustomizeItem* pItem = iter->second;
		pItem->CoolTimeEffect( bStart );
	}
}

void CSkillCustomizeItemGroup::SelectEffectProc(RwUInt32 uiBasetblidx, bool bStart)
{
	MAP_SKILL_ITEM_ITER iter;
	iter = m_mapSkillItem.find(uiBasetblidx);

	if (iter != m_mapSkillItem.end())
	{
		CSkillCustomizeItem* pItem = iter->second;
		pItem->SelectEffect(bStart);
	}
}

/// woosungs_test 20090730
RwBool CSkillCustomizeItemGroup::IsExistUpgradebleSkill()
{
	MAP_SKILL_ITEM_ITER iter;

	for( iter = m_mapSkillItem.begin() ; iter != m_mapSkillItem.end() ; ++iter )
	{
		CSkillCustomizeItem* pItem = iter->second;

		if(pItem->IsUpgradeble())			
			return true;
	}

	return false;
}

RwBool CSkillCustomizeItemGroup::IsExistTrainableSkill()
{
	MAP_SKILL_ITEM_ITER iter;

	for( iter = m_mapSkillItem.begin() ; iter != m_mapSkillItem.end() ; ++iter )
	{
		CSkillCustomizeItem* pItem = iter->second;

		if(pItem->IsTrainableSkill())			
			return true;
	}

	return false;
}

RwBool CSkillCustomizeItemGroup::IsExistSelfTrainableSkill()
{
	MAP_SKILL_ITEM_ITER iter;

	for( iter = m_mapSkillItem.begin() ; iter != m_mapSkillItem.end() ; ++iter )
	{
		CSkillCustomizeItem* pItem = iter->second;

		if(pItem->IsSelfTrainableSkill())			
			return true;
	}

	return false;
}