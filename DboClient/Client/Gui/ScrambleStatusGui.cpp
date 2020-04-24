#include "precomp_dboclient.h"
#include "ScrambleStatusGui.h"


// shared
#include "DojoTable.h"
#include "TableContainer.h"
#include "NtlDojo.h"
#include "ObjectTable.h"
#include "TextAllTable.h"
#include "NtlLuaState.h"

// core
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"
#include "NtlSoundManager.h"
#include "NtlSoundEventGenerator.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEmblemMaker.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlWorldConcept.h"
#include "NtlSLLogic.h"
#include "NtlSLLuaGlue.h"

// client
#include "SideDialogManager.h"
#include "InfoWndManager.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "MsgBoxManager.h"
#include "ScrambleBalloonHelper.h"

#define dEMBLEM_SIZE						(45)

#define dWINNER_SCALE_MARK_POS_X			(28)
#define dWINNER_SCALE_MARK_POS_Y			(152)
#define dWINNER_SCALE_MARK_WIDTH			(110)

#define dWINNER_MAX_POINT					(100)



CScrambleStatusGui::CScrambleStatusGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_eInfoWindowType(INVALID_INFO_WINDOW_TYPE)
,m_hBGMSound(INVALID_SOUND_HANDLE)
,m_pScrambleBalloonHelper(NULL)
{

}

CScrambleStatusGui::~CScrambleStatusGui()
{
}

