#include "precomp_ntlsimulation.h"
#include "NtlSLGuild.h"

// core
#include "NtlDebug.h"

// shared
#include "TextAllTable.h"
#include "TableContainer.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLEmblemMaker.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlSobCharProxy.h"
#include "NtlSobCharProxyDecoration.h"
#include "NtlSobManager.h"
#include "NtlSobGroup.h"

#define dBIT_PER_BYTE		8

CNtlGuild::CNtlGuild()
:m_guildID(INVALID_GUILDID)
,m_masterID(INVALID_SERIAL_ID)
,m_uiReputation(0)
,m_uiMaxReputationEver(0)
,m_timeToDisband(0)
,m_iOnlineMember(0)
,m_ui64Function(DBO_GUILD_FUNCTION_INVALID)
,m_byGuildDogiColorIndex(INVALID_BYTE)
,m_byDojoDogiColorIndex(INVALID_BYTE)
{
	ZeroMemory(m_wszGuildName, NTL_MAX_SIZE_GUILD_NAME+1 );
	ZeroMemory(&m_GuildNotice, sizeof(m_GuildNotice));

	for( RwUInt8 i = 0 ; i < DBO_MAX_SECOND_MASTER_IN_GUILD ; ++i )
		m_secondMasterID[i] = INVALID_SERIAL_ID;
}

CNtlGuild::~CNtlGuild()
{

}

RwBool CNtlGuild::Create(void)
{
	NTL_FUNCTION("CNtlGuild::Create");

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo(); 
	if( !pAvatarInfo )
		NTL_RETURN(FALSE);

	m_guildID = pAvatarInfo->sCharPf.guildId;

	NTL_RETURN(TRUE);
}

void CNtlGuild::Destroy(void)
{
	NTL_FUNCTION("CNtlGuild::Destroy");

	Leave();

	NTL_RETURNVOID();
}

VOID CNtlGuild::Leave()
{
	ZeroMemory(m_wszGuildName, NTL_MAX_SIZE_GUILD_NAME+1);
	ZeroMemory(&m_GuildNotice, sizeof(m_GuildNotice));

	m_masterID = INVALID_SERIAL_ID;

	for(RwUInt8 i = 0 ; i < DBO_MAX_SECOND_MASTER_IN_GUILD ; ++i )
		m_secondMasterID[i] = INVALID_SERIAL_ID;

	m_guildID				= INVALID_GUILDID;
	m_uiReputation			= 0;
	m_uiMaxReputationEver	= 0;
	m_timeToDisband			= 0;
	m_iOnlineMember			= 0;
	m_ui64Function			= DBO_GUILD_FUNCTION_INVALID;
	m_byGuildDogiColorIndex	= INVALID_BYTE;
	m_byDojoDogiColorIndex	= INVALID_BYTE;

	ClearMember();
}

const WCHAR* CNtlGuild::GetGuildName()
{
	return m_wszGuildName;
}

GUILDID CNtlGuild::GetGuildID()
{
	return m_guildID;
}

CHARACTERID CNtlGuild::GetMasterID()
{
	return m_masterID;
}

CHARACTERID CNtlGuild::GetSecondMasterID(RwInt32 iIndex)
{
	if( FALSE == Logic_IsUsableIndex(iIndex, DBO_MAX_SECOND_MASTER_IN_GUILD) )
	{
		DBO_FAIL("Invalid index : " << iIndex);
		return INVALID_CHARACTERID;
	}

	return m_secondMasterID[iIndex];
}

RwInt32 CNtlGuild::GetCountSecondMaster()
{
	RwInt32 iCount = 0;

	for( RwUInt8 i = 0 ; i < DBO_MAX_SECOND_MASTER_IN_GUILD ; ++i )
	{
		if( m_secondMasterID[i] != INVALID_SERIAL_ID )
			++iCount;
	}

	return iCount;
}

RwUInt32 CNtlGuild::GetReputation()
{
	return m_uiReputation;
}

