#include "precomp_dboclient.h"

// core
#include "TargetHpGui.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation 
#include "NtlSLEvent.h"
#include "NtlSobNpc.h"
#include "NtlSobPlayer.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSobCharProxy.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobMonsterAttr.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobPetAttr.h"
#include "NtlSobBuff.h"
#include "NtlSobBuffAttr.h"
#include "NtlSobBuffIcon.h"
#include "NtlSobCharProxy.h"
#include "NtlSobItemAttr.h"
#include "NtlSobMonster.h"
#include "NtlSLApi.h"
#include "TableContainer.h"
#include "NtlWorldConcept.h"
#include "NtlWorldConceptTB.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobTriggerObjectAttr.h"
#include "NtlSLDojo.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupScouter.h"

// GameTable
#include "NPCTable.h"
#include "MobTable.h"
#include "ItemTable.h"
#include "ObjectTable.h"

// Dbo
#include "DboEvent.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"
#include "DboGlobal.h"
#include "DialogManager.h"
#include "DboEventGenerator.h"
#include "DboPacketGenerator.h"
#include "GUISoundDefine.h"
#include "BuffDispObject.h"
#include "PopupManager.h"
#include "InfoWndManager.h"
#include "CursorManager.h"



#define TARGETSTATUS_UPDATE_INFOWND_TIME 1.0f

#define TARGETSTATUS_PC_WEAPON_ATTR_POS_X	189
#define TARGETSTATUS_PC_WEAPON_ATTR_POS_Y	29
#define TARGETSTATUS_PC_ARMOR_ATTR_POS_X	176
#define	TARGETSTATUS_PC_ARMOR_ATTR_POS_Y	51
#define TARGETSTATUS_MOB_BATTLE_ATTR_POS_X	TARGETSTATUS_PC_WEAPON_ATTR_POS_X
#define	TARGETSTATUS_MOB_BATTLE_ATTR_POS_Y	TARGETSTATUS_PC_WEAPON_ATTR_POS_Y


CTargetHpGui::CTargetHpGui()
{
	m_pTargetSobObj = NULL;
	m_pTargetsTargetSobObj = NULL;
}

CTargetHpGui::CTargetHpGui(const RwChar *pName)
:CNtlPLGui(pName)
{
	m_pTargetSobObj = NULL;	
	m_pTargetsTargetSobObj = NULL;
}

CTargetHpGui::~CTargetHpGui()
{
	Destroy();
}

RwBool CTargetHpGui::Create(VOID)
{
	NTL_FUNCTION("CTargetHpGui::Create");

	if(!CNtlPLGui::Create("", "gui\\StatusBar.srf", "gui\\TargetStatus.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgTargetStatus");
	m_pThis->SetPriority(dDIALOGPRIORITY_SIDE_DIALOG);

	m_psttNickName = (gui::CStaticBox*)GetComponent( "sttNickName" );
	m_psttJobName = (gui::CStaticBox*)GetComponent( "sttJobName" );
	m_ppgbHp = (gui::CProgressBar*)GetComponent( "pgbTargetLpGauge" );
	m_ppgbEp = (gui::CProgressBar*)GetComponent( "pgbTargetEpGauge" );
	m_sttHp = (gui::CStaticBox*)GetComponent( "sttTargetLp" );
	m_sttEp	= (gui::CStaticBox*)GetComponent( "sttTargetEp" );
	m_pbtnMenu = (gui::CButton*)GetComponent( "btnMenuButton" );
	m_pbtnShareTarget = (gui::CButton*)GetComponent( "btnShareTargetButton" );
	m_ppnlHp = (gui::CPanel*)GetComponent( "pnlTargetLpBack" );
	m_ppnlEp = (gui::CPanel*)GetComponent( "pnlTargetEpBack" );

	m_ppnlRaceIcon = (gui::CPanel*)GetComponent( "pnlRaceIcon" );
	m_ppnlMobGrade = (gui::CPanel*)GetComponent( "pnlMobGrade" );
	m_ppnlNpcJob = (gui::CPanel*)GetComponent( "pnlNpcJob" );

	m_ppnlSelfattack = (gui::CPanel*)GetComponent("pnlSelfattack");
	m_ppnlfirstattack = (gui::CPanel*)GetComponent("pnlfirstattack");
	m_pstSelfattack = (gui::CStaticBox*)GetComponent("stSelfattack");
	m_pstfirstattack = (gui::CStaticBox*)GetComponent("stfirstattack");
	m_ppnlSelfattack->Show(false);
	m_ppnlfirstattack->Show(false);

	m_ppnlPlayerLevel = (gui::CPanel*)GetComponent("pnlPlayerLevel");
	m_pstPlayerLevel = (gui::CStaticBox*)GetComponent("stPlayerLevel");

	// target of target
	m_psttTargetName = (gui::CStaticBox*)GetComponent("sttTargetName");
	m_ppnlTargetIcon = (gui::CPanel*)GetComponent("pnlTargetIcon");
	m_pbtTargetIcon = (gui::CPanel*)GetComponent("btTargetIcon");
	m_ppnlTargetIcon->SetToolTip(GetDisplayStringManager()->GetString("DST_TARGET_OF_TARGET"));
	m_ppnlTargetLpEdBarBack = (gui::CPanel*)GetComponent("pnlTargetLpEdBarBack");
	m_ppgbTargetAggroLpGauge = (gui::CProgressBar*)GetComponent("pgbTargetAggroLpGauge");
	m_ppgbTargetAggroEpGauge = (gui::CProgressBar*)GetComponent("pgbTargetAggroEpGauge");

	m_psttTargetName->Show(false);
	m_psttTargetName->SetPriority(dDIALOGPRIORITY_SIDE_DIALOG);
	m_ppnlTargetIcon->Show(false);
	m_ppnlTargetIcon->SetPriority(dDIALOGPRIORITY_SIDE_DIALOG);

	// Buff
	m_pBuff = NTL_NEW CBuffDispGui;
	NTL_ASSERT( m_pBuff, "CTargetHpGui::Create : Buff is not allocated. Not enough memory" );
	BYTE byShowFlag = MAKE_BIT_FLAG(BUFF_TYPE_BLESS) + MAKE_BIT_FLAG(BUFF_TYPE_CURSE);
	m_pBuff->Create(byShowFlag, CBuffDispGui::NORMAL_ICONSIZE, 5, m_ppnlTargetIcon->GetHeight() + 3, DIALOG_TARGER_UI );
	m_pBuff->Show( false );

	m_pbtnMenu->SetToolTip( GetDisplayStringManager()->GetString( "DST_TARGET_MENU" ) );
	m_pbtnShareTarget->SetToolTip( GetDisplayStringManager()->GetString( "DST_TARGET_SHARED_TARGET" ) );
	m_pbtnShareTarget->Show( false );
			    	
	Show(false);

	GetNtlGuiManager()->AddUpdateFunc( this );

	// Signal
	m_slotMove = m_pThis->SigMove().Connect( this, &CTargetHpGui::OnMove );
	m_slotClickMenuBtn = m_pbtnMenu->SigClicked().Connect( this, &CTargetHpGui::OnClickMenuBtn );
	m_slotClickShareTargetBtn = m_pbtnShareTarget->SigClicked().Connect( this, &CTargetHpGui::OnClickShareTargetBtn );


	m_slotMouseUpTargetsTarget = m_pbtTargetIcon->SigMouseUp().Connect(this, &CTargetHpGui::OnMouseUpTargetsTarget);
	m_slotMouseEnterTargetsTarget = m_pbtTargetIcon->SigMouseEnter().Connect(this, &CTargetHpGui::OnMouseEnterTargetsTarget);
	m_slotMouseLeaveTargetsTarget = m_pbtTargetIcon->SigMouseLeave().Connect(this, &CTargetHpGui::OnMouseLeaveTargetsTarget);


	// event 등록.
	LinkMsg(g_EventSobTargetInfoUpdate, 0);
	LinkMsg(g_EventSobTargetChanged);
	LinkMsg(g_EventSobInfoUpdate, 0);
	LinkMsg(g_EventCharObjDelete, 0);
	LinkMsg(g_EventIconPopupResult, 0);
	LinkMsg(g_EventTargetStatusRefresh, 0);
	LinkMsg(g_EventPartyLeaderChange);
	LinkMsg(g_EventPartyLeave);
	LinkMsg(g_EventDojoNotify);
	LinkMsg(g_EventTObjectUpdateState, 0);
	LinkMsg(g_EventChangeCharName);
				
	NTL_RETURN(TRUE);
}

void CTargetHpGui::Destroy(VOID)
{
	NTL_FUNCTION("CTargetHpGui::Destroy");

	m_pTargetSobObj = NULL;

	if( m_pBuff )
	{
		m_pBuff->Destroy();
		NTL_DELETE( m_pBuff );
	}

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg(g_EventSobTargetInfoUpdate);
	UnLinkMsg(g_EventSobTargetChanged);
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventIconPopupResult);
	UnLinkMsg(g_EventTargetStatusRefresh);
	UnLinkMsg(g_EventPartyLeaderChange);
	UnLinkMsg(g_EventPartyLeave);
	UnLinkMsg(g_EventDojoNotify);
	UnLinkMsg(g_EventTObjectUpdateState);
	UnLinkMsg(g_EventChangeCharName);
	
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTargetHpGui::Update( RwReal fElapsed  )
{
	if (m_pTargetSobObj && m_pBuff && m_pBuff->IsShow())
	{
		m_pBuff->Update(fElapsed);
	}
}

RwInt32 CTargetHpGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		// WorldConcept Check
		CNtlWorldConceptTB*	pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		if( pTBWorldConcept )
		{
			// Match Check
			if( pTBWorldConcept->GetMatchType() == MATCHWORLD_TYPE_MAJOR_MATCH || 
				pTBWorldConcept->GetMatchType() == MATCHWORLD_TYPE_FINAL_MATCH )
			{
				// Am i observer on TBMatch? ( observer is cant open target gui on major and final match )
				stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
				if( !pMember )
				{
					return -1;
				}
			}
		}
	}

	Show(bOpen);	

	if( !bOpen )
	{
		if(m_pBuff->IsShow())
			m_pBuff->Show(false);

		m_psttTargetName->Show(false);
		m_ppnlTargetIcon->Show(false);

		if( GetPopupManager()->IsVisiblePopupGui( PLACE_SUB_TARGETUI ) )
			GetPopupManager()->AllPopupClose();
	}

	return 1;
}