RwBool CScrambleStatusGui::Create()
{
	NTL_FUNCTION( "CScrambleStatusGui::Create" );

	if(!CNtlPLGui::Create("gui\\ScrambleStatus.rsr", "gui\\ScrambleStatus.srf", "gui\\ScrambleStatus.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pMinimizeButton		= (gui::CButton*)GetComponent( "btnMinimize" );
	m_slotMinimizeButton	= m_pMinimizeButton->SigClicked().Connect(this, &CScrambleStatusGui::OnClicked_MinimizeButton);

	m_pJoinButton			= (gui::CButton*)GetComponent( "btnJoin" );
	m_slotJoinButton		= m_pJoinButton->SigClicked().Connect(this, &CScrambleStatusGui::OnClicked_JoinButton);

	m_pDialogName			= (gui::CStaticBox*)GetComponent( "stbDialogName" );
	m_pEnemyTeam_GuildName	= (gui::CStaticBox*)GetComponent( "stbEnemyTeam_GuildName" );
	m_pEnemyTeam_PeopleCount= (gui::CStaticBox*)GetComponent( "stbEnemyTeamPeopleCount" );
	m_pEnemyTeam_SealCount	= (gui::CStaticBox*)GetComponent( "stbEnemyTeamSealCount" );
	m_pEnemyTeam_Point		= (gui::CStaticBox*)GetComponent( "stbEnemyTeamPoint" );
	m_pMyTeam_GuildName		= (gui::CStaticBox*)GetComponent( "stbMyTeam_GuildName" );
	m_pMyTeam_PeopleCount	= (gui::CStaticBox*)GetComponent( "stbMyTeamPeopleCount" );
	m_pMyTeam_SealCount		= (gui::CStaticBox*)GetComponent( "stbMyTeamSealCount" );
	m_pMyTeam_Point			= (gui::CStaticBox*)GetComponent( "stbMyTeamPoint" );
	m_pTimer				= (gui::CStaticBox*)GetComponent( "stbTimer" );

	m_pEnemyTeam_PeoplePanel= (gui::CPanel*)GetComponent( "pnlEnemyTeamPeopleCount" );
	m_pEnemyTeam_SealPanel	= (gui::CPanel*)GetComponent( "pnlEnemyTeamSealCount" );
	m_pStatus_Seal[0]		= (gui::CPanel*)GetComponent( "pnlOwnerSeal_0" );
	m_pStatus_Seal[1]		= (gui::CPanel*)GetComponent( "pnlOwnerSeal_1" );
	m_pStatus_Seal[2]		= (gui::CPanel*)GetComponent( "pnlOwnerSeal_2" );
	m_pStatus_Seal[3]		= (gui::CPanel*)GetComponent( "pnlOwnerSeal_3" );
	m_pStatus_Seal[4]		= (gui::CPanel*)GetComponent( "pnlOwnerSeal_4" );
	m_pMyTeam_PeoplePanel	= (gui::CPanel*)GetComponent( "pnlMyTeamPeopleCount" );
	m_pMyTeam_SealPanel		= (gui::CPanel*)GetComponent( "pnlMyTeamSealCount" );

	m_pEnemyTeam_PointGause	= (gui::CProgressBar*)GetComponent( "pgbEnemyTeamPointGause" );
	m_pMyTeam_PointGause	= (gui::CProgressBar*)GetComponent( "pgbMyTeamPointGause" );

	m_pEnemyTeamEmblem		= NTL_NEW CRwTextureGui;
	m_pEnemyTeamEmblem		->SetSize(dEMBLEM_SIZE, dEMBLEM_SIZE);

	m_pMyTeamEmblem			= NTL_NEW CRwTextureGui;
	m_pMyTeamEmblem			->SetSize(dEMBLEM_SIZE, dEMBLEM_SIZE);

	m_srfEnemyTeam_Background	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ScrambleStatus.srf", "srfEnemyTeamBackground" ));
	m_srfMyTeam_Background		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ScrambleStatus.srf", "srfMyTeamBackground" ));
	m_srfStatus_Background		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ScrambleStatus.srf", "srfStatusBackground" ));
	m_srfStatus_Background		.SetPositionfromParent(6, 127);

	m_srfEnemyTeam_GauseBack	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ScrambleStatus.srf", "srfEnemyTeamPointGauseBack" ));
	m_srfEnemyTeam_NoEmblem		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ScrambleStatus.srf", "srfNoEmblem" ));
	
	m_srfMyTeam_GauseBack		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ScrambleStatus.srf", "srfMyTeamPointGauseBack" ));
	m_srfMyTeam_NoEmblem		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ScrambleStatus.srf", "srfNoEmblem" ));



	// default value
	m_pDialogName			->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pEnemyTeam_GuildName	->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pMyTeam_GuildName		->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pEnemyTeam_Point		->CreateFontStd(DEFAULT_FONT, 120, DEFAULT_FONT_ATTR);
	m_pMyTeam_Point			->CreateFontStd(DEFAULT_FONT, 120, DEFAULT_FONT_ATTR);

	m_pDialogName			->SetText( GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_TITLE") );

	std::wstring wstr;

	wstr = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_GUILD_MEMBER_COUNT");
	m_pEnemyTeam_PeoplePanel	->SetToolTip(wstr);

	wstr = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_OWNER_SEAL_COUNT");
	m_pEnemyTeam_SealPanel	->SetToolTip(wstr);

	wstr = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_GUILD_MEMBER_COUNT");
	m_pMyTeam_PeoplePanel	->SetToolTip(wstr);

	wstr = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_OWNER_SEAL_COUNT");
	m_pMyTeam_SealPanel	->SetToolTip(wstr);

	m_pJoinButton			->SetText( GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_ENTER") );

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		NTL_RETURN(FALSE);
	}

	CNtlDojo* pDojo = pAvatar->GetDojo();
	sSCRAMBLE_INFO* pSCRAMBLE_INFO = pDojo->GetScramble();
	if( !pSCRAMBLE_INFO )
	{
		DBO_FAIL("Not exist scmrable info");
		NTL_RETURN(FALSE);
	}

	sDOJO_TBLDAT* pDOJO_TBLDAT = reinterpret_cast<sDOJO_TBLDAT*>( API_GetTableContainer()->GetDojoTable()->FindData(pSCRAMBLE_INFO->uiScrambleDojoTableIndex) );
	if( !pDOJO_TBLDAT )
	{
		DBO_FAIL("Not exist dojo table of index : " << pSCRAMBLE_INFO->uiScrambleDojoTableIndex);
		NTL_RETURN(FALSE);
	}

	m_pEnemyTeam_PointGause		->SetRange(0, pDOJO_TBLDAT->dwChargePointGoal);
	m_pMyTeam_PointGause		->SetRange(0, pDOJO_TBLDAT->dwChargePointGoal);

	InitEmblem();
	CheckTeleportButton();
	SetScrambleBGM((RwUInt8)pSCRAMBLE_INFO->eDojoState);


	m_pScrambleBalloonHelper	= NTL_NEW ScrambleBalloonHelper;
	if( !m_pScrambleBalloonHelper->Create() )
		NTL_RETURN(FALSE);


	// 컴포넌트를 배치
	SetComponent_of_Team();
	OnMove(0, 0);

	GetNtlGuiManager()->AddUpdateFunc( this );

	// sig
	m_slotMove			= m_pThis->SigMove().Connect( this, &CScrambleStatusGui::OnMove );	
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CScrambleStatusGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CScrambleStatusGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CScrambleStatusGui::OnPaint );

	LinkMsg(g_EventDojoNotify);
	LinkMsg(g_EventNotifyGuild);	
	LinkMsg(g_EventChangeWorldConceptState);
	LinkMsg(g_EventTeleportConfirmRes);
	LinkMsg(g_EventEndterWorld);
	LinkMsg(g_EventDojoStateNotify);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CScrambleStatusGui::Destroy()
{
	NTL_FUNCTION("CScrambleStatusGui::Destroy");

	CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_STOP_SCRAMBLE_READY_BGM);
	CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_END_SCRAMBLE_START_BGM);

	if( INVALID_SOUND_HANDLE != m_hBGMSound )
		GetSoundManager()->Stop(m_hBGMSound);

	if( m_pScrambleBalloonHelper )
	{
		m_pScrambleBalloonHelper->Destroy();
		NTL_DELETE(m_pScrambleBalloonHelper);
	}

	if( m_pEnemyTeamEmblem )
	{
		if( FALSE == m_pEnemyTeamEmblem->IsSelfCreatedTexture() &&
			m_pEnemyTeamEmblem->GetTexture() )
		{
			GetEmblemMaker()->DestoyEmblem( m_pEnemyTeamEmblem->GetTexture() );
		}

		NTL_DELETE(m_pEnemyTeamEmblem);
	}
	
	if( m_pMyTeamEmblem )
	{
		if( FALSE == m_pMyTeamEmblem->IsSelfCreatedTexture() &&
			m_pMyTeamEmblem->GetTexture() )
		{
			GetEmblemMaker()->DestoyEmblem( m_pMyTeamEmblem->GetTexture() );
		}

		NTL_DELETE(m_pMyTeamEmblem);
	}	

	GetNtlGuiManager()->RemoveUpdateFunc( this );
	

	UnLinkMsg(g_EventDojoNotify);
	UnLinkMsg(g_EventNotifyGuild);
	UnLinkMsg(g_EventChangeWorldConceptState);	
	UnLinkMsg(g_EventTeleportConfirmRes);
	UnLinkMsg(g_EventEndterWorld);	
	UnLinkMsg(g_EventDojoStateNotify);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CScrambleStatusGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	if( m_pScrambleBalloonHelper )
		m_pScrambleBalloonHelper->Update(fElapsed);


	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		NTL_RETURNVOID();

	CNtlDojo*		pDojo			= pAvatar->GetDojo();
	sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();
	if( !pSCRAMBLE_INFO )
		NTL_RETURNVOID();

	
	if( eDBO_DOJO_STATUS_START == pSCRAMBLE_INFO->eDojoState )
	{
		RwUInt32	uiLeftTime	= (RwUInt32)pSCRAMBLE_INFO->fLeftTime;
		RwUInt32	uiSecond	= uiLeftTime % 60;
		RwUInt32	uiMinute	= (uiLeftTime - uiSecond) / 60;

		if( 99 < uiMinute )
			uiMinute = 99;

		m_pTimer->Format(L"%02d:%02d", uiMinute, uiSecond);
	}	
}

VOID CScrambleStatusGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == SDIALOG_SCRAMBLE_STATUS )
	{
		m_eInfoWindowType = INVALID_INFO_WINDOW_TYPE;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CScrambleStatusGui::SetComponent_of_Team()
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		NTL_RETURNVOID();

	CNtlDojo*		pDojo			= pAvatar->GetDojo();
	sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();
	if( !pSCRAMBLE_INFO )
		NTL_RETURNVOID();


	if( SCRAMBLE_TEAM_OFFENCE == pSCRAMBLE_INFO->eAvatarTeam )
	{
		sSCRAMBLE_TEAM* pOFFENCE_SCRAMBLE_TEAM = pDojo->GetTeam(SCRAMBLE_TEAM_OFFENCE);
		if( pOFFENCE_SCRAMBLE_TEAM )
			m_pMyTeam_GuildName->SetText(pOFFENCE_SCRAMBLE_TEAM->awcGuildName);

		sSCRAMBLE_TEAM* pDEFENCE_SCRAMBLE_TEAM = pDojo->GetTeam(SCRAMBLE_TEAM_DEFENCE);
		if( pDEFENCE_SCRAMBLE_TEAM )
			m_pEnemyTeam_GuildName->SetText(pDEFENCE_SCRAMBLE_TEAM->awcGuildName);
	}
	else
	{
		sSCRAMBLE_TEAM* pOFFENCE_SCRAMBLE_TEAM = pDojo->GetTeam(SCRAMBLE_TEAM_OFFENCE);
		if( pOFFENCE_SCRAMBLE_TEAM )
			m_pEnemyTeam_GuildName->SetText(pOFFENCE_SCRAMBLE_TEAM->awcGuildName);

		sSCRAMBLE_TEAM* pDEFENCE_SCRAMBLE_TEAM = pDojo->GetTeam(SCRAMBLE_TEAM_DEFENCE);
		if( pDEFENCE_SCRAMBLE_TEAM )
			m_pMyTeam_GuildName->SetText(pDEFENCE_SCRAMBLE_TEAM->awcGuildName);
	}


	// 실제 월드에서 공격팀이 북쪽에 위치한다. GUI도 공격팀의 컴포넌트를 위에 위치한다.
	if( SCRAMBLE_TEAM_OFFENCE == pSCRAMBLE_INFO->eAvatarTeam )
	{
		// 우리팀
		m_pMyTeamEmblem			->SetPositionfromParent(8, 34);
		m_pMyTeam_GuildName		->SetPosition(59, 49);
		m_pMyTeam_PeoplePanel	->SetPosition(9, 81);
		m_pMyTeam_SealPanel		->SetPosition(9, 102);
		m_pMyTeam_PeopleCount	->SetPosition(35, 82);
		m_pMyTeam_SealCount		->SetPosition(35, 100);
		m_pMyTeam_PointGause	->SetPosition(58, 108);
		m_pMyTeam_Point			->SetPosition(88, 75);

		m_srfEnemyTeam_Background	.SetPositionfromParent(4, 31);		
		m_srfMyTeam_GauseBack		.SetPositionfromParent(58, 108);
		m_srfMyTeam_NoEmblem		.SetPositionfromParent(9, 35);		

		// 상대팀
		m_pEnemyTeamEmblem			->SetPositionfromParent(8, 204);
		m_pEnemyTeam_GuildName		->SetPosition(59, 221);
		m_pEnemyTeam_PeoplePanel	->SetPosition(9, 251);
		m_pEnemyTeam_SealPanel		->SetPosition(9, 272);
		m_pEnemyTeam_PeopleCount	->SetPosition(35, 250);
		m_pEnemyTeam_SealCount		->SetPosition(35, 270);
		m_pEnemyTeam_PointGause		->SetPosition(58, 278);
		m_pEnemyTeam_Point			->SetPosition(88, 245);

		m_srfMyTeam_Background		.SetPositionfromParent(4, 201);
		m_srfEnemyTeam_GauseBack	.SetPositionfromParent(58, 278);
		m_srfEnemyTeam_NoEmblem		.SetPositionfromParent(9, 205);
	}
	else
	{
		// 상대팀
		m_pEnemyTeamEmblem			->SetPositionfromParent(8, 34);
		m_pEnemyTeam_GuildName		->SetPosition(59, 49);
		m_pEnemyTeam_PeoplePanel	->SetPosition(9, 81);
		m_pEnemyTeam_SealPanel		->SetPosition(9, 102);
		m_pEnemyTeam_PeopleCount	->SetPosition(35, 82);
		m_pEnemyTeam_SealCount		->SetPosition(35, 100);
		m_pEnemyTeam_PointGause		->SetPosition(58, 108);
		m_pEnemyTeam_Point			->SetPosition(88, 75);

		m_srfMyTeam_Background		.SetPositionfromParent(4, 31);		
		m_srfEnemyTeam_GauseBack	.SetPositionfromParent(58, 108);
		m_srfEnemyTeam_NoEmblem		.SetPositionfromParent(9, 35);

		// 우리팀
		m_pMyTeamEmblem			->SetPositionfromParent(8, 204);
		m_pMyTeam_GuildName		->SetPosition(59, 221);
		m_pMyTeam_PeoplePanel	->SetPosition(9, 251);
		m_pMyTeam_SealPanel		->SetPosition(9, 272);
		m_pMyTeam_PeopleCount	->SetPosition(35, 250);
		m_pMyTeam_SealCount		->SetPosition(35, 270);
		m_pMyTeam_PointGause	->SetPosition(58, 278);
		m_pMyTeam_Point			->SetPosition(88, 245);

		m_srfEnemyTeam_Background	.SetPositionfromParent(4, 201);
		m_srfMyTeam_GauseBack		.SetPositionfromParent(58, 278);
		m_srfMyTeam_NoEmblem		.SetPositionfromParent(9, 205);		
	}
}

