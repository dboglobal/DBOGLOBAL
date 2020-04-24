#include "precomp_ntlsimulation.h"
#include "NtlSLDojo.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAvatar.h"
#include "NtlSLLogic.h"


CNtlDojo::CNtlDojo()
{
}

CNtlDojo::~CNtlDojo()
{
}

RwBool CNtlDojo::Create(void)
{
	NTL_FUNCTION("CNtlDojo::Create");

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo(); 
	if( !pAvatarInfo )
		NTL_RETURN(FALSE);

	for( RwUInt8 i = 0 ; i < pAvatarInfo->byDojoBriefCount ; ++i )
	{
		CreateDojo( &(pAvatarInfo->sDojoBrief[i]) );
	}

	for( RwUInt8 i = 0 ; i < NUM_SCRAMBLE_TEAM ; ++i )
		m_pScrambleTeam[i] = NULL;

	ClearScrambleInfo();

	NTL_RETURN(TRUE);
}

void CNtlDojo::Destroy()
{
	NTL_FUNCTION("CNtlDojo::Destroy");

	MAP_DOJO::iterator it = m_mapDojo.begin();
	for( ; it != m_mapDojo.end() ; ++it )
	{
		sDOJO_INFO* pDOJO_INFO = it->second;
		NTL_DELETE(pDOJO_INFO);
	}
	m_mapDojo.clear();

	ClearScrambleInfo();

	NTL_RETURNVOID();
}

void CNtlDojo::Update(RwReal fElapsed)
{
	m_ScrambleInfo.fLeftTime -= fElapsed;

	if( 0.f > m_ScrambleInfo.fLeftTime )
		m_ScrambleInfo.fLeftTime = 0.f;
}

sDOJO_INFO* CNtlDojo::CreateDojo(sDBO_DOJO_BRIEF* pDBO_DOJO_BRIEF)
{
	if( !pDBO_DOJO_BRIEF )
	{
		DBO_FAIL("Null sDBO_DOJO_BRIEF pointer");
		return NULL;
	}

	sDOJO_INFO* pDOJO_INFO = NTL_NEW sDOJO_INFO;

	pDOJO_INFO->uiDojoTableIndex		= pDBO_DOJO_BRIEF->dojoTblidx;
	pDOJO_INFO->guildID					= pDBO_DOJO_BRIEF->guildId;
	pDOJO_INFO->byLevel					= pDBO_DOJO_BRIEF->byLevel;
	pDOJO_INFO->tGuildMark				= pDBO_DOJO_BRIEF->sMark;

	m_mapDojo[pDOJO_INFO->uiDojoTableIndex] = pDOJO_INFO;

	return pDOJO_INFO;
}

void CNtlDojo::ClearDojo(RwUInt32 uiDojoTableIndex)
{
	MAP_DOJO::iterator it = m_mapDojo.find(uiDojoTableIndex);
	if( it == m_mapDojo.end() )
		return;
	
	sDOJO_INFO* pDOJO_INFO = it->second;
	NTL_DELETE(pDOJO_INFO);
	m_mapDojo.erase(it);
}

void CNtlDojo::ClearScrambleInfo()
{
	for( RwUInt8 i = 0 ; i < NUM_SCRAMBLE_TEAM ; ++i )
		NTL_DELETE(m_pScrambleTeam[i]);
	
	m_ScrambleInfo.eDojoState					= eDBO_DOJO_STATUS_NORMAL;
	m_ScrambleInfo.eAvatarTeam					= INVALID_SCRAMBLE_TEAM;
	m_ScrambleInfo.uiScrambleDojoTableIndex		= INVALID_TBLIDX;
	m_ScrambleInfo.byTeleportProposalIndex		= TELEPORT_TYPE_INVALID;
	m_ScrambleInfo.fLeftTime					= 0.f;

	MAP_SCRAMBLE_SEAL::iterator it_ScrambleSeal	= m_ScrambleInfo.mapScrambleSeal.begin();
	for( ; it_ScrambleSeal != m_ScrambleInfo.mapScrambleSeal.end() ; ++it_ScrambleSeal )
	{
		sSCRAMBLE_SEAL_INFO* pSCRAMBLE_SEAL_INFO = it_ScrambleSeal->second;
		NTL_DELETE(pSCRAMBLE_SEAL_INFO);
	}
	m_ScrambleInfo.mapScrambleSeal.clear();
}

