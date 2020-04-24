#include "precomp_dboclient.h"
#include "Lobby.h"

// core
#include "NtlDebug.h"

CLobby::CLobby()
:hServer(INVALID_SERVERFARMID)
,eServerState(DBO_SERVER_STATUS_UP)
,bySelectedChannelIndex(INVALID_BYTE)
,m_bySelectedCharacterIndex(INVALID_BYTE)
,byCharacterCount(0)
,byChannelCount(0)
,bSuccess_Loaded_Last_Used_Character(FALSE)
,bLoadedLastCharacter(FALSE)
{
	ZeroMemory(awcServerName, sizeof(awcServerName));
}

CLobby::~CLobby()
{

}

RwBool CLobby::Create()
{
	NTL_FUNCTION( "CLobby::Create" );

	NTL_RETURN(TRUE);
}

VOID CLobby::Destroy()
{
	NTL_FUNCTION("CLobby::Destroy");

	NTL_RETURNVOID();
}

VOID CLobby::SetServerInfo(sDBO_GAME_SERVER_FARM_INFO* pServerInfo)
{
	if( !pServerInfo )
	{
		DBO_FAIL("Null pointer of server infomation");
		return;
	}

	hServer			= pServerInfo->serverFarmId;
	eServerState	= (eDBO_SERVER_STATUS)pServerInfo->byServerStatus;
	uiUseRate		= pServerInfo->dwLoad;
	memset(awcServerName, 0, sizeof(awcServerName));

	if (pServerInfo->wszGameServerFarmName)
	{
		wcscpy_s(awcServerName, NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE + 1, pServerInfo->wszGameServerFarmName);
	}
}

VOID CLobby::RefreshServerInfo(eDBO_SERVER_STATUS eStatus, RwUInt32 uiUseRate)
{
	this->eServerState = eStatus;
	this->uiUseRate = uiUseRate;
}

VOID CLobby::SetChannelInfo(RwUInt8 byCount, sDBO_GAME_SERVER_CHANNEL_INFO* pChannelInfo)
{
	if( !pChannelInfo )
	{
		DBO_FAIL("Null pointer of channel infomation");
		return;
	}

	if( NTL_MAX_SERVER_CHANNEL_COUNT_IN_SERVER_FARM < byCount )
	{
		DBO_FAIL("Too many channel infomation : " << byCount);
		return;
	}

	byChannelCount = byCount;
	memcpy(&atChannel, pChannelInfo, sizeof(sDBO_GAME_SERVER_CHANNEL_INFO) * byChannelCount);
}

VOID CLobby::SetCharacterInfo(RwUInt8 byCount, sPC_SUMMARY* pPC_SUMMARY)
{
	if( !pPC_SUMMARY )
	{
		DBO_FAIL("Null pointer of character infomation");
		return;
	}

	if( NTL_MAX_COUNT_USER_CHAR_SLOT < byCount )
	{
		DBO_FAIL("Too many character infomation : " << byCount);
		return;
	}

	byCharacterCount = byCount;

	for( RwUInt8 i = 0 ; i < byCharacterCount ; ++i )
	{
		memcpy(&atLobbyCharacter[i].tSummary, &pPC_SUMMARY[i], sizeof(sPC_SUMMARY));
	}
}

VOID CLobby::SetDeletingCharcterInfo(sDELETE_WAIT_CHARACTER_INFO* pDELETE_WAIT_CHARACTER_INFO)
{
	if( !pDELETE_WAIT_CHARACTER_INFO )
	{
		DBO_FAIL("Null pointer of deleting character infomation");
		return;
	}

	//for( RwUInt8 i = 0 ; i < NTL_MAX_COUNT_USER_CHAR_SLOT ; ++i )
	{
		for( RwUInt8 k = 0 ; k < byCharacterCount ; k++ )
		{
			if( atLobbyCharacter[k].tSummary.charId == pDELETE_WAIT_CHARACTER_INFO[k].charId )
			{
				RwReal fTime_util_Deletion = (RwReal)(pDELETE_WAIT_CHARACTER_INFO[k].dwRemainTick) / 1000.f;

				atLobbyCharacter[k].bReservatedDelete		= TRUE;
				atLobbyCharacter[k].fTime_util_Deletion		= fTime_util_Deletion;
			}
		}
	}
}

