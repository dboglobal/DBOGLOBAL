#include "precomp_dboclient.h"
#include "ScrambleNotifyGui.h"


// shared
#include "DojoTable.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlDTCinematicManager.h"

//client
#include "DboGlobal.h"
#include "DialogPriority.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "ResultNarrationGui.h"


#define dCINEMATIC_GUI_KEYWARD_NAME		"DojoTeamInfoGui"


#define dFLASH_SCRAMBLE_TITLE			"Dojo_Scramble_Title.swf"
#define dFLASH_SCRAMBLE_START			"Dojo_Scramble_Start.swf"
#define dFLASH_SCRAMBLE_DEFENCE_FAIL	"Dojo_Seal_Defence_Fail.swf"
#define dFLASH_SCRAMBLE_OFFENCE_SUCCESS	"Dojo_Seal_Offence_Success.swf"
#define dFLASH_SCRAMBLE_WARNING			"Dojo_Screamble_Warnning.swf"
#define dFLASH_SCRAMBLE_RESULT			"Dojo_Scramble_Conclusion.swf"
#define dFLASH_SCRAMBLE_REWARD			"Dojo_Scramble_Result.swf"
#define dFLASH_SCRAMBLE_REWARD_NPC		"Dojo_Scramble_Result_npc.swf"

#define dWARNING_REST_TIME					(180.f)			///< 경고를 하기 위한 최소 휴지 시간(단위 : 초)
#define dWARNING_POINT_GAP					(90)			///< 지고 있을 경우 경고를 하기 위한 점수차
#define dWARNING_BE_STEALED_SEAL_COUNT		(5)				///< 인장을 뺏겼을 경우 경고를 하기 위한 최소 갯수


CScrambleNotifyGui::CScrambleNotifyGui(const RwChar* pName)
:CNtlPLGui(pName)
{
	m_tREWARD_WIDGET.pResultNarration = NULL;
}

CScrambleNotifyGui::~CScrambleNotifyGui()
{
}