void CTargetHpGui::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CTargetHpGui::HandleEvents");

	if(pMsg.Id == g_EventSobTargetInfoUpdate)
	{
		SNtlEventSobTargetInfoUpdate *pTartgetUpdate = reinterpret_cast<SNtlEventSobTargetInfoUpdate*>(pMsg.pData); 
		
		if(pTartgetUpdate->bShow)
		{
			if( m_pTargetSobObj && m_pTargetSobObj->GetSerialID() == pTartgetUpdate->hSerialId )
				return;

			CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pTartgetUpdate->hSerialId );
			if (!pSobObj)
				return;

			m_pTargetSobObj = pSobObj;
			if( !TargetUpdateProc() )
			{
				m_pTargetSobObj = NULL;

				m_pTargetsTargetSobObj = NULL;
				m_psttTargetName->Show(false);
				m_ppnlTargetIcon->Show(false);

				return;
			}

			//Initialization at initial targeting.
			if (m_pTargetSobObj->GetSerialID() != Logic_GetAvatarHandle())
			{
				m_pBuff->ClearBuff();
			}
		}
		else
		{
			if (m_pTargetSobObj && m_pTargetSobObj->GetSerialID() != Logic_GetAvatarHandle())
			{
				m_pBuff->ClearBuff();
			}

			m_pTargetSobObj = NULL;

			// hide targets target
			m_pTargetsTargetSobObj = NULL;
			m_psttTargetName->Show(false);
			m_ppnlTargetIcon->Show(false);

			GetDialogManager()->CloseDialog( DIALOG_TARGER_UI, FALSE );
		}
	}
	else if (pMsg.Id == g_EventSobTargetChanged)
	{
		SNtlEventSobTargetChanged *pTarget = reinterpret_cast<SNtlEventSobTargetChanged*>(pMsg.pData);

		if (!m_pTargetSobObj || m_pTargetSobObj->GetSerialID() != pTarget->hSerialId) // Check if we really target that object. If not, should we target that object?
			return;

		if (Logic_GetAvatarHandle() == pTarget->hSerialId)// if I click myself, dont show myself as targets target.
			return;

		if (pTarget->hTarget == INVALID_HOBJECT) // if target has no more target, hide it.
		{
			m_psttTargetName->Show(false);
			m_ppnlTargetIcon->Show(false);
		}
		else
		{
			CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject(pTarget->hTarget);
			if (!pSobObj)
				return;

			m_pTargetsTargetSobObj = pSobObj;

			RwUInt32 uiHp = Logic_GetLp(pSobObj);
			RwUInt32 uiEp = Logic_GetEp(pSobObj);
			RwUInt32 uiMaxHp = Logic_GetMaxLp(pSobObj);
			RwUInt32 uiMaxEp = Logic_GetMaxEp(pSobObj);

			m_ppgbTargetAggroLpGauge->SetRange(0, uiMaxHp);
			m_ppgbTargetAggroLpGauge->SetPos(uiHp);
			m_ppgbTargetAggroEpGauge->SetRange(0, uiMaxEp);
			m_ppgbTargetAggroEpGauge->SetPos(uiEp);


			if (pSobObj->GetClassID() == SLCLASS_AVATAR || pSobObj->GetClassID() == SLCLASS_PLAYER)
			{
				CNtlSobAvatarAttr* pSobAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pSobObj->GetSobAttr());
				SetTargetRaceIcon(pSobAvatarAttr->GetRace());

				m_psttTargetName->SetText(Logic_GetName(pSobObj));
			}
			else
			{
				if (pSobObj->GetClassID() == SLCLASS_NPC)
				{
					CNtlSobNpcAttr* pAttr = reinterpret_cast<CNtlSobNpcAttr*>(pSobObj->GetSobAttr());
					sNPC_TBLDAT* pNpcData = pAttr->GetNpcTbl();

					SetTargetMobNpcTypeIcon(pNpcData->byNpcType);
				}
				else if (pSobObj->GetClassID() == SLCLASS_PET)
				{
					CNtlSobPetAttr* pAttr = reinterpret_cast<CNtlSobPetAttr*>(pSobObj->GetSobAttr());
					sNPC_TBLDAT* pPetData = pAttr->GetNpcTbl();

					SetTargetMobNpcTypeIcon(pPetData->byNpcType);
				}
				else if (pSobObj->GetClassID() == SLCLASS_MONSTER)
				{
					CNtlSobMonsterAttr* pSobMosterAttr = reinterpret_cast<CNtlSobMonsterAttr*>(pSobObj->GetSobAttr());
					sMOB_TBLDAT* pMobData = pSobMosterAttr->GetMobTbl();

					SetTargetMobNpcTypeIcon(pMobData->byMob_Type);
				}
				else if (pSobObj->GetClassID() == SLCLASS_TRIGGER_OBJECT)
				{
					SetTargetMobNpcTypeIcon(MOB_TYPE_ROBOT);
				}

				if (Logic_IsValidScouterEquipped(false))
				{
					m_psttTargetName->SetText(Logic_GetName(pSobObj));
				}
				else
				{
					m_psttTargetName->SetText(L"???");
				}
			}

			m_psttTargetName->Show(true);
			m_ppnlTargetIcon->Show(true);
		}
	}
	else if(pMsg.Id == g_EventTargetStatusRefresh)
	{
		if( m_pTargetSobObj )
			TargetUpdateProc();
	}
	else if(pMsg.Id == g_EventSobInfoUpdate)
	{
		SNtlEventSobInfoUpdate *pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>(pMsg.pData);

		if (m_pTargetsTargetSobObj && m_pTargetsTargetSobObj->GetSerialID() == pUpdate->hSerialId)
		{
			RwUInt32 uiHp = Logic_GetLp(m_pTargetsTargetSobObj);
			RwUInt32 uiEp = Logic_GetEp(m_pTargetsTargetSobObj);
			RwUInt32 uiMaxHp = Logic_GetMaxLp(m_pTargetsTargetSobObj);
			RwUInt32 uiMaxEp = Logic_GetMaxEp(m_pTargetsTargetSobObj);

			m_ppgbTargetAggroLpGauge->SetRange(0, uiMaxHp);
			m_ppgbTargetAggroLpGauge->SetPos(uiHp);
			m_ppgbTargetAggroEpGauge->SetRange(0, uiMaxEp);
			m_ppgbTargetAggroEpGauge->SetPos(uiEp);

		}
		// dont do "else if" otherwise buffs etc wont be shown and can cause a crash.
		if (m_pTargetSobObj && m_pTargetSobObj->GetSerialID() == pUpdate->hSerialId)
		{
			if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR )
			{
				//const WCHAR* pName = Logic_GetName( m_pTargetSobObj );
				RwUInt32 uiHp = Logic_GetLp( m_pTargetSobObj );
				RwUInt32 uiMaxHp = Logic_GetMaxLp( m_pTargetSobObj );
				RwUInt32 uiEp = Logic_GetEp( m_pTargetSobObj );
				RwUInt32 uiMaxEp = Logic_GetMaxEp( m_pTargetSobObj );
				
				m_ppgbHp->SetRange( 0, uiMaxHp );
				m_ppgbHp->SetPos(uiHp);

				m_ppgbEp->SetRange( 0, uiMaxEp );
				m_ppgbEp->SetPos(uiEp);


				if( m_pTargetSobObj->GetClassID() == SLCLASS_AVATAR || m_pTargetSobObj->GetClassID() == SLCLASS_MONSTER || m_pTargetSobObj->GetClassID() == SLCLASS_PLAYER)
				{
					if(Logic_IsValidScouterEquipped( false ) )
					{
						m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
						m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );						
					}
					else
					{
						const WCHAR* szLPEP = GetDisplayStringManager()->GetString( "DST_STATUS_UNIDENTIFIED_LPEP" );
						m_sttHp->SetText( szLPEP );
						m_sttEp->SetText( szLPEP );						
					}
				}
				else
				{
					m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
					m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );		
				}
			}

			if (pUpdate->uiUpdateType & EVENT_AIUT_ATTR_LEVELUP)
			{
				if (m_pTargetSobObj->GetClassID() == SLCLASS_MONSTER || m_pTargetSobObj->GetClassID() == SLCLASS_PLAYER)
				{
					m_pstPlayerLevel->SetText(Logic_GetLevel(m_pTargetSobObj));
				}
			}
			
			if (m_pTargetSobObj->GetSerialID() != Logic_GetAvatarHandle())
			{
				if (pUpdate->uiUpdateType & EVENT_AIUT_BUFF)
					m_pBuff->SetBuffAll(m_pTargetSobObj->GetSerialID());
			}
		}
		else
		{
		}
	}
	else if(pMsg.Id == g_EventCharObjDelete)
	{
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( pMsg.pData );
		
		if( m_pTargetSobObj && m_pTargetSobObj->GetSerialID() == *pDeleteSerial )
		{
			if (m_pTargetSobObj->GetSerialID() != Logic_GetAvatarHandle())
			{
				m_pBuff->ClearBuff();
			}

			m_pTargetSobObj = NULL;

			// hide targets target
			m_pTargetsTargetSobObj = NULL;
			m_psttTargetName->Show(false);
			m_ppnlTargetIcon->Show(false);

			GetDialogManager()->CloseDialog( DIALOG_TARGER_UI, FALSE );			
		}
	}
	else if(pMsg.Id == g_EventIconPopupResult)
	{
		SDboEventIconPopupResult* pPacket = reinterpret_cast<SDboEventIconPopupResult*>(pMsg.pData);
		if(pPacket->nSrcPlace == PLACE_SUB_TARGETUI)
		{
			if( pPacket->nWorkId == PMW_PARTY_INVITE )
			{
				// 이미 나의 파티원이 아니라면
				if( GetNtlSLGlobal()->GetSobAvatar()->GetParty()->IsMember(pPacket->uiSerial) == false )
				{
					CNtlSob* pSob = GetNtlSobManager()->GetSobObject(pPacket->uiSerial);
					CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>(pSob);

					// 플레이어만이 파티로 초대 가능하다
					if( pPlayer->GetClassID() != SLCLASS_PLAYER)
						NTL_RETURNVOID();

					CGamePacketGenerator *pGamePacketGenerator = GetDboGlobal()->GetGamePacketGenerator();
					pGamePacketGenerator->SendPartyInvite(pPlayer->GetSerialID());
				}				
			}
			else if( pPacket->nWorkId == PMW_PARTY_LEADER_CHANGE )
			{				
				// 파티장 위임
				GetDboGlobal()->GetGamePacketGenerator()->SendPartyChangeLeader(pPacket->uiSerial);
			}
			else if(pPacket->nWorkId == PMW_PARTY_KICK_OUT)
			{
				// 파티 강퇴
				GetDboGlobal()->GetGamePacketGenerator()->SendPartyKickOut(pPacket->uiSerial);
			}
			else if( pPacket->nWorkId == PMW_PVP_REQUEST_FIGHT )
			{
				if (Logic_CanRequestFreeBattle())
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendFreeBattleChallengeReq(m_pTargetSobObj->GetSerialID());
				}
				else
					Logic_WarnningWorldConceptActionMessage( WORLD_CONCEPT_SECOND_GRADE );
			}
			else if( pPacket->nWorkId == PMW_USER_TRADE )
			{
				if(Logic_CanRequestTrade())
				{
					if( GetDboGlobal()->GetGamePacketGenerator()->SendTradeStartReq( m_pTargetSobObj->GetSerialID() ) )
					{
						CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( GetNtlSobManager()->GetSobObject(m_pTargetSobObj->GetSerialID()) );
						NTL_ASSERT(pSobPlayer, "CTargetHpGui::HandleEvents, g_EventIconPopupResult, pPacket->nWorkId == PMW_USER_TRADE, Not exist player of handle : " << m_pTargetSobObj->GetSerialID());
						CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( pSobPlayer->GetSobAttr() );
						NTL_ASSERT(pSobPlayerAttr, "CTargetHpGui::HandleEvents, g_EventIconPopupResult, pPacket->nWorkId == PMW_USER_TRADE, Not exist player attr of handle : " << m_pTargetSobObj->GetSerialID());

						// %s님에게 거래를 요청하였습니다
						GetAlarmManager()->FormattedAlarmMessage("DST_TRADE_REQUEST_START", FALSE, NULL, pSobPlayerAttr->GetName());
					}
				}
				else
					Logic_WarnningWorldConceptActionMessage( WORLD_CONCEPT_SECOND_GRADE );
			}
			else if( pPacket->nWorkId == PMW_GUILD_INVITE )
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendGuildInviteReq( m_pTargetSobObj->GetSerialID() );
			}
		}
	}
	else if( pMsg.Id == g_EventPartyLeaderChange )
	{
		if( Logic_I_am_PartyLeader() )
			m_pbtnShareTarget->Show( true );
		else
			m_pbtnShareTarget->Show( false );
	}
	else if( pMsg.Id == g_EventPartyLeave )
	{
		m_pbtnShareTarget->Show( false );
	}
	else if( pMsg.Id == g_EventDojoNotify )
	{
		SNtlEventDojo* pEvent = reinterpret_cast<SNtlEventDojo*>( pMsg.pData );

		if( false == GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
			NTL_RETURNVOID();

		if( !m_pTargetSobObj )
			NTL_RETURNVOID();		

		if( DOJO_EVENT_SCRAMBLE_CHANGE_SEAL_OWNER != pEvent->byDojoEvent )
			NTL_RETURNVOID();

		if( SLCLASS_TRIGGER_OBJECT != m_pTargetSobObj->GetClassID() )
			NTL_RETURNVOID();		

		CNtlSobTriggerObjectAttr*	pSobObjectAttr	= reinterpret_cast<CNtlSobTriggerObjectAttr*>( m_pTargetSobObj->GetSobAttr() );
		sOBJECT_TBLDAT*				pOBJECT_TBLDAT	= pSobObjectAttr->GetTriggerObjectTbl();

		if( false == BIT_FLAG_TEST(pOBJECT_TBLDAT->dwFunction, eDBO_TRIGGER_OBJECT_FUNC_DOJO_SEAL) )
			NTL_RETURNVOID();

		RwUInt32 idxObject = *(RwUInt32*)pEvent->pExData;

		if( idxObject != pOBJECT_TBLDAT->tblidx )
			NTL_RETURNVOID();

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			NTL_RETURNVOID();

		CNtlDojo*		pDojo			= pAvatar->GetDojo();
		sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

		MAP_SCRAMBLE_SEAL::iterator it_ScrambleSealInfo = pSCRAMBLE_INFO->mapScrambleSeal.find(idxObject);
		if( it_ScrambleSealInfo == pSCRAMBLE_INFO->mapScrambleSeal.end() )
			NTL_RETURNVOID();


		sSCRAMBLE_SEAL_INFO* pSCRAMBLE_SEAL_INFO = it_ScrambleSealInfo->second;

		SetScrambleOwner((RwUInt8)pSCRAMBLE_SEAL_INFO->eState);
	}	
	else if( pMsg.Id == g_EventTObjectUpdateState )
	{
		SNtlEventTObjectUpdateState* pEvent = reinterpret_cast<SNtlEventTObjectUpdateState*>( pMsg.pData );

		if(Logic_GetAvatarTargetHandle() != pEvent->hSerialId )
			NTL_RETURNVOID();

		if( FALSE == IsShow() )
			NTL_RETURNVOID();

		if( false == BIT_FLAG_TEST(pEvent->bySubState, TOBJECT_SUBSTATE_FLAG_SHOW) )
			GetDialogManager()->CloseDialog( DIALOG_TARGER_UI, FALSE );
	}
	else if (pMsg.Id == g_EventChangeCharName)
	{
		if (IsShow())
		{
			SNtlEventChangeCharName* pEvent = reinterpret_cast<SNtlEventChangeCharName*>(pMsg.pData);

			if (wcscmp(m_psttNickName->GetText().c_str(), pEvent->wchOldCharName) == 0)
			{
				m_psttNickName->SetText(pEvent->wchCharName);
			}

			if (m_psttTargetName->IsVisible() && wcscmp(m_psttTargetName->GetText().c_str(), pEvent->wchOldCharName) == 0)
			{
				m_psttTargetName->SetText(pEvent->wchCharName);
			}
		}
	}
	
	NTL_RETURNVOID();
}