VOID CScrambleStatusGui::InitEmblem()
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlDojo*		pDojo			= pAvatar->GetDojo();
	sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();
	if( !pSCRAMBLE_INFO )
		return;


	if( SCRAMBLE_TEAM_OFFENCE == pSCRAMBLE_INFO->eAvatarTeam )
	{
		sSCRAMBLE_TEAM* pOFFENCE_SCRAMBLE_TEAM = pDojo->GetTeam(SCRAMBLE_TEAM_OFFENCE);
		if( pOFFENCE_SCRAMBLE_TEAM )
		{
			SetEmblem(FALSE, &pOFFENCE_SCRAMBLE_TEAM->tEmblemFactor);
		}

		sSCRAMBLE_TEAM* pDEFENCE_SCRAMBLE_TEAM = pDojo->GetTeam(SCRAMBLE_TEAM_DEFENCE);
		if( pDEFENCE_SCRAMBLE_TEAM )
		{
			SetEmblem(TRUE, &pDEFENCE_SCRAMBLE_TEAM->tEmblemFactor);
		}
	}
	else
	{
		sSCRAMBLE_TEAM* pOFFENCE_SCRAMBLE_TEAM = pDojo->GetTeam(SCRAMBLE_TEAM_OFFENCE);
		if( pOFFENCE_SCRAMBLE_TEAM )
		{
			SetEmblem(TRUE, &pOFFENCE_SCRAMBLE_TEAM->tEmblemFactor);
		}

		sSCRAMBLE_TEAM* pDEFENCE_SCRAMBLE_TEAM = pDojo->GetTeam(SCRAMBLE_TEAM_DEFENCE);
		if( pDEFENCE_SCRAMBLE_TEAM )
		{
			SetEmblem(FALSE, &pDEFENCE_SCRAMBLE_TEAM->tEmblemFactor);
		}
	}
}