sDOJO_INFO* CNtlDojo::GetDojoInfo(RwUInt32 uiDojoTableIndex)
{
	MAP_DOJO::iterator it = m_mapDojo.find(uiDojoTableIndex);
	if( it == m_mapDojo.end() )
		return NULL;

	return it->second;
}

sDOJO_INFO* CNtlDojo::GetDojoInfo_fromGuildID(GUILDID guildID)
{
	MAP_DOJO::iterator it = m_mapDojo.begin();
	for( ; it != m_mapDojo.end() ; ++it )
	{
		sDOJO_INFO* pDOJO_INFO = it->second;
		if( pDOJO_INFO->guildID == guildID )
			return pDOJO_INFO;
	}

	return NULL;
}

sSCRAMBLE_TEAM* CNtlDojo::GetTeam(eSCRAMBLE_TEAM eTeam)
{
	if( FALSE == Logic_IsUsableIndex((RwUInt32)eTeam, NUM_SCRAMBLE_TEAM) )
	{
		DBO_FAIL("Invalid index");
		return NULL;
	}

	return m_pScrambleTeam[eTeam];
}

sSCRAMBLE_INFO* CNtlDojo::GetScramble()
{
	return &m_ScrambleInfo;
}

void CNtlDojo::HandleEvents( RWS::CMsg &msg)
{
	NTL_FUNCTION("CNtlDojo::HandleEvents");

	if( msg.Id == g_EventDojo )
	{
		SNtlEventDojo* pEvent = reinterpret_cast<SNtlEventDojo*>( msg.pData );

		switch(pEvent->byDojoEvent)
		{
		case DOJO_EVENT_DOJO_INFO:
			{
				sDBO_DOJO_BRIEF* pDBO_DOJO_BRIEF = reinterpret_cast<sDBO_DOJO_BRIEF*>( pEvent->pExData );

				sDOJO_INFO* pDOJO_INFO = GetDojoInfo(pDBO_DOJO_BRIEF->dojoTblidx);
				if( !pDOJO_INFO )
				{
					pDOJO_INFO = CreateDojo(pDBO_DOJO_BRIEF);

					if( !pDOJO_INFO )
					{
						DBO_FAIL("Fail create dojo info");
						NTL_RETURNVOID();
					}
				}
				else
				{
					pDOJO_INFO->guildID				= pDBO_DOJO_BRIEF->guildId;
					pDOJO_INFO->byLevel				= pDBO_DOJO_BRIEF->byLevel;
					pDOJO_INFO->tGuildMark			= pDBO_DOJO_BRIEF->sMark;
				}

				CNtlSLEventGenerator::DojoEventNotify(DOJO_EVENT_DOJO_INFO, pDBO_DOJO_BRIEF->dojoTblidx);
				break;
			}
		case DOJO_EVENT_LOST_DOJO:
			{
				sDOJO_INFO* pDOJO_INFO = GetDojoInfo(pEvent->uiParam);
				if( !pDOJO_INFO )
				{
					DBO_FAIL("Not exist dojo info of guild ID : " << pEvent->uiParam);
					NTL_RETURNVOID();
				}

				CNtlSLEventGenerator::DojoEventNotify(DOJO_EVENT_LOST_DOJO, pEvent->uiParam);
				ClearDojo(pEvent->uiParam);
				break;
			}
		case DOJO_EVENT_CHANGE_LEVEL:
			{
				sDOJO_INFO* pDOJO_INFO = GetDojoInfo(pEvent->uiParam);
				if( !pDOJO_INFO )
				{
					DBO_FAIL("Not exist dojo info of dojo index : " << pEvent->uiParam);
					NTL_RETURNVOID();
				}

				pDOJO_INFO->byLevel				= *(RwUInt8*)pEvent->pExData;

				CNtlSLEventGenerator::DojoEventNotify(DOJO_EVENT_CHANGE_LEVEL, pEvent->uiParam, pEvent->pExData);
				break;
			}
		case DOJO_EVENT_SCRAMBLE_SCORE:
			{
				if( pEvent->uiParam != m_ScrambleInfo.uiScrambleDojoTableIndex )
					break;

				sDOJO_SCRAMBLE_POINT* pDOJO_SCRAMBLE_POINT = (sDOJO_SCRAMBLE_POINT*)pEvent->pExData;

				// 공격팀 정보
				m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->byPlayerCount	= pDOJO_SCRAMBLE_POINT->byAttCount;
				m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->bySealCount		= pDOJO_SCRAMBLE_POINT->byAttSealCount;
				m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->uiScore			= pDOJO_SCRAMBLE_POINT->dwAttCharge;
				m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->uiTotalPoint	= pDOJO_SCRAMBLE_POINT->dwAttPoint;

				// 방어팀 정보
				m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->byPlayerCount	= pDOJO_SCRAMBLE_POINT->byDefCount;
				m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->bySealCount		= pDOJO_SCRAMBLE_POINT->byDefSealCount;
				m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->uiScore			= pDOJO_SCRAMBLE_POINT->dwDefCharge;
				m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->uiTotalPoint	= pDOJO_SCRAMBLE_POINT->dwDefPoint;

				CNtlSLEventGenerator::DojoEventNotify(DOJO_EVENT_SCRAMBLE_SCORE, pEvent->uiParam, pEvent->pExData, pEvent->pExData2);
				break;
			}
		case DOJO_EVENT_SCRAMBLE_TELEPORT_PROPOSAL:
			{
				m_ScrambleInfo.byTeleportProposalIndex = (RwUInt8)pEvent->uiParam;
				CNtlSLEventGenerator::DojoEventNotify(DOJO_EVENT_SCRAMBLE_TELEPORT_PROPOSAL, pEvent->uiParam, pEvent->pExData, pEvent->pExData2, pEvent->pExData3);
				break;
			}
		case DOJO_EVENT_SEAL_ATTACK_STATE:
			{
				TBLIDX			idxObject	= pEvent->uiParam;
				RwUInt8			byState		= *(RwUInt8*)pEvent->pExData;
				sVECTOR2		v2Loc		= *(sVECTOR2*)pEvent->pExData2;

				MAP_SCRAMBLE_SEAL::iterator it_ScrambleSeal	= m_ScrambleInfo.mapScrambleSeal.find(idxObject);
				if( it_ScrambleSeal	== m_ScrambleInfo.mapScrambleSeal.end() )
				{
					sSCRAMBLE_SEAL_INFO* pSCRAMBLE_SEAL_INFO = NTL_NEW sSCRAMBLE_SEAL_INFO;

					pSCRAMBLE_SEAL_INFO->eState			= (eTOBJECT_STATE_TYPE_C)byState;
					pSCRAMBLE_SEAL_INFO->v2Pos.x		= v2Loc.x;
					pSCRAMBLE_SEAL_INFO->v2Pos.y		= v2Loc.z;

					m_ScrambleInfo.mapScrambleSeal[idxObject] = pSCRAMBLE_SEAL_INFO;
				}
				else
				{
					sSCRAMBLE_SEAL_INFO* pSCRAMBLE_SEAL_INFO = it_ScrambleSeal->second;
					pSCRAMBLE_SEAL_INFO->eState			= (eTOBJECT_STATE_TYPE_C)byState;
				}

				CNtlSLEventGenerator::DojoEventNotify(DOJO_EVENT_SEAL_ATTACK_STATE, pEvent->uiParam, pEvent->pExData, pEvent->pExData2, pEvent->pExData3);
				break;
			}
		case DOJO_EVENT_SCRAMBLE_CHANGE_SEAL_OWNER:
			{
				RwUInt32		uiDojoTblidx	= pEvent->uiParam;
				RwUInt32		uiObjectTable	= *(RwUInt32*)pEvent->pExData;
				GUILDID			guildID			= *(GUILDID*)pEvent->pExData2;
				sDOJO_INFO*		pDOJO_INFO		= GetDojoInfo( uiDojoTblidx );

				if( !pDOJO_INFO )
				{
					DBO_FAIL("Not exist dojo info");
					break;
				}

				if( m_ScrambleInfo.uiScrambleDojoTableIndex != pDOJO_INFO->uiDojoTableIndex )
				{
					DBO_FAIL("It's not scramble dojo info : " << pDOJO_INFO->uiDojoTableIndex);
					break;
				}

				MAP_SCRAMBLE_SEAL::iterator it_ScrambleSeal	= m_ScrambleInfo.mapScrambleSeal.find(uiObjectTable);
				if( it_ScrambleSeal	== m_ScrambleInfo.mapScrambleSeal.end() )
				{
					break;
				}

				sSCRAMBLE_SEAL_INFO* pSCRAMBLE_SEAL_INFO = it_ScrambleSeal->second;
				pSCRAMBLE_SEAL_INFO->eState = TOBJECT_STATE_TYPE_C_NO_TEAM;

				sSCRAMBLE_TEAM* pSCRAMBLE_TEAM = GetTeam(SCRAMBLE_TEAM_OFFENCE);
				if( pSCRAMBLE_TEAM && guildID == pSCRAMBLE_TEAM->guildID )
				{
					pSCRAMBLE_SEAL_INFO->eState = TOBJECT_STATE_TYPE_C_ATTACK_TEAM;
				}
				else
				{
					pSCRAMBLE_TEAM = GetTeam(SCRAMBLE_TEAM_DEFENCE);
					if( pSCRAMBLE_TEAM && guildID == pSCRAMBLE_TEAM->guildID )
					{
						pSCRAMBLE_SEAL_INFO->eState = TOBJECT_STATE_TYPE_C_DEFENCE_TEAM;
					}
				}

				CNtlSLEventGenerator::DojoEventNotify(DOJO_EVENT_SCRAMBLE_CHANGE_SEAL_OWNER, pEvent->uiParam, pEvent->pExData, pEvent->pExData2, pEvent->pExData3);
				break;
			}
		default:
			{
				CNtlSLEventGenerator::DojoEventNotify(pEvent->byDojoEvent, pEvent->uiParam, pEvent->pExData, pEvent->pExData2, pEvent->pExData3);
				break;
			}
		}
	}	
	else if( msg.Id == g_EventDojoState )
	{
		SNtlEventDojoMessage* pEvent = reinterpret_cast<SNtlEventDojoMessage*>( msg.pData );

		RwBool bClearScrambleInfo = FALSE;

		if( m_ScrambleInfo.uiScrambleDojoTableIndex == pEvent->uiDojoTableIndex )
		{
			if( eDBO_DOJO_STATUS_NORMAL != m_ScrambleInfo.eDojoState )
			{
				if( eDBO_DOJO_STATUS_NORMAL == (eDBO_DOJO_STATUS)pEvent->byDojoState )
					bClearScrambleInfo = TRUE;
			}

			m_ScrambleInfo.eDojoState	= (eDBO_DOJO_STATUS)pEvent->byDojoState;
			m_ScrambleInfo.fLeftTime	= (RwReal)(pEvent->ui64time - GetNtlSLGlobal()->GetServerTime());
		}
	

		CNtlSLEventGenerator::DojoStateNotify(pEvent->uiDojoTableIndex, pEvent->byDojoState, pEvent->ui64time);


		if( bClearScrambleInfo )
		{
			ClearScrambleInfo();

			// 쟁탈전에 이겨서 쟁탈전이 끝나도 도장에 남아있는 유파원들을 위해서
			if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
				GetNtlWorldConcept()->DeleteGradeWorldPlayConcept(WORLD_CONCEPT_FIRST_GRADE);
		}
	}	
	else if( msg.Id == g_EventScrambleDefaultInfo )
	{
		SNtlEventScrambleDefaultInfo* pEvent = reinterpret_cast<SNtlEventScrambleDefaultInfo*>( msg.pData );

		if( m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE] || m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE] )
		{
			DBO_FAIL("Already exist scramble default team infomation");
			NTL_RETURNVOID();
		}

		sDOJO_INFO* pDOJO_INFO = GetDojoInfo(pEvent->uiDojoTableIndex);
		if( !pDOJO_INFO )
		{
			DBO_FAIL("Not exist dojo info of guild ID : " << pEvent->uiDojoTableIndex);
			NTL_RETURNVOID();
		}

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
		{
			DBO_FAIL("Not exist sob avatar instance");
			NTL_RETURNVOID();
		}

		CNtlGuild* pGuild = pAvatar->GetGuild();		


		// 도장 쟁탈전을 진행하는 해당 도장의 테이블 인덱스
		m_ScrambleInfo.uiScrambleDojoTableIndex = pEvent->uiDojoTableIndex;

		if( pGuild->GetGuildID() == pEvent->uiOffenceGuildID )
			m_ScrambleInfo.eAvatarTeam = SCRAMBLE_TEAM_OFFENCE;
		else
			m_ScrambleInfo.eAvatarTeam = SCRAMBLE_TEAM_DEFENCE;


		// 공격팀 정보
		m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE] = NTL_NEW sSCRAMBLE_TEAM;

		m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->guildID			= pEvent->uiOffenceGuildID;
		m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->byPlayerCount	= 0;
		m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->bySealCount		= 0;
		m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->uiScore			= 0;
		m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->uiTotalPoint	= 0;		
		
		swprintf_s(m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->awcGuildName, NTL_MAX_SIZE_GUILD_NAME + 1, L"%s", pEvent->pwcOffenceGuildName);


		sDBO_GUILD_MARK* pDBO_GUILD_MARK = (sDBO_GUILD_MARK*)pEvent->pOffenceGuildEmblem;

		m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->tEmblemFactor.byTypeA		= pDBO_GUILD_MARK->byMarkMain;
		m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->tEmblemFactor.byTypeB		= pDBO_GUILD_MARK->byMarkInLine;
		m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->tEmblemFactor.byTypeC		= pDBO_GUILD_MARK->byMarkOutLine;
		m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->tEmblemFactor.byTypeAColor	= pDBO_GUILD_MARK->byMarkMainColor;
		m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->tEmblemFactor.byTypeBColor	= pDBO_GUILD_MARK->byMarkInColor;
		m_pScrambleTeam[SCRAMBLE_TEAM_OFFENCE]->tEmblemFactor.byTypeCColor	= pDBO_GUILD_MARK->byMarkOutColor;
		


		// 방어팀 정보
		m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE] = NTL_NEW sSCRAMBLE_TEAM;

		m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->guildID			= pEvent->uiDefenceGuildID;
		m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->byPlayerCount	= 0;
		m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->bySealCount		= 0;
		m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->uiScore			= 0;
		m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->uiTotalPoint	= 0;

		swprintf_s(m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->awcGuildName, NTL_MAX_SIZE_GUILD_NAME + 1, L"%s", pEvent->pwcDefenceGuildName);


		pDBO_GUILD_MARK = (sDBO_GUILD_MARK*)pEvent->pDefenceGuildEmblem;

		m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->tEmblemFactor.byTypeA		= pDBO_GUILD_MARK->byMarkMain;
		m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->tEmblemFactor.byTypeB		= pDBO_GUILD_MARK->byMarkInLine;
		m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->tEmblemFactor.byTypeC		= pDBO_GUILD_MARK->byMarkOutLine;
		m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->tEmblemFactor.byTypeAColor	= pDBO_GUILD_MARK->byMarkMainColor;
		m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->tEmblemFactor.byTypeBColor	= pDBO_GUILD_MARK->byMarkInColor;
		m_pScrambleTeam[SCRAMBLE_TEAM_DEFENCE]->tEmblemFactor.byTypeCColor	= pDBO_GUILD_MARK->byMarkOutColor;


		CNtlSLEventGenerator::ScrambleDefaultInfoNotify(pEvent->uiDojoTableIndex,
														pEvent->uiDefenceGuildID,	pEvent->uiOffenceGuildID,
														pEvent->pwcDefenceGuildName,pEvent->pwcOffenceGuildName,
														pEvent->pDefenceGuildEmblem,pEvent->pOffenceGuildEmblem);
	}

	NTL_RETURNVOID();
}