RwBool CTargetHpGui::TargetUpdateProc(VOID)
{
	m_pbtnMenu->Show( false );

	if( m_pTargetSobObj->GetClassID() == SLCLASS_AVATAR )
	{
		m_ppnlHp->Show( true );
		m_ppnlEp->Show( true );
		m_psttJobName->Show( false );
		m_pBuff->Show( false );
		m_ppnlMobGrade->Show( false );			
		m_ppnlRaceIcon->Show( true );
		m_ppnlNpcJob->Show( false );
		m_ppnlPlayerLevel->Show(false);
		
		RwUInt32 uiHp = Logic_GetLp( m_pTargetSobObj );
		RwUInt32 uiEp = Logic_GetEp( m_pTargetSobObj );
		RwUInt32 uiMaxHp = Logic_GetMaxLp( m_pTargetSobObj );
		RwUInt32 uiMaxEp = Logic_GetMaxEp( m_pTargetSobObj );

		m_psttNickName->SetText( Logic_GetName( m_pTargetSobObj ) );

		m_ppgbHp->SetRange( 0, uiMaxHp );
		m_ppgbHp->SetPos(uiHp);
		m_ppgbEp->SetRange( 0, uiMaxEp );
		m_ppgbEp->SetPos(uiEp);
		m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
		m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );

		// 종족 아이콘
		CNtlSobAvatarAttr* pSobAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( m_pTargetSobObj->GetSobAttr() );
		SetRaceIcon( pSobAvatarAttr->GetRace() );	
	}
	else if( m_pTargetSobObj->GetClassID() == SLCLASS_PLAYER )
	{
		RwUInt32 uiHp = Logic_GetLp( m_pTargetSobObj );
		RwUInt32 uiEp = Logic_GetEp( m_pTargetSobObj );
		RwUInt32 uiMaxHp = Logic_GetMaxLp( m_pTargetSobObj );
		RwUInt32 uiMaxEp = Logic_GetMaxEp( m_pTargetSobObj );

		m_ppnlHp->Show( true );
		m_ppnlEp->Show( true );
		m_psttJobName->Show( false );
		m_ppnlMobGrade->Show( false );	
		m_ppnlRaceIcon->Show( true );
		m_ppnlNpcJob->Show( false );
		m_pbtnMenu->Show( true );
		m_ppnlPlayerLevel->Show(true);

		m_psttNickName->SetText(Logic_GetName(m_pTargetSobObj));
		
		m_ppgbHp->SetRange( 0, uiMaxHp );
		m_ppgbHp->SetPos(uiHp);
		m_ppgbEp->SetRange( 0, uiMaxEp );
		m_ppgbEp->SetPos(uiEp);
		
		CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( m_pTargetSobObj );
		CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( m_pTargetSobObj->GetSobAttr() );
		/*CNtlStorageGroupScouter* pGroupScouter = (CNtlStorageGroupScouter*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_SCOUTER );
		SMemoryScouterData* pScouterData = pGroupScouter->IsUsePlayerScouter( pSobPlayer->GetCharID() );*/

		if(Logic_IsValidScouterEquipped(false))
		{
			/*m_numPower.SetNumber( pScouterData->uiPower );
			m_ppnlPowerText->Show( true );*/

			m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
			m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );

			m_pstPlayerLevel->SetText(Logic_GetLevel(m_pTargetSobObj));

			m_pBuff->Show( true );
		}
		else
		{
			/*m_ppnlPowerText->Show( false );*/
			
			const WCHAR* szLPEP = GetDisplayStringManager()->GetString( "DST_STATUS_UNIDENTIFIED_LPEP" );
			m_sttHp->SetText( szLPEP );
			m_sttEp->SetText( szLPEP );

			m_pstPlayerLevel->SetText(L"??");

			m_pBuff->Show( false );
		}

		// 종족 아이콘		
		SetRaceIcon( pSobPlayerAttr->GetRace() );
	}
	else if( m_pTargetSobObj->GetClassID() == SLCLASS_MONSTER )
	{
		CNtlSobMonsterAttr* pSobMosterAttr = reinterpret_cast<CNtlSobMonsterAttr*>( m_pTargetSobObj->GetSobAttr() );
		sMOB_TBLDAT* pMobData = pSobMosterAttr->GetMobTbl();

		RwUInt32 uiHp = Logic_GetLp( m_pTargetSobObj );
		RwUInt32 uiEp = Logic_GetEp( m_pTargetSobObj );
		RwUInt32 uiMaxHp = Logic_GetMaxLp( m_pTargetSobObj );
		RwUInt32 uiMaxEp = Logic_GetMaxEp( m_pTargetSobObj );

		m_ppnlHp->Show( true );
		m_ppnlEp->Show( true );
		m_pBuff->Show( true );
		m_ppnlMobGrade->Show( true );
		m_ppnlRaceIcon->Show( true );

		m_psttJobName->Show( false );
		m_ppnlNpcJob->Show( false );

		m_ppnlPlayerLevel->Show(true);

		m_ppgbHp->SetRange( 0, uiMaxHp );
		m_ppgbHp->SetPos(uiHp);

		m_ppgbEp->SetRange( 0, uiMaxEp );
		m_ppgbEp->SetPos(uiEp);

		bool bCanSeeName = Logic_IsValidScouterEquipped(pMobData->bShow_Name);
		if(bCanSeeName)
		{
			m_psttNickName->SetText( Logic_GetName( m_pTargetSobObj ) );
			
			m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
			m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );		

			m_pstPlayerLevel->SetText(Logic_GetLevel(m_pTargetSobObj));

			// Mob grade
			SetMobGrade( pMobData->byGrade );
		}
		else
		{
			m_psttNickName->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_UNIDENTIFIED_NAME" ) );

			const WCHAR* szLPEP = GetDisplayStringManager()->GetString( "DST_STATUS_UNIDENTIFIED_LPEP" );
			m_sttHp->SetText( szLPEP );
			m_sttEp->SetText( szLPEP );

			m_pstPlayerLevel->SetText(L"??");

			// Mob grade
			SetMobGrade( MOB_GRADE_NORMAL );
		}

		// Race Icon
		SetMobNpcTypeIcon( pMobData->byMob_Type );	

		Logic_PlayGUISound( GSD_SYSTEM_ENEMY_SELECT );
	}	
	else if( m_pTargetSobObj->GetClassID() == SLCLASS_NPC )
	{
		m_psttNickName->SetText( Logic_GetName( m_pTargetSobObj ) );

		RwUInt32 uiHp = Logic_GetLp( m_pTargetSobObj );
		RwUInt32 uiMaxHp = Logic_GetMaxLp( m_pTargetSobObj );
		
		m_ppnlEp->Show( false );
		m_psttJobName->Show( true );
		m_pBuff->Show( false );
		m_ppnlMobGrade->Show( false );
		m_ppnlRaceIcon->Show( true );
		m_ppnlNpcJob->Show( true );
		m_ppnlPlayerLevel->Show(false);

		CNtlSobNpcAttr* pAttr = reinterpret_cast<CNtlSobNpcAttr*>( m_pTargetSobObj->GetSobAttr() );
		if( pAttr->GetNpcTbl()->dwFunc_Bit_Flag & NPC_FUNC_FLAG_DISCLOSE_LP )
		{
			m_sttHp->Format("%u / %u", uiHp, uiMaxHp);
			m_ppnlHp->Show( true );
			m_ppgbHp->SetRange( 0, uiMaxHp );
			m_ppgbHp->SetPos(uiHp);
			m_psttJobName->SetPosition( 18, 51 );
		}		
		else
		{
			m_ppnlHp->Show( false );
			m_psttJobName->SetPosition( 18, 41 );
		}

		const WCHAR* pJobText = Logic_GetNPCJobName( pAttr->GetNpcTbl()->byJob );
		if (pJobText)
		{
			m_psttJobName->SetText(pJobText);
			m_ppnlNpcJob->SetToolTip(std::wstring(pJobText));
		}

		// 종족 아이콘
		sNPC_TBLDAT* pNpcData = pAttr->GetNpcTbl();

		SetMobNpcTypeIcon( pNpcData->byNpcType );
		SetNPCJobIcon( pNpcData->byJob );
	}
	else if( m_pTargetSobObj->GetClassID() == SLCLASS_PET )
	{
		m_psttNickName->SetText( Logic_GetName( m_pTargetSobObj ) );

		RwUInt32 uiHp = Logic_GetLp( m_pTargetSobObj );
		RwUInt32 uiEp = Logic_GetEp( m_pTargetSobObj );
		RwUInt32 uiMaxHp = Logic_GetMaxLp( m_pTargetSobObj );
		RwUInt32 uiMaxEp = Logic_GetMaxEp( m_pTargetSobObj );

		m_ppnlHp->Show( true );
		m_ppnlEp->Show( true );
		m_psttJobName->Show( false );
		m_ppnlMobGrade->Show( false );	
		m_ppnlRaceIcon->Show( true );
		m_ppnlNpcJob->Show( false );
		m_pBuff->Show( true );
		m_ppnlPlayerLevel->Show(false);

		m_ppgbHp->SetRange( 0, uiMaxHp );
		m_ppgbHp->SetPos(uiHp);
		m_ppgbEp->SetRange( 0, uiMaxEp );
		m_ppgbEp->SetPos(uiEp);

		m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
		m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );	

		// 종족 아이콘( Pet 은 NPC )
		CNtlSobPetAttr* pPetAttr = reinterpret_cast<CNtlSobPetAttr*>( m_pTargetSobObj->GetSobAttr() );
		sNPC_TBLDAT* pPetData = pPetAttr->GetNpcTbl();

		SetMobNpcTypeIcon( pPetData->byNpcType );
	}
	else if( m_pTargetSobObj->GetClassID() == SLCLASS_TRIGGER_OBJECT )
	{
		CNtlSobTriggerObject* pTriggerObject = reinterpret_cast<CNtlSobTriggerObject*>( m_pTargetSobObj );
		CNtlSobTriggerObjectAttr* pTriggerObjectAttr = reinterpret_cast<CNtlSobTriggerObjectAttr*>( pTriggerObject->GetSobAttr() );
		sOBJECT_TBLDAT* pOBJECT_TBLDAT = pTriggerObjectAttr->GetTriggerObjectTbl();

		if( pOBJECT_TBLDAT->dwFunction & eDBO_TRIGGER_OBJECT_FUNC_DOJO_SEAL )
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( !pAvatar )
				return FALSE;

			CNtlDojo*		pDojo			= pAvatar->GetDojo();
			sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

			MAP_SCRAMBLE_SEAL::iterator it_ScrambleSealInfo = pSCRAMBLE_INFO->mapScrambleSeal.find( pOBJECT_TBLDAT->tblidx );
			if( it_ScrambleSealInfo == pSCRAMBLE_INFO->mapScrambleSeal.end() )
				return FALSE;

			std::wstring			wstrObjectName			= Logic_GetName(m_pTargetSobObj);
			sSCRAMBLE_SEAL_INFO*	pSCRAMBLE_SEAL_INFO	= it_ScrambleSealInfo->second;

			//const WCHAR* pwcText = GetDisplayStringManager()->GetString();
			//const WCHAR* pwcState;			

			/*m_ppnlPowerText->Show( false );*/
			m_ppnlHp->Show( false );
			m_ppnlEp->Show( false );
			m_psttJobName->Show( true );
			m_pBuff->Show( false );
			m_ppnlMobGrade->Show( false );
			m_ppnlRaceIcon->Show( true );
			m_ppnlNpcJob->Show( true );
			m_ppnlPlayerLevel->Show(false);

			m_psttNickName->SetText( wstrObjectName.c_str() );
			m_psttJobName->SetPosition( 18, 41 );
			m_ppnlNpcJob->SetToolTip( wstrObjectName.c_str() );

			SetScrambleOwner((RwUInt8)pSCRAMBLE_SEAL_INFO->eState);

			//m_psttJobName->Format(pwcText,);
			SetMobNpcTypeIcon( MOB_TYPE_ROBOT );
			SetObjectIcon( pOBJECT_TBLDAT->dwFunction );
		}
		else
			return FALSE;
	}
	else 
	{
		// Pet 말고 다른 것 출력할게 있나? 
		NTL_ASSERT( FALSE, "CTargetHpGui::TargetUpdateProc - Invalid SobObj's Class ID" );
	}

	GetDialogManager()->OpenDialog( DIALOG_TARGER_UI );
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// signal function