RwUInt32 CNtlGuild::GetReputationEver()
{
	return m_uiMaxReputationEver;
}

time_t CNtlGuild::GetTimeToDisband()
{
	return m_timeToDisband;
}

sGuildNotice* CNtlGuild::GetNotice()
{
	return &m_GuildNotice;
}

RwInt32 CNtlGuild::GetOnlineMemberCount()
{
	return m_iOnlineMember;
}

RwUInt8 CNtlGuild::GetGuildDogiColorIndex()
{
	return m_byGuildDogiColorIndex;
}

RwUInt8 CNtlGuild::GetDojoDogiColorIndex()
{
	return m_byDojoDogiColorIndex;
}

VOID CNtlGuild::AppointSecondMaster(CHARACTERID charID)
{
	for(RwUInt8 i = 0 ; i < DBO_MAX_SECOND_MASTER_IN_GUILD ; ++i )
	{
		if( m_secondMasterID[i] == INVALID_SERIAL_ID )
		{
			m_secondMasterID[i] = charID;
			return;
		}
	}

	DBO_FAIL("Because too many second master exist, so can not appint second master");
}

VOID CNtlGuild::DismissSecondMaster(CHARACTERID charID)
{
	for(RwUInt8 i = 0 ; i < DBO_MAX_SECOND_MASTER_IN_GUILD ; ++i )
	{
		if( m_secondMasterID[i] == charID )
		{
			m_secondMasterID[i] = INVALID_SERIAL_ID;
			return;
		}
	}
}

VOID CNtlGuild::ChangeAllMemberDogiColor(RwUInt8 byGuildDogiColor, RwUInt8 byDojoDogiColor)
{
	m_byGuildDogiColorIndex	= byGuildDogiColor;
	m_byDojoDogiColorIndex	= byDojoDogiColor;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;


	// 아바타 도복 색상
	pAvatar->SetGuildDogiColorIndex(m_byGuildDogiColorIndex);
	pAvatar->SetDojoDogiColorIndex(m_byDojoDogiColorIndex);
	Logic_SetDogiItemColor(pAvatar, pAvatar->GetRealDogiColorIndex());
	Logic_SetDogiItemColor(pAvatar, pAvatar->GetRealDogiColorIndex(), TRUE);


	// 유파원 도복 색상
	COMMUNITY_ITER it_list = m_listMember.begin();
	for( ; it_list != m_listMember.end() ; ++it_list )
	{
		sGuildMember* pMember = reinterpret_cast<sGuildMember*>( *it_list );
		ChangeMemberDogiColor(pMember->charID, m_byGuildDogiColorIndex, m_byDojoDogiColorIndex);
	}
}

VOID CNtlGuild::ChangeMemberDogiColor(CHARACTERID charID, RwUInt8 byGuildDogiColor, RwUInt8 byDojoDogiColor)
{
	CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_PLAYER );
	if( !pSobGroup )
		return;

	CNtlSobGroup::MapObject::iterator it;
	for( it = pSobGroup->GetObjects().begin() ; it != pSobGroup->GetObjects().end() ; ++it )
	{
		CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( it->second );

		if( pSobPlayer->GetCharID() == charID )
		{
			// 도복 색상
			pSobPlayer->SetGuildDogiColorIndex(byGuildDogiColor);
			pSobPlayer->SetDojoDogiColorIndex(byDojoDogiColor);
			Logic_SetDogiItemColor(pSobPlayer, pSobPlayer->GetRealDogiColorIndex());
			break;
		}
	}
}

VOID CNtlGuild::Sort(RwUInt8 byDataType, RwBool bAscent)
{
	switch(byDataType)
	{
	case COMMUNITY_GUILD_LEVEL:	dCOMMUNITY_GROUP_SORT(bAscent, CompareAscentLevel, CompareDecentLevel);
	case COMMUNITY_GUILD_AREA:	dCOMMUNITY_GROUP_SORT(bAscent, CompareAscentArea, CompareDecentArea);
	}

	CNtlSLCommunityGroup::Sort(byDataType, bAscent);
}