VOID CScrambleStatusGui::SetEmblem(RwBool bEnemyTeamTeam, sEmblemFactor* pEmblemFactor)
{
	if( !pEmblemFactor )
		return;

	if( bEnemyTeamTeam )
	{
		if( !m_pEnemyTeamEmblem )
			return;

		if( FALSE == m_pEnemyTeamEmblem->IsSelfCreatedTexture() &&
			m_pEnemyTeamEmblem->GetTexture() )
		{
			GetEmblemMaker()->DestoyEmblem( m_pEnemyTeamEmblem->GetTexture() );
		}

		m_pEnemyTeamEmblem->UnsetTexture();
		m_pEnemyTeamEmblem->SetTexture( GetEmblemMaker()->CreateEmblem(pEmblemFactor) );
	}
	else
	{
		if( !m_pMyTeamEmblem )
			return;

		if( FALSE == m_pMyTeamEmblem->IsSelfCreatedTexture() &&
			m_pMyTeamEmblem->GetTexture() )
		{
			GetEmblemMaker()->DestoyEmblem( m_pMyTeamEmblem->GetTexture() );
		}

		m_pMyTeamEmblem->UnsetTexture();
		m_pMyTeamEmblem->SetTexture( GetEmblemMaker()->CreateEmblem(pEmblemFactor) );
	}
}