VOID CTargetHpGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	CRectangle rect = m_pThis->GetScreenRect();
	m_pBuff->SetMove( rect );

	m_ppnlTargetIcon->SetPosition(rect.right + 10, rect.top);
	m_psttTargetName->SetPosition(rect.right + 45, rect.top + 24);
}

VOID CTargetHpGui::OnClickMenuBtn( gui::CComponent* pComponent )
{
	RwInt32 iAdjust = 37;
	CRectangle rect = m_pThis->GetScreenRect();
	CDboEventGenerator::IconPopupShow( TRUE, m_pTargetSobObj->GetSerialID(), PLACE_SUB_TARGETUI, 0, rect.right - iAdjust, -iAdjust );	
}

VOID CTargetHpGui::OnClickShareTargetBtn( gui::CComponent* pComponent )
{
	RwInt32 iAdjust = 37;
	CRectangle rect = m_pThis->GetScreenRect();
	CDboEventGenerator::IconPopupShow( TRUE, m_pTargetSobObj->GetSerialID(), PLACE_SUB_TARGETUI, 1, rect.right - iAdjust, -iAdjust );		// nSrcSlotId로  ShareTarget인지 Menu인지 구분.
}

VOID CTargetHpGui::OnMouseUpTargetsTarget(const CKey & key)
{
	if (m_pTargetsTargetSobObj)
	{
		Logic_SobTarget(m_pTargetsTargetSobObj->GetSerialID(), INVALID_BYTE);

		if (GetNtlSLGlobal()->IsSkillSelectMode())
		{
			Logic_UseProc(GetNtlSLGlobal()->GetSkillSelectModeHandle(), INVALID_BYTE, Logic_GetAvatarTargetHandle(), false);

			if (GetNtlSLGlobal()->IsSkillSelectMode())
			{
				CNtlSLEventGenerator::ActionSkillSelectMode(false, INVALID_SERIAL_ID);
			}
		}
	}
}