RwBool CNtlGuild::IsBeingDisband()
{
	return m_timeToDisband > 0;
}

RwBool CNtlGuild::IsGuildMaster(CHARACTERID charID)
{
	return (charID == m_masterID);
}

RwBool CNtlGuild::IsSecondGuildMaster(CHARACTERID charID)
{
	for( RwUInt8 i = 0 ; i < DBO_MAX_SECOND_MASTER_IN_GUILD ; ++i )
	{
		if( charID == m_secondMasterID[i] )
			return TRUE;
	}

	return FALSE;
}

RwBool CNtlGuild::IsHaveEmblem()
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return FALSE;
	}

	CNtlSobCharProxy* pSobCharProxy = reinterpret_cast<CNtlSobCharProxy*>( pAvatar->GetSobProxy() );
	if( !pSobCharProxy )
	{
		DBO_FAIL("Not exist avatar proxy");
		return FALSE;
	}

	CNtlSobCharDecorationProxy* pSobCharDecorationProxy = pSobCharProxy->GetDecorationProxy();
	if( !pSobCharDecorationProxy )
	{
		DBO_FAIL("Not exist avatar decoration proxy");
		return FALSE;
	}

	sEmblemFactor emblemFactor = *pSobCharDecorationProxy->GetEmblemFactor();

	if( emblemFactor.byTypeA == dPLEMBLEM_INVALID_FACTOR &&
		emblemFactor.byTypeB == dPLEMBLEM_INVALID_FACTOR &&
		emblemFactor.byTypeC == dPLEMBLEM_INVALID_FACTOR )
		return FALSE;

	return TRUE;
}

RwBool CNtlGuild::IsChangedGuildDogiColor()
{
	if( INVALID_BYTE == m_byGuildDogiColorIndex )
		return FALSE;

	return TRUE;
}

RwBool CNtlGuild::IsChangedDojoDogiColor()
{
	if( INVALID_BYTE == m_byDojoDogiColorIndex )
		return FALSE;

	return TRUE;
}

RwBool CNtlGuild::IsExistFunction(eDBO_GUILD_FUNCTION eFunctionIndex)
{
	return Dbo_HasGuildFunction(m_ui64Function, eFunctionIndex);
}

RwBool CNtlGuild::CanGetGuildFunction(eDBO_GUILD_FUNCTION eFunctionIndex)
{
	RwUInt16 ui16Result;
	return Dbo_CanGetGuildFunction(m_ui64Function, m_uiMaxReputationEver, Logic_GetZenny(), eFunctionIndex, &ui16Result);
}

RwBool CNtlGuild::CanGetDojoFunction(eDBO_GUILD_FUNCTION eFunctionIndex)
{
	RwUInt16 ui16Result;
	return Dbo_CanGetDojoFunction(m_ui64Function, m_uiMaxReputationEver, Logic_GetZenny(), eFunctionIndex, &ui16Result);
}

RwBool CNtlGuild::EnableUIGuildFunction(eDBO_GUILD_FUNCTION eFunctionIndex)
{
	return Dbo_GetUIGuildFunction(m_ui64Function, eFunctionIndex);
}

RwBool CNtlGuild::EnableUIDojoFunction(eDBO_GUILD_FUNCTION eFunctionIndex)
{
	return Dbo_GetUIDojoFunction(m_ui64Function, eFunctionIndex);
}

RwBool CNtlGuild::IsHaveGroup()
{
	// GuildInfo 패킷이 오기 전에 길드 ID만 가지고 있다.
	// GuildInfo 패킷에 포함된 길드 이름 여부로 길드 가입 여부를 판단하자
	if( wcslen(m_wszGuildName) > 0 )
		return TRUE;

	return FALSE;
}