RwBool CScrambleNotifyGui::Create()
{
	NTL_FUNCTION( "CScrambleNotifyGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\ScrambleNotify.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_NOTIFY);

	// notify
	pFlash_Notify			= (gui::CFlash*)GetComponent("flashNotify");
	pFlash_Notify2			= (gui::CFlash*)GetComponent("flashNotify2");

	m_slotMovieEnd_Notify		= pFlash_Notify->SigMovieEnd().Connect( this, &CScrambleNotifyGui::OnMovieEnd_Notify );

	// StartWidget
	m_tSTART_WIDGET.pParentDialog		= (gui::CDialog*)GetComponent("dlgStart");
	m_tSTART_WIDGET.pFlash				= (gui::CFlash*)GetComponent("flash");
	m_tSTART_WIDGET.pOffenceGuildName	= (gui::CStaticBox*)GetComponent("stbStart_OffenceGuildName");
	m_tSTART_WIDGET.pDefenceGuildName	= (gui::CStaticBox*)GetComponent("stbStart_DefenceGuildName");
	m_tSTART_WIDGET.pRuleTitle			= (gui::CStaticBox*)GetComponent("stbStart_RuleTitle");
	m_tSTART_WIDGET.pRule				= (gui::CHtmlBox*)GetComponent("hbxStart_Rule");

	m_tSTART_WIDGET.pOffenceGuildName->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_tSTART_WIDGET.pDefenceGuildName->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_tSTART_WIDGET.pRuleTitle		->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);

	m_tSTART_WIDGET.pRule->SetLineSpace(8);

	m_tSTART_WIDGET.slotFSCallback	= m_tSTART_WIDGET.pFlash->SigFSCallBack().Connect( this, &CScrambleNotifyGui::OnFSCallback_StartWidget );
	m_tSTART_WIDGET.slotMovieEnd	= m_tSTART_WIDGET.pFlash->SigMovieEnd().Connect( this, &CScrambleNotifyGui::OnMovieEnd_Notify );


	// REWARD_WIDGET
	m_tREWARD_WIDGET.pParentDialog				= (gui::CDialog*)GetComponent("dlgReward");
	m_tREWARD_WIDGET.pFlash						= (gui::CFlash*)GetComponent("flashReward");
	m_tREWARD_WIDGET.pGuildNameStatic			= (gui::CStaticBox*)GetComponent("stbGuildNameStatic");
	m_tREWARD_WIDGET.pDojoNameStatic			= (gui::CStaticBox*)GetComponent("stbDojoNameStatic");
	m_tREWARD_WIDGET.pRewardReputationPointStatic= (gui::CStaticBox*)GetComponent("stbRewardReputationStatic");
	m_tREWARD_WIDGET.pRewardItemStatic			= (gui::CStaticBox*)GetComponent("stbRewardItemStatic");
	m_tREWARD_WIDGET.pGuildName					= (gui::CStaticBox*)GetComponent("stbGuildName");
	m_tREWARD_WIDGET.pDojoName					= (gui::CStaticBox*)GetComponent("stbDojoName");
	m_tREWARD_WIDGET.pRewardReputationPoint		= (gui::CStaticBox*)GetComponent("stbRewardReputation");
	m_tREWARD_WIDGET.pRewardItem				= (gui::CStaticBox*)GetComponent("stbRewardItem");

	m_tREWARD_WIDGET.bShowRewardItem = FALSE;

	m_tREWARD_WIDGET.srfRewardItem.Create(m_pThis, DIALOG_SCRAMBLE_NOTIFY, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
	m_tREWARD_WIDGET.srfRewardItem.SetPosition_fromParent(495, 389);

	m_tREWARD_WIDGET.pGuildNameStatic			->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_tREWARD_WIDGET.pDojoNameStatic			->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_tREWARD_WIDGET.pRewardReputationPointStatic->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_tREWARD_WIDGET.pRewardItemStatic			->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_tREWARD_WIDGET.pGuildName					->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_tREWARD_WIDGET.pDojoName					->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_tREWARD_WIDGET.pRewardReputationPoint		->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_tREWARD_WIDGET.pRewardItem				->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);

	m_tREWARD_WIDGET.pGuildNameStatic				->SetTextColor(RGB(255, 255, 255));
	m_tREWARD_WIDGET.pDojoNameStatic				->SetTextColor(RGB(255, 255, 255));
	m_tREWARD_WIDGET.pRewardReputationPointStatic	->SetTextColor(RGB(255, 255, 255));
	m_tREWARD_WIDGET.pRewardItemStatic				->SetTextColor(RGB(255, 255, 255));

	m_tREWARD_WIDGET.pGuildName				->SetTextColor(RGB(255, 252, 0));
	m_tREWARD_WIDGET.pDojoName				->SetTextColor(RGB(255, 252, 0));
	m_tREWARD_WIDGET.pRewardReputationPoint	->SetTextColor(RGB(255, 252, 0));
	m_tREWARD_WIDGET.pRewardItem			->SetTextColor(RGB(255, 252, 0));

	m_tREWARD_WIDGET.slotFSCallback = m_tREWARD_WIDGET.pFlash->SigFSCallBack().Connect( this, &CScrambleNotifyGui::OnFSCallback_RewardWidget );
	m_tREWARD_WIDGET.slotMovieEnd	= m_tREWARD_WIDGET.pFlash->SigMovieEnd().Connect( this, &CScrambleNotifyGui::OnMovieEnd_Reward );


	// default value
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pAvatar )
	{
		CNtlDojo* pDojo = pAvatar->GetDojo();

		sSCRAMBLE_TEAM*	pOFFENCE_SCRAMBLE_TEAM	= pDojo->GetTeam(SCRAMBLE_TEAM_OFFENCE);
		if( pOFFENCE_SCRAMBLE_TEAM )
		{
			m_tSTART_WIDGET.pOffenceGuildName->SetText(pOFFENCE_SCRAMBLE_TEAM->awcGuildName);
		}

		sSCRAMBLE_TEAM*	pDEFENCE_SCRAMBLE_TEAM	= pDojo->GetTeam(SCRAMBLE_TEAM_DEFENCE);
		if( pDEFENCE_SCRAMBLE_TEAM )
		{
			m_tSTART_WIDGET.pDefenceGuildName->SetText(pDEFENCE_SCRAMBLE_TEAM->awcGuildName);
		}
	}	


	m_tSTART_WIDGET.pRuleTitle	->SetText( GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_RULE_TITLE") );

	const WCHAR* pwcRule = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_RULE");
	if( pwcRule )
		m_tSTART_WIDGET.pRule->SetHtmlFromMemory(pwcRule, wcslen(pwcRule));

	m_tSTART_WIDGET.pOffenceGuildName	->Show(false);
	m_tSTART_WIDGET.pDefenceGuildName	->Show(false);
	m_tSTART_WIDGET.pRuleTitle			->Show(false);
	m_tSTART_WIDGET.pRule				->Show(false);

	

	m_tREWARD_WIDGET.pGuildNameStatic			->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO_GUILD_NAME"));
	m_tREWARD_WIDGET.pDojoNameStatic			->SetText(GetDisplayStringManager()->GetString("DST_DOJO_NAME"));
	m_tREWARD_WIDGET.pRewardReputationPointStatic->SetText(GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_REWARD_REPUTATION"));
	m_tREWARD_WIDGET.pRewardItemStatic			->SetText(GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_REWARD_ITEM"));
	
	m_tREWARD_WIDGET.pGuildNameStatic			->Show(false);
	m_tREWARD_WIDGET.pDojoNameStatic			->Show(false);
	m_tREWARD_WIDGET.pRewardReputationPointStatic->Show(false);
	m_tREWARD_WIDGET.pRewardItemStatic			->Show(false);
	m_tREWARD_WIDGET.pGuildName					->Show(false);
	m_tREWARD_WIDGET.pDojoName					->Show(false);
	m_tREWARD_WIDGET.pRewardReputationPoint		->Show(false);
	m_tREWARD_WIDGET.pRewardItem				->Show(false);

	m_tREWARD_WIDGET.uiGuildReputation		= 0;
	m_tREWARD_WIDGET.uiItemTableIndex		= INVALID_SERIAL_ID;
	m_tREWARD_WIDGET.byItemCount			= 0;

	m_tWARNING_INFO.bCanWarningSeal			= TRUE;
	m_tWARNING_INFO.bCanWarningPoint		= TRUE;
	m_tWARNING_INFO.fWarningElapsed			= 0.f;	

	GetNtlGuiManager()->AddUpdateFunc( this );

	// sig
	m_slotMouseMove		= m_tREWARD_WIDGET.pFlash->SigMouseMove().Connect( this, &CScrambleNotifyGui::OnMouseMove );
	m_slotMouseLeave	= m_tREWARD_WIDGET.pFlash->SigMouseLeave().Connect( this, &CScrambleNotifyGui::OnMouseLeave );
	m_slotPaint			= m_tREWARD_WIDGET.pFlash->SigPaint().Connect( this, &CScrambleNotifyGui::OnPaint );	

	LinkMsg(g_EventDojoNotify);
	LinkMsg(g_EventDojoStateNotify);
	LinkMsg(g_EventCinematicGuiCommand);	
	LinkMsg(g_EventResize);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CScrambleNotifyGui::Destroy()
{
	NTL_FUNCTION("CScrambleNotifyGui::Destroy");

	m_tREWARD_WIDGET.srfRewardItem.Destroy();

	DestroyResultNarration();
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	UnLinkMsg(g_EventDojoNotify);
	UnLinkMsg(g_EventDojoStateNotify);
	UnLinkMsg(g_EventCinematicGuiCommand);
	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CScrambleNotifyGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_TABLE_ITEM &&
		GetInfoWndManager()->GetRequestGui() == DIALOG_SCRAMBLE_NOTIFY )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CScrambleNotifyGui::DestroyResultNarration()
{
	if( m_tREWARD_WIDGET.pResultNarration )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_tREWARD_WIDGET.pResultNarration );
		m_tREWARD_WIDGET.pResultNarration->Destroy();
		NTL_DELETE( m_tREWARD_WIDGET.pResultNarration );
	}
}

VOID CScrambleNotifyGui::Update(RwReal fElapsed)
{
	m_tWARNING_INFO.fWarningElapsed -= fElapsed;
	if( m_tWARNING_INFO.fWarningElapsed < 0.f )
		m_tWARNING_INFO.fWarningElapsed = 0.f;

	if( IsShow() == FALSE )
		return;

	m_tSTART_WIDGET.pFlash	->Update(fElapsed);
	m_tREWARD_WIDGET.pFlash	->Update(fElapsed);
	pFlash_Notify			->Update(fElapsed);
	pFlash_Notify2			->Update(fElapsed);	
}

VOID CScrambleNotifyGui::PlayReward()
{
	FinishFlash();
	LoadFlash(m_tREWARD_WIDGET.pFlash, dFLASH_SCRAMBLE_REWARD);
	GetDialogManager()->OpenDialog(DIALOG_SCRAMBLE_NOTIFY);

	Enable(true);	
}

RwBool CScrambleNotifyGui::LoadFlash(gui::CFlash* pFlash, const char* acFileName)
{
	if( pFlash->Load(acFileName) == false )
	{
		DBO_FAIL("Not eixst file : " << acFileName << ". You can continue the game.");
		FinishFlash();
		GetDialogManager()->CloseDialog(DIALOG_SCRAMBLE_NOTIFY);
		return FALSE;
	}

	LocateComponent();
	pFlash->SetFrameCallback(FALSE);
	pFlash->PlayMovie(TRUE);

	m_strFileName = acFileName;

	return TRUE;
}

VOID CScrambleNotifyGui::LocateComponent()
{
	CRectangle rtFlash		= m_tSTART_WIDGET.pFlash->GetFrameResolution();
	RwInt32 iWidth			= rtFlash.GetWidth();
	RwInt32 iHeight			= rtFlash.GetHeight();
	RwInt32 iCleintWidth	= (RwInt32)GetDboGlobal()->GetScreenWidth();
	RwInt32 iCleintHeight	= (RwInt32)GetDboGlobal()->GetScreenHeight();	

	rtFlash.left	= (iCleintWidth - iWidth)/2;
	rtFlash.right	= rtFlash.left + iWidth;
	rtFlash.top		= (iCleintHeight - iHeight)/2;
	rtFlash.bottom	= rtFlash.top + iHeight;

	m_tSTART_WIDGET.pParentDialog->SetPosition(rtFlash);
	m_tSTART_WIDGET.pFlash->SetSize(iWidth, iHeight);


	rtFlash		= m_tREWARD_WIDGET.pFlash->GetFrameResolution();
	iWidth		= rtFlash.GetWidth();
	iHeight		= rtFlash.GetHeight();

	rtFlash.left	= (iCleintWidth - iWidth)/2;
	rtFlash.right	= rtFlash.left + iWidth;
	rtFlash.top		= (iCleintHeight - iHeight)/2;
	rtFlash.bottom	= rtFlash.top + iHeight;

	m_tREWARD_WIDGET.pParentDialog->SetPosition(rtFlash);
	m_tREWARD_WIDGET.pFlash->SetSize(iWidth, iHeight);

	m_tREWARD_WIDGET.srfRewardItem.SetParentPosition(rtFlash.left, rtFlash.top);



	rtFlash	= pFlash_Notify->GetFrameResolution();
	iWidth	= rtFlash.GetWidth();
	iHeight	= rtFlash.GetHeight();
	rtFlash.left	= (iCleintWidth - iWidth)/2;
	rtFlash.right	= rtFlash.left + iWidth;
	rtFlash.top		= (iCleintHeight - iHeight)/2;
	rtFlash.bottom	= rtFlash.top + iHeight;
	pFlash_Notify->SetPosition(rtFlash);

	rtFlash.left	= (iCleintWidth - iWidth)/2;
	rtFlash.right	= rtFlash.left + iWidth;
	rtFlash.top		= (iCleintHeight - iHeight)/2;
	rtFlash.bottom	= rtFlash.top + iHeight;
	pFlash_Notify2->SetPosition(rtFlash);



	rtFlash.left		= 0;
	rtFlash.top			= 0;
	rtFlash.right		= iCleintWidth;
	rtFlash.bottom		= iCleintHeight;
	pFlash_Notify2->SetPosition(rtFlash);


	m_pThis->SetWidth(iCleintWidth);
	m_pThis->SetHeight(iCleintHeight);
}

VOID CScrambleNotifyGui::FinishFlash()
{
	m_strFileName.clear();

	pFlash_Notify->Unload();
	pFlash_Notify2->Unload();

	m_tSTART_WIDGET.pFlash->Unload();

	m_tREWARD_WIDGET.bShowRewardItem = FALSE;

	DestroyResultNarration();

	Enable(false);
}

VOID CScrambleNotifyGui::OnFSCallback_StartWidget(const char* pcString, const char* pcString2)
{
	if( !pcString )
		return;

	if( strcmp(pcString, "Text1") == 0 )
	{
		m_tSTART_WIDGET.pOffenceGuildName->Show(true);
	}
	else if( strcmp(pcString, "Text2") == 0 )
	{
		m_tSTART_WIDGET.pDefenceGuildName->Show(true);
	}
	else if( strcmp(pcString, "Text1End") == 0 )
	{
		m_tSTART_WIDGET.pOffenceGuildName->Show(false);
	}
	else if( strcmp(pcString, "Text2End") == 0 )
	{
		m_tSTART_WIDGET.pDefenceGuildName->Show(false);
	}
	else if( strcmp(pcString, "Text3") == 0 )
	{
		m_tSTART_WIDGET.pRuleTitle	->Show(true);
		m_tSTART_WIDGET.pRule		->Show(true);
	}
	else if( strcmp(pcString, "Text3End") == 0 )
	{
		m_tSTART_WIDGET.pRuleTitle	->Show(false);
		m_tSTART_WIDGET.pRule		->Show(false);
	}
}

VOID CScrambleNotifyGui::OnFSCallback_RewardWidget(const char* pcString, const char* pcString2)
{
	if( !pcString )
		return;

	if( strcmp(pcString, "npc") != 0 )
		return;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}
	

	CNtlGuild*		pGuild			= pAvatar->GetGuild();

	m_tREWARD_WIDGET.pGuildNameStatic			->Show(true);
	m_tREWARD_WIDGET.pDojoNameStatic			->Show(true);
	m_tREWARD_WIDGET.pRewardReputationPointStatic->Show(true);
	m_tREWARD_WIDGET.pRewardItemStatic			->Show(true);
	m_tREWARD_WIDGET.pGuildName					->Show(true);
	m_tREWARD_WIDGET.pDojoName					->Show(true);
	m_tREWARD_WIDGET.pRewardReputationPoint		->Show(true);
	m_tREWARD_WIDGET.pRewardItem				->Show(true);

	m_tREWARD_WIDGET.bShowRewardItem = TRUE;

	// 이긴 길드 이름
	m_tREWARD_WIDGET.pGuildName->SetText( pGuild->GetGuildName() );

	// 획득 유파 포인트
	m_tREWARD_WIDGET.pRewardReputationPoint->SetText( m_tREWARD_WIDGET.uiGuildReputation );			

	// 도장 지역 이름 얻어오기
	std::wstring wstrDojoName = pGuild->GetGuildName();
	wstrDojoName += L" ";
	wstrDojoName += GetDisplayStringManager()->GetString("DST_DOJO");
	m_tREWARD_WIDGET.pDojoName->SetText( wstrDojoName.c_str() );

	// 획득 아이템
	if( INVALID_TBLIDX == m_tREWARD_WIDGET.uiItemTableIndex )
	{
		m_tREWARD_WIDGET.pRewardItem->SetText(L"-");
	}
	else
	{
		m_tREWARD_WIDGET.srfRewardItem.SetIcon(m_tREWARD_WIDGET.uiItemTableIndex, m_tREWARD_WIDGET.byItemCount);

		sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable(m_tREWARD_WIDGET.uiItemTableIndex);
		if( pITEM_TBLDAT )
		{
			CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
			std::wstring& strItemName = pItemTextTable->GetText(pITEM_TBLDAT->Name);		

			GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_REWARD_ITEM", FALSE, NULL, strItemName.c_str(), m_tREWARD_WIDGET.byItemCount);

			m_tREWARD_WIDGET.pRewardItem->Show(false);
		}
		else
		{
			DBO_FAIL("Not exist item table of index : " << m_tREWARD_WIDGET.uiItemTableIndex);
		}
	}

	// NPC 나레이션
	if( !m_tREWARD_WIDGET.pResultNarration )
	{
		m_tREWARD_WIDGET.pResultNarration = NTL_NEW CResultNarrationGui( "ScrambleResultNarration" );
		if( !m_tREWARD_WIDGET.pResultNarration->Create() )
		{
			m_tREWARD_WIDGET.pResultNarration->Destroy();
			NTL_DELETE( m_tREWARD_WIDGET.pResultNarration );
			return;
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_tREWARD_WIDGET.pResultNarration );

		const WCHAR* pwcResultMessage = GetDisplayStringManager()->GetString("DST_DOJO_REWARD_NPC_NARRATION");

		m_tREWARD_WIDGET.pResultNarration->Show(true);
		m_tREWARD_WIDGET.pResultNarration->SetData(dFLASH_SCRAMBLE_REWARD_NPC, pwcResultMessage);
	}
	else
	{
		DBO_FAIL("Already created scramble reward NPC narration instance");
	}
}