VOID CTargetHpGui::OnMouseEnterTargetsTarget(gui::CComponent * pComponent)
{
	if (GetNtlSLGlobal()->IsSkillSelectMode() == false)
		GetCursorManager()->SetDesignatedCursor(CCursorManager::CS_BATTLE_TARGET);
}

VOID CTargetHpGui::OnMouseLeaveTargetsTarget(gui::CComponent * pComponent)
{
	if(GetCursorManager()->GetCursorState() == CCursorManager::CS_BATTLE_TARGET)
		GetCursorManager()->SetDesignatedCursor(CCursorManager::CS_INVALID_CURSOR);
}

VOID CTargetHpGui::SetRaceIcon( RwUInt8 byRace )
{
	gui::CSurface surface;

	switch( byRace )
	{
	case RACE_HUMAN : 
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceHuman" ); 
		m_ppnlRaceIcon->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_HUMAN" ) ) );
		break;
	case RACE_NAMEK : 
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceNamek" ); 
		m_ppnlRaceIcon->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_NAMEK" ) ) );
		break;
	case RACE_MAJIN :
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceMajin" ); 
		m_ppnlRaceIcon->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_MAJIN" ) ) );
		break;		
	default : 
		NTL_ASSERTFAIL( "CTargetHpGui::SetRaceIcon : Invalid Race type" );
		return;
	}
	
	m_ppnlRaceIcon->GetSurface()->clear();
	m_ppnlRaceIcon->AddSurface( surface );
}