VOID CNtlGuild::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlGuild::HandleEvents");

	if( pMsg.Id == g_EventSLGuild )
	{
		SNtlEventGuild* pEvent = reinterpret_cast<SNtlEventGuild*>( pMsg.pData );
		switch(pEvent->iMessage)
		{
			case SLGE_GUILD_INFO:
			{
				// 이 데이터는 월드에 완전히 진입하고 나서 늦게 올 수도 있다
				sDBO_GUILD_INFO* pDBO_GUILD_INFO = reinterpret_cast<sDBO_GUILD_INFO*>( pEvent->pData );

				m_guildID = pDBO_GUILD_INFO->guildId;

				memset(m_wszGuildName, 0, sizeof(m_wszGuildName));
				memcpy(m_wszGuildName, pDBO_GUILD_INFO->wszName, sizeof(WCHAR)*NTL_MAX_SIZE_GUILD_NAME);
	
				m_masterID = pDBO_GUILD_INFO->guildMaster;

				for(RwUInt8 i = 0 ; i < DBO_MAX_SECOND_MASTER_IN_GUILD ; ++i )
					m_secondMasterID[i] = pDBO_GUILD_INFO->guildSecondMaster[i];

				m_uiReputation			= pDBO_GUILD_INFO->dwGuildReputation;
				m_uiMaxReputationEver	= pDBO_GUILD_INFO->dwMaxGuildPointEver;

				m_ui64Function			= pDBO_GUILD_INFO->qwGuildFunctionFlag;
				m_timeToDisband			= pDBO_GUILD_INFO->timeToDisband;


				// 아바타는 따로 패킷을 받지 않는다
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				CNtlSobCharProxy* pSobCharProxy = reinterpret_cast<CNtlSobCharProxy*>( pAvatar->GetSobProxy() );
				CNtlSobCharDecorationProxy* pSobCharDecorationProxy = pSobCharProxy->GetDecorationProxy();
				pSobCharDecorationProxy->SetEmblemFactor(pDBO_GUILD_INFO->sMark.byMarkMain,		pDBO_GUILD_INFO->sMark.byMarkMainColor,
														 pDBO_GUILD_INFO->sMark.byMarkInLine,	pDBO_GUILD_INFO->sMark.byMarkInColor,
														 pDBO_GUILD_INFO->sMark.byMarkOutLine,	pDBO_GUILD_INFO->sMark.byMarkOutColor);				

				// Avatar Overhead Name
				Logic_SetHeadNameColor(pAvatar);

				ChangeAllMemberDogiColor(pDBO_GUILD_INFO->sDogi.byGuildColor,
										 pDBO_GUILD_INFO->sDogi.byDojoColor);


				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_GUILD_INFO);
				
				break;
			}
		case SLGE_ADD_MEMBER:
			{
				sDBO_GUILD_MEMBER_INFO* pData = reinterpret_cast<sDBO_GUILD_MEMBER_INFO*>( pEvent->pData );
				sGuildMember* pMember = NTL_NEW sGuildMember;

				pMember->charID			= pData->charId;				
				pMember->byRace			= pData->byRace;
				pMember->byLevel		= pData->byLevel;
				pMember->byClass		= pData->byClass;
				pMember->uiReputation	= pData->dwReputation;
				pMember->bOnline		= pData->bIsOnline;
				memcpy(pMember->wszMemberName, pData->wszMemberName, sizeof(WCHAR)*NTL_MAX_SIZE_CHAR_NAME);

				if( AddMember(pMember->charID, pMember) )
				{
					if( pMember->bOnline )
						++m_iOnlineMember;


					// 처음 길드에 들어가면 맴버 정보를 먼저 받고 이후 길드 조직에 관한 정보를 받는다
					if( IsHaveGroup() )
					{
						ChangeMemberDogiColor(pMember->charID, m_byGuildDogiColorIndex, m_byDojoDogiColorIndex);

						CNtlSLEventGenerator::NotifyGuildEvent(SLGE_ADD_MEMBER, pData->charId);
					}
				}
				else
				{
					NTL_DELETE(pMember);
				}

				break;
			}
		case SLGE_DEL_MEMBER:
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
					break;

				CHARACTERID avatarID = pAvatar->GetCharID();
				if( avatarID == pEvent->uiParam )
				{
					// 내가 강퇴 당했다					
					CNtlSLEventGenerator::NotifyGuildEvent(SLGE_DISBAND);

					// 아바타 도복 색상
					pAvatar->SetGuildDogiColorIndex(INVALID_BYTE);
					pAvatar->SetDojoDogiColorIndex(INVALID_BYTE);
					Logic_SetDogiItemColor(pAvatar, INVALID_BYTE);
					Logic_SetDogiItemColor(pAvatar, INVALID_BYTE, TRUE);
					
					Leave();

					// 머리 위 이름
					CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
					Logic_SetHeadNameColor(pAvatar);
				}
				else
				{
					sGuildMember* pMember = reinterpret_cast<sGuildMember*>( GetMemberbyKey(pEvent->uiParam) );
					if( !pMember )
					{
						DBO_FAIL("SLGE_DEL_MEMBER, Not exist guild member character ID " << pEvent->uiParam);
						break;
					}

					if( pMember->bOnline )
						--m_iOnlineMember;


					ChangeMemberDogiColor(pMember->charID, INVALID_BYTE, INVALID_BYTE);


					// 부길드장이라면 부길드장 정보 초기화
					DismissSecondMaster(pEvent->uiParam);
					DelMember(pEvent->uiParam);					

					CNtlSLEventGenerator::NotifyGuildEvent(SLGE_DEL_MEMBER, pEvent->uiParam);				
				}				

				break;
			}
		case SLGE_APPOINT_MASTER:
			{
				// 부길드장이라면 부길드장 정보 초기화
				DismissSecondMaster(pEvent->uiParam);
				m_masterID = pEvent->uiParam;
				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_APPOINT_MASTER, pEvent->uiParam);
				break;
			}
		case SLGE_APPOINT_SECOND_MASTER:
			{
				AppointSecondMaster(pEvent->uiParam);
				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_APPOINT_SECOND_MASTER, pEvent->uiParam);
				break;
			}
		case SLGE_DISMISS_SECOND_MASTER:
			{
				DismissSecondMaster(pEvent->uiParam);
				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_DISMISS_SECOND_MASTER, pEvent->uiParam);
				break;
			}		
		case SLGE_BEING_DISBAND:
			{
				m_timeToDisband = *reinterpret_cast<RwUInt64*>( pEvent->pData ) ;
				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_BEING_DISBAND);
				break;
			}
		case SLGE_CANCLE_DISBAND:
			{
				m_timeToDisband = 0;
				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_CANCLE_DISBAND);
				break;
			}
		case SLGE_DISBAND:
			{				
				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_DISBAND);

				ChangeAllMemberDogiColor(INVALID_BYTE, INVALID_BYTE);

				Leave();

				// 머리 위 이름
				// 다른 길드원의 이름은 서버로부터 별도의 패킷으로 업데이트 된다
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				Logic_SetHeadNameColor(pAvatar);

				break;
			}
		case SLGE_MEM_POS:
			{				
				sGuildMember* pMember = reinterpret_cast<sGuildMember*>( GetMemberbyKey(pEvent->uiParam) );
				if( !pMember )
				{
					DBO_FAIL("Not exist guild member character ID " << pEvent->uiParam << " when member position change");
					break;
				}
				
				pMember->uiWorldNameTable = *reinterpret_cast<RwUInt32*>(pEvent->pData);
				CMapNameTextTable* pMapNameTextTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();

				if( pMapNameTextTable->GetAreaName(pMember->uiWorldNameTable, &pMember->wstrArea) == FALSE )
				{
					if( pMapNameTextTable->GetSectorName(pMember->uiWorldNameTable, &pMember->wstrArea) == FALSE )
					{
						if( pMapNameTextTable->GetZoneName(pMember->uiWorldNameTable, &pMember->wstrArea) == FALSE )
							pMember->wstrArea = L"";
					}
				}

				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_MEM_POS, pMember->charID);

				break;
			}
		case SLGE_MEM_CLASS:
			{
				sGuildMember* pMember = reinterpret_cast<sGuildMember*>( GetMemberbyKey(pEvent->uiParam) );
				if( !pMember )
				{
					DBO_FAIL("Not exist guild member character ID " << pEvent->uiParam << " when member class change");
					break;
				}
				
				pMember->byClass = *reinterpret_cast<RwUInt8*>(pEvent->pData);
				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_MEM_CLASS, pMember->charID);

				break;
			}
		case SLGE_MEM_LEVEL:
			{
				sGuildMember* pMember = reinterpret_cast<sGuildMember*>( GetMemberbyKey(pEvent->uiParam) );
				if( !pMember )
				{
					DBO_FAIL("Not exist guild member character ID " << pEvent->uiParam << " when member level change");
					break;
				}
				
				pMember->byLevel = *reinterpret_cast<RwUInt8*>(pEvent->pData);
				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_MEM_LEVEL, pMember->charID);

				break;
			}
		case SLGE_MEM_REPUTATION:
			{
				sGuildMember* pMember = reinterpret_cast<sGuildMember*>( GetMemberbyKey(pEvent->uiParam) );
				if( !pMember )
				{
					DBO_FAIL("Not exist guild member character ID " << pEvent->uiParam << " when member reputation change");
					break;
				}
				
				pMember->uiReputation = *reinterpret_cast<RwUInt8*>(pEvent->pData);
				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_MEM_REPUTATION, pMember->charID);

				break;
			}
		case SLGE_MEM_ONLINE:
			{
				sGuildMember* pMember = reinterpret_cast<sGuildMember*>( GetMemberbyKey(pEvent->uiParam) );
				if( !pMember )
				{
					DBO_FAIL("Not exist guild member character ID " << pEvent->uiParam << " when member online state change");				
					break;
				}
				
				pMember->uiWorldNameTable = *reinterpret_cast<RwUInt32*>(pEvent->pData);
				CMapNameTextTable* pMapNameTextTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();

				if( pMapNameTextTable->GetAreaName(pMember->uiWorldNameTable, &pMember->wstrArea) == FALSE )
				{
					if( pMapNameTextTable->GetSectorName(pMember->uiWorldNameTable, &pMember->wstrArea) == FALSE )
					{
						if( pMapNameTextTable->GetZoneName(pMember->uiWorldNameTable, &pMember->wstrArea) == FALSE )
							pMember->wstrArea = L"unknown";
					}
				}

				if( !pMember->bOnline )
					++m_iOnlineMember;

				pMember->bOnline = TRUE;				

				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_MEM_ONLINE, pMember->charID);
				break;
			}
		case SLGE_MEM_OFFLINE:
			{
				sGuildMember* pMember = reinterpret_cast<sGuildMember*>( GetMemberbyKey(pEvent->uiParam) );
				if( !pMember )
				{
					DBO_FAIL("Not exist guild member character ID " << pEvent->uiParam << " when member online state change");
					break;
				}
				
				if( pMember->bOnline )
					--m_iOnlineMember;

				pMember->bOnline = FALSE;

				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_MEM_OFFLINE, pMember->charID);
				break;
			}
		case SLGE_CHANGE_REPUTATION:
			{
				m_uiReputation = *(RwUInt32*)pEvent->pData;
				m_uiMaxReputationEver = *(RwUInt32*)pEvent->pExData;
				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_CHANGE_REPUTATION);
				break;
			}
		case SLGE_CHANGE_FUNCTION:
			{
				m_ui64Function = *(RwUInt64*)pEvent->pData;
				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_CHANGE_FUNCTION, *(RwUInt8*)pEvent->pExData);
				break;
			}
		case SLGE_NOTICE:
			{
				ZeroMemory(&m_GuildNotice, sizeof(m_GuildNotice));

				memcpy(m_GuildNotice.awcCharName, pEvent->pData, sizeof(WCHAR)*NTL_MAX_SIZE_CHAR_NAME);
				memcpy(m_GuildNotice.awcNotice, pEvent->pExData, sizeof(WCHAR)*pEvent->uiParam);

				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_NOTICE);
				break;
			}
		case SLGE_DOGI_NFY:
			{
				// 실제 도복의 색상을 바꾸는 것은 이벤트를 보내는 곳에서 한다
				sDBO_DOGI_DATA* pDBO_DOGI_DATA = (sDBO_DOGI_DATA*)pEvent->pData;

				m_byGuildDogiColorIndex	= pDBO_DOGI_DATA->byGuildColor;
				m_byDojoDogiColorIndex	= pDBO_DOGI_DATA->byDojoColor;

				CNtlSLEventGenerator::NotifyGuildEvent(SLGE_DOGI_NFY);
				break;
			}
		default:
			{
				CNtlSLEventGenerator::NotifyGuildEvent(pEvent->iMessage, pEvent->uiParam);
				break;
			}
		}
	}
	else if( pMsg.Id == g_EventChangeGuildName )
	{
		SNtlEventChangleGuildName* pEvent = reinterpret_cast<SNtlEventChangleGuildName*>( pMsg.pData ) ;

		if( Logic_GetAvatarHandle() == pEvent->hSerialId )
		{
			wcscpy_s(m_wszGuildName, NTL_MAX_SIZE_GUILD_NAME + 1, pEvent->pwcGuildName);

			CNtlSLEventGenerator::NotifyGuildEvent(SLGE_CHANGE_GUILD_NAME);
			Logic_SetHeadNameColor( GetNtlSLGlobal()->GetSobAvatar() );
		}
	}
	else if (pMsg.Id == g_EventChangeCharName)
	{
		SNtlEventChangeCharName* pEvent = reinterpret_cast<SNtlEventChangeCharName*>(pMsg.pData);

		sGuildMember* pMember = reinterpret_cast<sGuildMember*>(GetMemberbyKey(pEvent->charid));
		if (pMember)
		{
			wcscpy_s(pMember->wszMemberName, NTL_MAX_SIZE_CHAR_NAME + 1, pEvent->wchCharName);

			CNtlSLEventGenerator::NotifyGuildEvent(SLGE_MEM_NAME, pMember->charID);
		}
	}
	else if( pMsg.Id == NPEI_IS_MAPNAME_CHANGE )
	{
		if( IsHaveGroup() == FALSE )
			NTL_RETURNVOID();

		// 길드 정보중 아바타의 위치정보는 서버에서 보내주지 않는다
		sNTL_EVENT_MAPNAME* pEvent = reinterpret_cast<sNTL_EVENT_MAPNAME*>( pMsg.pData );
		CMapNameTextTable* pMapNameTextTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
		{
			DBO_FAIL("Not exist avatar instance");
			NTL_RETURNVOID();
		}

		sGuildMember* pMember = reinterpret_cast<sGuildMember*>( GetMemberbyKey(pAvatar->GetCharID()) );
		if(pMember)
		{
			pMember->uiWorldNameTable = *(RwUInt32*)( pEvent );

			if( pMapNameTextTable->GetAreaName(pMember->uiWorldNameTable, &pMember->wstrArea) == FALSE )
			{
				if( pMapNameTextTable->GetSectorName(pMember->uiWorldNameTable, &pMember->wstrArea) == FALSE )
				{
					if( pMapNameTextTable->GetZoneName(pMember->uiWorldNameTable, &pMember->wstrArea) == FALSE )
						pMember->wstrArea = L"unknown";
				}
			}

			CNtlSLEventGenerator::NotifyGuildEvent(SLGE_MEM_POS, pMember->charID);
		}
	}

	NTL_RETURNVOID();
}