VOID CScrambleNotifyGui::OnMovieEnd_Notify(gui::CComponent* pComponent)
{
	RwBool bPlayReward = FALSE;

	Enable(false);

	if( dFLASH_SCRAMBLE_RESULT == m_strFileName )
	{
		bPlayReward = TRUE;
	}
		
	GetDialogManager()->CloseDialog(DIALOG_SCRAMBLE_NOTIFY);

	if( bPlayReward )
		PlayReward();
}

VOID CScrambleNotifyGui::OnMovieEnd_Reward(gui::CComponent* pComponent)
{
	m_tREWARD_WIDGET.bShowRewardItem = FALSE;

	Enable(false);
	CheckInfoWindow();
	DestroyResultNarration();
	GetDialogManager()->CloseDialog(DIALOG_SCRAMBLE_NOTIFY);
}

VOID CScrambleNotifyGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( m_tREWARD_WIDGET.srfRewardItem.PtInRect(nX, nY) )
	{
		if( m_tREWARD_WIDGET.srfRewardItem.GetSerial() != INVALID_SERIAL_ID )
		{
			if( GetInfoWndManager()->GetInfoWndState() != CInfoWndManager::INFOWND_TABLE_ITEM &&
				GetInfoWndManager()->GetRequestGui() != DIALOG_SCRAMBLE_NOTIFY )
			{
				GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
													m_tREWARD_WIDGET.srfRewardItem.GetParentX() + m_tREWARD_WIDGET.srfRewardItem.GetX_fromParent(),
													m_tREWARD_WIDGET.srfRewardItem.GetParentY() + m_tREWARD_WIDGET.srfRewardItem.GetY_fromParent(),
													m_tREWARD_WIDGET.srfRewardItem.GetItemTable(), DIALOG_SCRAMBLE_NOTIFY );
			}
		}

		return;
	}

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CScrambleNotifyGui::OnMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CScrambleNotifyGui::OnPaint()
{
	if( m_tREWARD_WIDGET.bShowRewardItem )
		m_tREWARD_WIDGET.srfRewardItem.Paint();
}