VOID CTargetHpGui::SetTargetRaceIcon(RwUInt8 byRace)
{
	gui::CSurface surface;

	switch (byRace)
	{
		case RACE_HUMAN:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceHuman");
		break;
		case RACE_NAMEK:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceNamek");
		break;
		case RACE_MAJIN:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceMajin");
		break;
		default:
		NTL_ASSERTFAIL("CTargetHpGui::SetTargetRaceIcon : Invalid Race type");
		return;
	}

	m_pbtTargetIcon->GetSurface()->clear();
	m_pbtTargetIcon->AddSurface(surface);
}

VOID CTargetHpGui::SetMobNpcTypeIcon( RwUInt8 byType )
{
	gui::CSurface surface;
	std::wstring wstrToolTip = Logic_GetMobRaceName(byType );

	switch( byType )
	{
		case MOB_TYPE_ANIMAL : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceAnimal" ); break;
		case MOB_TYPE_HUMAN_ANIMAL :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceHumanAnimal" ); break;
		case MOB_TYPE_DINOSAUR : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceDinosaur" ); break;
		case MOB_TYPE_ALIEN : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceAlien" ); break;
		case MOB_TYPE_ANDROID :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceAndroid" ); break;
		case MOB_TYPE_ROBOT : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceRobot" ); break;
		case MOB_TYPE_DRAGON : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceDragon" ); break;
		case MOB_TYPE_DEVIL :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceDevil" ); break;
		case MOB_TYPE_UNDEAD : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceUndead" ); break;
		case MOB_TYPE_PLANT : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRacePlant" ); break;
		case MOB_TYPE_INSECT:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceInsect" ); break;
		case MOB_TYPE_HUMAN :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceHuman" ); break;
		case MOB_TYPE_NAMEC :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceNamek" ); break;
		case MOB_TYPE_MAJIN :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceMajin" ); break;		
		case MOB_TYPE_BUILDING:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceRobot" ); break;
		case MOB_TYPE_ITEM_BOX:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceItemBox"); break;
	}

	m_ppnlRaceIcon->GetSurface()->clear();
	m_ppnlRaceIcon->AddSurface( surface );
	m_ppnlRaceIcon->SetToolTip( wstrToolTip );
}