VOID CScrambleStatusGui::InitSealState()
{
	// Seal(인장의 GUI 순서)
	// 1					2
	//			3
	// 4					5
	// 이 순서에 맞게 인장 오브젝트 테이블의 인덱스 순서와 배치 순서를 기획쪽과 맞추기로 했다


	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CTextTable*		pObjectTextTable	= API_GetTableContainer()->GetTextAllTable()->GetObjectTbl();
	CObjectTable*	pObjectTable		= API_GetTableContainer()->GetObjectTable( Logic_GetActiveWorldId() );
	if( pObjectTable == NULL )
	{
		DBO_FAIL("Not exist obejct table of world ID : " << Logic_GetActiveWorldId());
	}


	CNtlDojo*		pDojo			= pAvatar->GetDojo();
	sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

	
	RwUInt8			byIndex = 0;
	std::wstring	wstrSealName;
	MAP_SCRAMBLE_SEAL::iterator it_SrambleSeal = pSCRAMBLE_INFO->mapScrambleSeal.begin();
	for( ; it_SrambleSeal != pSCRAMBLE_INFO->mapScrambleSeal.end() ; ++it_SrambleSeal )
	{
		sSCRAMBLE_SEAL_INFO*	pSCRAMBLE_SEAL_INFO = it_SrambleSeal->second;
		eDBO_TEAM				eDBOTeam			= Logic_GetScrambleTeam(pSCRAMBLE_SEAL_INFO->eState);

		// 상태 설정
		SetSealState(byIndex, eDBOTeam);

		// 툴팁 설정
		if( pObjectTable )
		{
			TBLIDX			idxObject		= it_SrambleSeal->first;
			sOBJECT_TBLDAT*	pOBJECT_TBLDAT	= reinterpret_cast<sOBJECT_TBLDAT*>( pObjectTable->FindData(idxObject) );

			if( pOBJECT_TBLDAT )
			{
				wstrSealName = pObjectTextTable->GetText(pOBJECT_TBLDAT->dwName);
				m_pStatus_Seal[byIndex]->SetToolTip(wstrSealName);	
			}
			else
			{
				DBO_FAIL("Not exist object table of index " << idxObject);
			}
		}

		++byIndex;
	}

	// 도장 쟁탈전 대기중 연출 시작
	if( eDBO_DOJO_STATUS_READY == pSCRAMBLE_INFO->eDojoState )
	{
		CNtlLuaState* pLuaState = GetLuaState();
		if( pLuaState )
		{
			pLuaState->RunString("Dojo_prestage()");
		}
		else
		{
			DBO_FAIL("Not exist LuaState instance");
		}
	}	

	SetScrambleBGM((RwUInt8)pSCRAMBLE_INFO->eDojoState);
}

VOID CScrambleStatusGui::SetSealState(RwUInt8 byObjectIndex, eDBO_TEAM eDBOTeam)
{
	if( dNUM_SEAL <= byObjectIndex )
	{
		DBO_FAIL("Invalid seal index : " << byObjectIndex);
		return;
	}


	std::list<gui::CSurface>* pSurfaceList = m_pStatus_Seal[byObjectIndex]->GetSurface();
	CRectangle rtRect = m_pStatus_Seal[byObjectIndex]->GetPosition();

	pSurfaceList->clear();	

	switch(eDBOTeam)
	{
	case DBO_TEAM_MY_TEAM:
		{
			m_pStatus_Seal[byObjectIndex]->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ScrambleStatus.srf", "srfMyTeamSeal" ) );
			break;
		}
	case DBO_TEAM_ENEMY:
		{
			m_pStatus_Seal[byObjectIndex]->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ScrambleStatus.srf", "srfEnemyTeamSeal" ) );
			break;
		}
	case DBO_TEAM_NEUTRAILITY:
		{
			m_pStatus_Seal[byObjectIndex]->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ScrambleStatus.srf", "srfNoTeamSeal" ) );
			break;
		}
	default:
		{
			DBO_FAIL("Invalid DBO Team type");
			break;
		}
	}

	m_pStatus_Seal[byObjectIndex]->SetPosition(rtRect.left, rtRect.top);
}

VOID CScrambleStatusGui::SetScrambleBGM(RwUInt8 byDojoState)
{
	if( eDBO_DOJO_STATUS_READY == byDojoState )
	{
		if( FALSE == GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
			return;

		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_SCRAMBLE_READY_BGM);
	}
	else if( eDBO_DOJO_STATUS_START == byDojoState )
	{
		if( FALSE == GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
			return;

		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_SCRAMBLE_START_BGM);
	}
	else
	{
		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_STOP_SCRAMBLE_READY_BGM);
		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_END_SCRAMBLE_START_BGM);
	}
}