RwInt32 CScrambleNotifyGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{		
	}
	else
	{
		FinishFlash();		
	}

	Show(bOpen);
	return 1;
}

VOID CScrambleNotifyGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CScrambleNotifyGui::HandleEvents");

	if( msg.Id == g_EventDojoNotify )
	{
		SNtlEventDojo* pEvent = reinterpret_cast<SNtlEventDojo*>( msg.pData );

		switch(pEvent->byDojoEvent)
		{
		case DOJO_EVENT_SCRAMBLE_SCORE:
			{
				// 다른 정보가 더 중요하기에 우선 순위를 낮췄다.
				// 플래쉬가 이미 연출중이면 경고 플래쉬는 보여주지 않는다
				if( IsShow() )
					NTL_RETURNVOID();

				if( m_tWARNING_INFO.fWarningElapsed > 0.f )
					NTL_RETURNVOID();

				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
					NTL_RETURNVOID();

				CNtlDojo*				pDojo					= pAvatar->GetDojo();
				sSCRAMBLE_INFO*			pSCRAMBLE_INFO			= pDojo->GetScramble();
				sDOJO_SCRAMBLE_POINT*	pDOJO_SCRAMBLE_POINT	= (sDOJO_SCRAMBLE_POINT*)pEvent->pExData;

				RwBool bPlayFlash = FALSE;
				RwInt32 iPointGap = pDOJO_SCRAMBLE_POINT->dwAttPoint - pDOJO_SCRAMBLE_POINT->dwDefPoint;

				switch(pSCRAMBLE_INFO->eAvatarTeam)
				{
				case SCRAMBLE_TEAM_OFFENCE:
					{
						if( -(dWARNING_POINT_GAP) > iPointGap )
						{
							if( m_tWARNING_INFO.bCanWarningPoint )
							{
								bPlayFlash = TRUE;
								m_tWARNING_INFO.bCanWarningPoint = FALSE;
							}
						}
						else
						{
							m_tWARNING_INFO.bCanWarningPoint = TRUE;
						}
						
						if( dWARNING_BE_STEALED_SEAL_COUNT <= pDOJO_SCRAMBLE_POINT->byDefSealCount )
						{
							if( m_tWARNING_INFO.bCanWarningSeal )
							{
								bPlayFlash = TRUE;
								m_tWARNING_INFO.bCanWarningSeal = FALSE;
							}
						}
						else
						{
							m_tWARNING_INFO.bCanWarningSeal = TRUE;
						}
						break;
					}
				case SCRAMBLE_TEAM_DEFENCE:
					{
						if( dWARNING_POINT_GAP < iPointGap )
						{
							if( m_tWARNING_INFO.bCanWarningPoint )
							{
								bPlayFlash = TRUE;
								m_tWARNING_INFO.bCanWarningSeal = FALSE;
							}
						}
						else
						{
							m_tWARNING_INFO.bCanWarningPoint = TRUE;
						}

						if( dWARNING_BE_STEALED_SEAL_COUNT <= pDOJO_SCRAMBLE_POINT->byAttSealCount )
						{
							if( m_tWARNING_INFO.bCanWarningSeal )
							{
								bPlayFlash = TRUE;
								m_tWARNING_INFO.bCanWarningSeal = FALSE;
							}
						}
						else
						{
							m_tWARNING_INFO.bCanWarningSeal = TRUE;
						}
						break;
					}
				}

				if( bPlayFlash )
				{
					m_tWARNING_INFO.fWarningElapsed = dWARNING_REST_TIME;

					FinishFlash();
					LoadFlash(pFlash_Notify, dFLASH_SCRAMBLE_WARNING);
					GetDialogManager()->OpenDialog(DIALOG_SCRAMBLE_NOTIFY);
				}

				break;
			}
		case DOJO_EVENT_SCRAMBLE_CHANGE_SEAL_OWNER:
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
					break;

				GUILDID		guildID	= *(GUILDID*)pEvent->pExData2;
				CNtlGuild*	pGuild	= pAvatar->GetGuild();

				if( guildID == pGuild->GetGuildID() )
				{			
					FinishFlash();
					LoadFlash(pFlash_Notify, dFLASH_SCRAMBLE_OFFENCE_SUCCESS);
					GetDialogManager()->OpenDialog(DIALOG_SCRAMBLE_NOTIFY);
				}
				else
				{
					FinishFlash();
					LoadFlash(pFlash_Notify, dFLASH_SCRAMBLE_DEFENCE_FAIL);
					GetDialogManager()->OpenDialog(DIALOG_SCRAMBLE_NOTIFY);
				}

				break;
			}
		case DOJO_EVENT_SCRAMBLE_RESULT:
			{
				GUILDID			winnerGuildID	= pEvent->uiParam;
				CNtlSobAvatar*	pAvatar			= GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
					break;

				CNtlGuild* pGuild = pAvatar->GetGuild();

				FinishFlash();				

				if( pGuild->GetGuildID() == winnerGuildID )
				{
					LoadFlash(pFlash_Notify, dFLASH_SCRAMBLE_RESULT);
					pFlash_Notify->Invoke("Conclusion", "%d", 1);
				}
				else
				{
					// 졌다면 임시로 서버에서 연출 상태가 추가될 때까지 막는다
					NTL_RETURNVOID();

					pFlash_Notify->Invoke("Conclusion", "%d", 0);
				}			

				GetDialogManager()->OpenDialog(DIALOG_SCRAMBLE_NOTIFY);
				break;
			}
		case DOJO_EVENT_SCRAMBLE_REWARD:
			{
				m_tREWARD_WIDGET.uiGuildReputation	= pEvent->uiParam;
				m_tREWARD_WIDGET.uiItemTableIndex	= *(SERIAL_HANDLE*)pEvent->pExData;
				m_tREWARD_WIDGET.byItemCount		= *(RwUInt8*)pEvent->pExData2;

				if( m_strFileName.empty() )
				{
					// 아무런 연출중이 아닐 때만 보상 연출을 바로 보여준다
					// 만약 결과창을 보여주고 있다면 결과창 이후에 보상 연출을 보여줄 것이다
					PlayReward();
				}

				break;
			}
		}
	}
	else if( msg.Id == g_EventDojoStateNotify )
	{
		SNtlEventDojoMessage* pEvent = reinterpret_cast<SNtlEventDojoMessage*>( msg.pData );

		if( eDBO_DOJO_STATUS_START == pEvent->byDojoState )
		{			
			FinishFlash();
			LoadFlash(pFlash_Notify, dFLASH_SCRAMBLE_START);
			GetDialogManager()->OpenDialog(DIALOG_SCRAMBLE_NOTIFY);

			GetNtlDTCinematicManager()->Clear();
		}
	}
	else if( msg.Id == g_EventCinematicGuiCommand )
	{
		SNtlEventCinematicGuiCommand* pEvent = reinterpret_cast<SNtlEventCinematicGuiCommand*>( msg.pData );

		if( !pEvent->szCommand )
			NTL_RETURNVOID();

		if( strcmp(dCINEMATIC_GUI_KEYWARD_NAME, pEvent->szCommand ) != 0 )
			NTL_RETURNVOID();

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			NTL_RETURNVOID();

		CNtlDojo*		pDojo			= pAvatar->GetDojo();
		sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();
		
		if( eDBO_DOJO_STATUS_READY != pSCRAMBLE_INFO->eDojoState )
			NTL_RETURNVOID();


		// 도장 쟁탈전 대기중 GUI 연출
		if( pEvent->bShow )
		{
			LoadFlash(m_tSTART_WIDGET.pFlash, dFLASH_SCRAMBLE_TITLE);
			GetDialogManager()->OpenDialog(DIALOG_SCRAMBLE_NOTIFY);
		}
	}
	else if( msg.Id == g_EventResize )
	{
		LocateComponent();
	}

	NTL_RETURNVOID();
}