VOID CTargetHpGui::SetTargetMobNpcTypeIcon(RwUInt8 byType)
{
	gui::CSurface surface;

	switch (byType)
	{
	case MOB_TYPE_ANIMAL:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceAnimal"); break;
	case MOB_TYPE_HUMAN_ANIMAL:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceHumanAnimal"); break;
	case MOB_TYPE_DINOSAUR:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceDinosaur"); break;
	case MOB_TYPE_ALIEN:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceAlien"); break;
	case MOB_TYPE_ANDROID:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceAndroid"); break;
	case MOB_TYPE_ROBOT:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceRobot"); break;
	case MOB_TYPE_DRAGON:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceDragon"); break;
	case MOB_TYPE_DEVIL:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceDevil"); break;
	case MOB_TYPE_UNDEAD:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceUndead"); break;
	case MOB_TYPE_PLANT:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRacePlant"); break;
	case MOB_TYPE_INSECT:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceInsect"); break;
	case MOB_TYPE_HUMAN:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceHuman"); break;
	case MOB_TYPE_NAMEC:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceNamek"); break;
	case MOB_TYPE_MAJIN:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceMajin"); break;
	case MOB_TYPE_BUILDING:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceRobot"); break;
	case MOB_TYPE_ITEM_BOX:
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfRaceItemBox"); break;
	}

	m_pbtTargetIcon->GetSurface()->clear();
	m_pbtTargetIcon->AddSurface(surface);
}