VOID CScrambleStatusGui::CheckTeleportButton()
{
	if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
	{
		GetMsgBoxManager()->DeleteMsgBox("DST_DOJO_SCRAMBLE_ASK_TELEPORT");
		m_pJoinButton->ClickEnable(false);
	}
	else
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
		{
			DBO_FAIL("Not exist avatar instance");
			return;
		}

		CNtlDojo*		pDojo			= pAvatar->GetDojo();
		sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

		if( TELEPORT_TYPE_INVALID == pSCRAMBLE_INFO->byTeleportProposalIndex )
			return;

		m_pJoinButton->ClickEnable(true);
	}
}

VOID CScrambleStatusGui::OnClicked_MinimizeButton(gui::CComponent* pComponent)
{
	CDboEventGenerator::SideDialogEvent(SDIALOG_MESSAGE_MINIMIZE, SDIALOG_SCRAMBLE_STATUS, DIALOG_SIDEDIALOG_CONTROLLER);
}

VOID CScrambleStatusGui::OnClicked_JoinButton(gui::CComponent* pComponent)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlDojo*		pDojo			= pAvatar->GetDojo();
	sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

	if( TELEPORT_TYPE_INVALID == pSCRAMBLE_INFO->byTeleportProposalIndex )
		return;

	GetAlarmManager()->AlarmMessage("DST_DOJO_SCRAMBLE_ASK_TELEPORT");
}

VOID CScrambleStatusGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_srfMyTeam_Background		.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfEnemyTeam_Background	.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfStatus_Background		.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfEnemyTeam_GauseBack	.SetPositionbyParent(rtScreen.left, rtScreen.top);	
	m_srfEnemyTeam_NoEmblem		.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfMyTeam_GauseBack		.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfMyTeam_NoEmblem		.SetPositionbyParent(rtScreen.left, rtScreen.top);

	if( m_pEnemyTeamEmblem )
		m_pEnemyTeamEmblem		->SetPositionbyParent(rtScreen.left, rtScreen.top);

	if( m_pMyTeamEmblem )
		m_pMyTeamEmblem		->SetPositionbyParent(rtScreen.left, rtScreen.top);
}

VOID CScrambleStatusGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{	
	CRectangle rtScreen = m_pThis->GetScreenRect();

	// 상대편 엠블렘
	if( m_srfEnemyTeam_NoEmblem.PtInRect(rtScreen.left + nX, rtScreen.top + nY) )
	{
		if( INFO_WINDOW_TYPE_DOJO_NAME_IN_UPPER_EMBLEM != m_eInfoWindowType )
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( pAvatar )
			{
				CNtlDojo*		pDojo			= pAvatar->GetDojo();				
				sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

				if( pSCRAMBLE_INFO )
				{
					sSCRAMBLE_TEAM* pSCRAMBLE_TEAM = NULL;

					if( SCRAMBLE_TEAM_OFFENCE == pSCRAMBLE_INFO->eAvatarTeam )
					{
						pSCRAMBLE_TEAM	= pDojo->GetTeam(SCRAMBLE_TEAM_DEFENCE);
					}
					else
					{
						pSCRAMBLE_TEAM	= pDojo->GetTeam(SCRAMBLE_TEAM_OFFENCE);
					}
				
					if( pSCRAMBLE_TEAM )
					{
						CRectangle rtEmblem;
						m_srfEnemyTeam_NoEmblem.GetRect(rtEmblem);
						GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
															rtEmblem.left, rtEmblem.top,
															pSCRAMBLE_TEAM->awcGuildName, SDIALOG_SCRAMBLE_STATUS );

						m_eInfoWindowType = INFO_WINDOW_TYPE_DOJO_NAME_IN_UPPER_EMBLEM;						
					}
				}
			}
		}	

		return;
	}
	else if( m_srfMyTeam_NoEmblem.PtInRect(rtScreen.left + nX, rtScreen.top + nY) )
	{
		if( INFO_WINDOW_TYPE_DOJO_NAME_IN_LOWER_EMBLEM != m_eInfoWindowType )
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( pAvatar )
			{
				CNtlDojo*		pDojo			= pAvatar->GetDojo();				
				sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

				if( pSCRAMBLE_INFO )
				{
					sSCRAMBLE_TEAM* pSCRAMBLE_TEAM = NULL;

					if( SCRAMBLE_TEAM_OFFENCE == pSCRAMBLE_INFO->eAvatarTeam )
					{
						pSCRAMBLE_TEAM	= pDojo->GetTeam(SCRAMBLE_TEAM_OFFENCE);
					}
					else
					{
						pSCRAMBLE_TEAM	= pDojo->GetTeam(SCRAMBLE_TEAM_DEFENCE);
					}

					if( pSCRAMBLE_TEAM )
					{
						CRectangle rtEmblem;
						m_srfMyTeam_NoEmblem.GetRect(rtEmblem);
						GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
															rtEmblem.left, rtEmblem.top,
															pSCRAMBLE_TEAM->awcGuildName, SDIALOG_SCRAMBLE_STATUS );

						m_eInfoWindowType = INFO_WINDOW_TYPE_DOJO_NAME_IN_LOWER_EMBLEM;
					}
				}
			}
		}

		return;
	}

	m_eInfoWindowType = INVALID_INFO_WINDOW_TYPE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CScrambleStatusGui::OnMouseLeave(gui::CComponent* pComponent)
{
	m_eInfoWindowType = INVALID_INFO_WINDOW_TYPE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CScrambleStatusGui::OnPaint()
{
	m_srfEnemyTeam_Background	.Render();
	m_srfEnemyTeam_GauseBack	.Render();
	m_srfStatus_Background		.Render();
	m_srfMyTeam_Background		.Render();
	m_srfMyTeam_GauseBack		.Render();
	

	if( m_pEnemyTeamEmblem && m_pEnemyTeamEmblem->GetTexture() )
		m_pEnemyTeamEmblem->Render();
	else
		m_srfEnemyTeam_NoEmblem.Render();


	if( m_pMyTeamEmblem && m_pMyTeamEmblem->GetTexture() )
		m_pMyTeamEmblem->Render();
	else
		m_srfMyTeam_NoEmblem.Render();
}

RwInt32 CScrambleStatusGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		Show(false);
		CheckInfoWindow();
	}

	return 1;
}