RwBool CLobby::AddCharacter(sPC_SUMMARY* pSummary)
{
	if( !pSummary )
		return FALSE;

	if( byCharacterCount >= NTL_MAX_COUNT_USER_CHAR_SLOT )
		return FALSE;


	memcpy(&atLobbyCharacter[byCharacterCount].tSummary, pSummary, sizeof(sPC_SUMMARY));
	atLobbyCharacter[byCharacterCount].bReservatedDelete	= FALSE;
	atLobbyCharacter[byCharacterCount].fTime_util_Deletion	= 0.f;

	++byCharacterCount;

	return TRUE;
}

RwUInt8 CLobby::RemoveCharacter(CHARACTERID charID)
{
	if( INVALID_CHARACTERID == charID )
		return INVALID_BYTE;

	RwUInt8	byDeletedIndex	= INVALID_BYTE;

	for( RwUInt8 i = 0 ; i < NTL_MAX_COUNT_USER_CHAR_SLOT ; ++i )
	{
		if( INVALID_BYTE != byDeletedIndex && i > 0 )
		{
			memcpy(&atLobbyCharacter[i-1].tSummary, &atLobbyCharacter[i].tSummary, sizeof(sPC_SUMMARY));
			atLobbyCharacter[i-1].bReservatedDelete		= atLobbyCharacter[i].bReservatedDelete;
			atLobbyCharacter[i-1].fTime_util_Deletion	= atLobbyCharacter[i].fTime_util_Deletion;
		}

		if( atLobbyCharacter[i].tSummary.charId == charID )
		{
			byDeletedIndex = i;
			--byCharacterCount;
		}
	}


	if( INVALID_BYTE == byDeletedIndex )
		return INVALID_BYTE;

	// 마직막 슬롯은 항상 비운다
	atLobbyCharacter[NTL_MAX_COUNT_USER_CHAR_SLOT-1].tSummary.charId = INVALID_CHARACTERID;

	return byDeletedIndex;	
}

VOID CLobby::SetReserveDeleteCharacter(CHARACTERID charID, RwReal fTime_util_Deletion)
{
	for( RwUInt8 i = 0 ; i < byCharacterCount ; ++i )
	{
		if( atLobbyCharacter[i].tSummary.charId == charID )
		{
			atLobbyCharacter[i].bReservatedDelete	= TRUE;
			atLobbyCharacter[i].fTime_util_Deletion	= fTime_util_Deletion;
			return;
		}
	}
}

VOID CLobby::CancelDeleteCharacter(CHARACTERID charID)
{
	for( RwUInt8 i = 0 ; i < byCharacterCount ; ++i )
	{
		if( atLobbyCharacter[i].tSummary.charId == charID )
		{
			atLobbyCharacter[i].bReservatedDelete	= FALSE;
			atLobbyCharacter[i].fTime_util_Deletion	= 0.f;
			return;
		}
	}
}

VOID CLobby::Rename(CHARACTERID charID, const WCHAR* pwcName)
{
	if( !pwcName )
		return;

	for( RwUInt8 i = 0 ; i < byCharacterCount ; ++i )
	{
		if( atLobbyCharacter[i].tSummary.charId == charID )
		{
			RwUInt32 uiNameLength = (RwUInt32)wcslen(pwcName);

			if( NTL_MAX_SIZE_CHAR_NAME < uiNameLength )
				uiNameLength = NTL_MAX_SIZE_CHAR_NAME;

			atLobbyCharacter[i].tSummary.bNeedNameChange = false;
			memcpy(atLobbyCharacter[i].tSummary.awchName, pwcName, sizeof(WCHAR)*uiNameLength);
			return;
		}
	}
}

sLOBBY_CHARACTER* CLobby::GetCharacter(CHARACTERID charID)
{
	if( INVALID_CHARACTERID == charID )
		return NULL;

	for( RwUInt8 i = 0 ; i < byCharacterCount ; ++i )
	{
		if( atLobbyCharacter[i].tSummary.charId == charID )
			return &(atLobbyCharacter[i]);
	}

	return NULL;
}