VOID CTargetHpGui::SetMobGrade( RwUInt8 byMobGrade )
{ // Show 여부도 판단
	gui::CSurface	surface;
	std::wstring	wstrToolTip;

	switch( byMobGrade )
	{
		case MOB_GRADE_NORMAL :	
			m_ppnlMobGrade->Show( false );
			return;
		case MOB_GRADE_SUPER : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGradeSuper" );
			wstrToolTip = GetDisplayStringManager()->GetString( "DST_MOB_GRADE_SUPER" );
			break;
		case MOB_GRADE_ULTRA : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGradeUltra" ); 
			wstrToolTip = GetDisplayStringManager()->GetString( "DST_MOB_GRADE_ULTRA" );
			break;
		case MOB_GRADE_BOSS :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGradeBoss" ); 
			wstrToolTip = GetDisplayStringManager()->GetString( "DST_MOB_GRADE_BOSS" );
			break;
		case MOB_GRADE_HERO : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGradeHero" ); 
			wstrToolTip = GetDisplayStringManager()->GetString( "DST_MOB_GRADE_HERO" );
			break;
	}

	m_ppnlMobGrade->Show( true );
	m_ppnlMobGrade->GetSurface()->clear();
	m_ppnlMobGrade->AddSurface( surface );
	m_ppnlMobGrade->SetToolTip( wstrToolTip );
}

VOID CTargetHpGui::SetNPCJobIcon( RwUInt8 byNPCJob )
{
	gui::CSurface surface;

	switch( byNPCJob )
	{
		case NPC_JOB_WEAPON_MERCHANT :				
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfWeaponMerchantIcon" ); break;
		case NPC_JOB_ARMOR_MERCHANT :				
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfArmorMerchantIcon" ); break;
		case NPC_JOB_GOODS_MERCHANT :				
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfMiscMerchantIcon" ); break;
		case NPC_JOB_SCOUTER_MERCHANT :			
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScouterMerchantIcon" ); break;
		case NPC_JOB_GUARD :						
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGuardIcon" ); break;
		case NPC_JOB_SKILL_TRAINER_HFI :						
		case NPC_JOB_SKILL_TRAINER_HMY :						
		case NPC_JOB_SKILL_TRAINER_HEN :						
		case NPC_JOB_SKILL_TRAINER_NFI :						
		case NPC_JOB_SKILL_TRAINER_NMY :						
		case NPC_JOB_SKILL_TRAINER_MMI :						
		case NPC_JOB_SKILL_TRAINER_MWO :			
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSkillMasterIcon" ); break;
		case NPC_JOB_BANKER :						
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfWareHouseIcon" ); break;
		case NPC_JOB_TALKER :						
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfTalkerIcon" ); break;
		case NPC_JOB_GUILD_MANAGER :				
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGuildIcon" ); break;
		case NPC_JOB_SUMMON_PET :					
			//surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "" ); 
			break;
		case NPC_JOB_DOGI_MERCHANT :				
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfDogiMerchantIcon" ); break;
		case NPC_JOB_SPECIAL_WEAPON_MERCHANT :	
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSPWeaponMerchantIcon" ); break;
		case NPC_JOB_SPECIAL_ARMOR_MERCHANT :		
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSPArmorMerchantIcon" ); break;
		case NPC_JOB_SPECIAL_GOODS_MERCHANT :		
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSPMiscMerchantIcon" ); break;
		case NPC_JOB_SPECIAL_FOODS_MERCHANT :		
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSPFoodMerchantIcon" ); break;
		case NPC_JOB_SPECIAL_SCOUTER_MERCHANT :	
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSPScouterMerchantIcon" ); break;
		case NPC_JOB_GRAND_SKILL_TRAINER_HFI :	
		case NPC_JOB_GRAND_SKILL_TRAINER_HMY :	
		case NPC_JOB_GRAND_SKILL_TRAINER_HEN :			
		case NPC_JOB_GRAND_SKILL_TRAINER_NFI :				
		case NPC_JOB_GRAND_SKILL_TRAINER_NMY :				
		case NPC_JOB_GRAND_SKILL_TRAINER_MMI :				
		case NPC_JOB_GRAND_SKILL_TRAINER_MWO :	
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGrandSkillMasterIcon" ); break;
		case NPC_JOB_SUB_WEAPON_MERCHANT :		
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSubWeaponMerchantIcon" ); break;
		case NPC_JOB_GATE_KEEPER :				
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfKeeperIcon" ); break;
		case NPC_JOB_VENDING_MACHINE :			
			//surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "" ); 
			break;
		case NPC_JOB_TIMEMACHINE_MERCHANT :		
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfTimeMachineIcon" ); break;
		case NPC_JOB_PORTAL_MAN :					
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfKeeperIcon" );
			break;
	}

	m_ppnlNpcJob->GetSurface()->clear();
	m_ppnlNpcJob->AddSurface( surface );
}

VOID CTargetHpGui::SetObjectIcon( RwUInt32 uiObjectFunction )
{
	gui::CSurface surface;

	if( uiObjectFunction & eDBO_TRIGGER_OBJECT_FUNC_DOJO_SEAL )
	{
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSealObject" );
	}

	m_ppnlNpcJob->GetSurface()->clear();
	m_ppnlNpcJob->AddSurface( surface );
}

//VOID CTargetHpGui::OnPowerPaint(VOID)
//{
//	m_numPower.Render();
//}
//
//VOID CTargetHpGui::OnPowerMove( RwInt32 nOldX, RwInt32 nOldY )
//{
//	CRectangle rtScreen = m_ppnlPowerText->GetScreenRect();
//	m_numPower.SetPosition( rtScreen.left, rtScreen.top );
//}

VOID CTargetHpGui::SetScrambleOwner(RwUInt8 bySealOwnerTeam)
{
	eDBO_TEAM eDBOTeam = Logic_GetScrambleTeam((eTOBJECT_STATE_TYPE_C)bySealOwnerTeam);

	switch( eDBOTeam )
	{
	case DBO_TEAM_MY_TEAM:
		{
			m_psttJobName->SetText( GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_OUR_GUILD_SEAL") );
			break;
		}
	case DBO_TEAM_ENEMY:
		{
			m_psttJobName->SetText( GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_OTHER_GUILD_SEAL") );
			break;
		}
	case DBO_TEAM_NEUTRAILITY:
		{
			m_psttJobName->SetText( GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_NO_OWNER_SEAL") );
			break;
		}
	default:
		{
			DBO_FAIL("Invalid DBO team : " << eDBOTeam);
			break;
		}
	}
}