VOID CScrambleStatusGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CScrambleStatusGui::HandleEvents");

	if( msg.Id == g_EventDojoNotify )
	{
		SNtlEventDojo* pEvent = reinterpret_cast<SNtlEventDojo*>( msg.pData );

		switch(pEvent->byDojoEvent)
		{
		case DOJO_EVENT_SCRAMBLE_SCORE:
			{
				WCHAR awcBuffer[64];
				sDOJO_SCRAMBLE_POINT* pDOJO_SCRAMBLE_POINT = (sDOJO_SCRAMBLE_POINT*)pEvent->pExData;

				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
					NTL_RETURNVOID();

				CNtlDojo*		pDojo			= pAvatar->GetDojo();
				sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();
				if( !pSCRAMBLE_INFO )
					NTL_RETURNVOID();

				if( SCRAMBLE_TEAM_OFFENCE == pSCRAMBLE_INFO->eAvatarTeam )
				{
					// 우리 유파 정보
					m_pMyTeam_PeopleCount	->Format("%d / %d", pDOJO_SCRAMBLE_POINT->byAttCount, DBO_DOJO_SCRAMBLE_MAX_MEMBER);
					m_pMyTeam_SealCount		->SetText(pDOJO_SCRAMBLE_POINT->byAttSealCount);
					swprintf_s(awcBuffer, 64, L"%d", pDOJO_SCRAMBLE_POINT->dwAttCharge);
					m_pMyTeam_PointGause->SetToolTip(awcBuffer);
					m_pMyTeam_PointGause->SetPos(pDOJO_SCRAMBLE_POINT->dwAttCharge);
					m_pMyTeam_Point->SetText((RwUInt32)pDOJO_SCRAMBLE_POINT->dwAttPoint);
					
					// 상대 유파 정보
					m_pEnemyTeam_PeopleCount->Format("%d / %d", pDOJO_SCRAMBLE_POINT->byDefCount, DBO_DOJO_SCRAMBLE_MAX_MEMBER);
					m_pEnemyTeam_SealCount	->SetText(pDOJO_SCRAMBLE_POINT->byDefSealCount);
					swprintf_s(awcBuffer, 64, L"%d", pDOJO_SCRAMBLE_POINT->dwDefCharge);
					m_pEnemyTeam_PointGause->SetToolTip(awcBuffer);
					m_pEnemyTeam_PointGause->SetPos(pDOJO_SCRAMBLE_POINT->dwDefCharge);
					m_pEnemyTeam_Point->SetText((RwUInt32)pDOJO_SCRAMBLE_POINT->dwDefPoint);
				}
				else
				{
					// 상대 유파 정보
					m_pEnemyTeam_PeopleCount	->Format("%d / %d", pDOJO_SCRAMBLE_POINT->byAttCount, DBO_DOJO_SCRAMBLE_MAX_MEMBER);
					m_pEnemyTeam_SealCount		->SetText(pDOJO_SCRAMBLE_POINT->byAttSealCount);
					swprintf_s(awcBuffer, 64, L"%d", pDOJO_SCRAMBLE_POINT->dwAttCharge);
					m_pEnemyTeam_PointGause->SetToolTip(awcBuffer);
					m_pEnemyTeam_PointGause->SetPos(pDOJO_SCRAMBLE_POINT->dwAttCharge);
					m_pEnemyTeam_Point->SetText((RwUInt32)pDOJO_SCRAMBLE_POINT->dwAttPoint);

					// 우리 유파 정보
					m_pMyTeam_PeopleCount->Format("%d / %d", pDOJO_SCRAMBLE_POINT->byDefCount, DBO_DOJO_SCRAMBLE_MAX_MEMBER);
					m_pMyTeam_SealCount	->SetText(pDOJO_SCRAMBLE_POINT->byDefSealCount);
					swprintf_s(awcBuffer, 64, L"%d", pDOJO_SCRAMBLE_POINT->dwDefCharge);
					m_pMyTeam_PointGause->SetToolTip(awcBuffer);
					m_pMyTeam_PointGause->SetPos(pDOJO_SCRAMBLE_POINT->dwDefCharge);
					m_pMyTeam_Point->SetText((RwUInt32)pDOJO_SCRAMBLE_POINT->dwDefPoint);
				}

				

				break;
			}
		case DOJO_EVENT_SCRAMBLE_CHANGE_SEAL_OWNER:
			{
				TBLIDX		dojoTblidx		= pEvent->uiParam;
				TBLIDX		idxObject		= *(TBLIDX*)pEvent->pExData;

				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
					NTL_RETURNVOID();

				CNtlDojo*		pDojo			= pAvatar->GetDojo();
				sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

				if( pSCRAMBLE_INFO->uiScrambleDojoTableIndex != dojoTblidx )
					NTL_RETURNVOID();

				MAP_SCRAMBLE_SEAL::iterator it_ScrambleSealInfo = pSCRAMBLE_INFO->mapScrambleSeal.find(idxObject);
				if( it_ScrambleSealInfo == pSCRAMBLE_INFO->mapScrambleSeal.end() )
					NTL_RETURNVOID();			


				sSCRAMBLE_SEAL_INFO*	pSCRAMBLE_SEAL_INFO	= it_ScrambleSealInfo->second;
				eDBO_TEAM				eDBOTeam			= Logic_GetScrambleTeam(pSCRAMBLE_SEAL_INFO->eState);

				RwUInt8 byIndex = 0;
				MAP_SCRAMBLE_SEAL::iterator it_SrambleSeal = pSCRAMBLE_INFO->mapScrambleSeal.begin();
				for( ; it_SrambleSeal != pSCRAMBLE_INFO->mapScrambleSeal.end() ; ++it_SrambleSeal )
				{
					if( idxObject == it_SrambleSeal->first )
					{
						SetSealState(byIndex, eDBOTeam);
						break;
					}

					++byIndex;
				}
				
				break;
			}
		}
	}
	else if( msg.Id == g_EventNotifyGuild )
	{
		SNtlEventNotifyGuild* pEvent = reinterpret_cast<SNtlEventNotifyGuild*>( msg.pData );

		if( SLGE_CHANGE_GUILD_EMBLEM != pEvent->iMessage )
			NTL_RETURNVOID();		

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			NTL_RETURNVOID();

		CNtlDojo*		pDojo			= pAvatar->GetDojo();
		sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();
		if( !pSCRAMBLE_INFO )
			NTL_RETURNVOID();

		sEmblemFactor emblemFactor;
		if( FALSE == Logic_GetEmblemFactor(&emblemFactor, GetNtlSLGlobal()->GetSobAvatar()) )
			NTL_RETURNVOID();


		if( SCRAMBLE_TEAM_OFFENCE == pSCRAMBLE_INFO->eAvatarTeam )
			SetEmblem(FALSE, &emblemFactor);
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pEvent->eWorldConcept != WORLD_PLAY_DOJO_SCRAMBLE )
			NTL_RETURNVOID();

		CheckTeleportButton();

		if( WORLD_STATE_ENTER == pEvent->uiState )
		{
			for( RwUInt8 i = 0 ; i < dNUM_SEAL ; ++i )
				m_pStatus_Seal[i]->Show(true);
		}
		else if( WORLD_STATE_EXIT == pEvent->uiState ||
				 WORLD_STATE_NONE == pEvent->uiState )
		{
			for( RwUInt8 i = 0 ; i < dNUM_SEAL ; ++i )
				m_pStatus_Seal[i]->Show(false);
		}
	}	
	else if( msg.Id == g_EventTeleportConfirmRes )
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
		{
			DBO_FAIL("Not exist avatar instance");
			NTL_RETURNVOID();
		}

		CNtlDojo*		pDojo			= pAvatar->GetDojo();
		sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

		SDboEventTeleportConfirmRes* pResult = reinterpret_cast<SDboEventTeleportConfirmRes*>(msg.pData);
		if( pSCRAMBLE_INFO->byTeleportProposalIndex == pResult->byTeleportIndex )
			pSCRAMBLE_INFO->byTeleportProposalIndex = TELEPORT_TYPE_INVALID;
	}
	else if( msg.Id == g_EventEndterWorld )
	{
		if( FALSE == GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
			NTL_RETURNVOID();

		InitSealState();
	}
	else if( msg.Id == g_EventDojoStateNotify )
	{
		SNtlEventDojoMessage* pEvent = reinterpret_cast<SNtlEventDojoMessage*>( msg.pData );

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			NTL_RETURNVOID();

		CNtlDojo*		pDojo			= pAvatar->GetDojo();
		sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();
		if( !pSCRAMBLE_INFO )
			NTL_RETURNVOID();

		if( pSCRAMBLE_INFO->uiScrambleDojoTableIndex != pEvent->uiDojoTableIndex )
			NTL_RETURNVOID();


		SetScrambleBGM(pEvent->byDojoState);
	}

	NTL_RETURNVOID();
}