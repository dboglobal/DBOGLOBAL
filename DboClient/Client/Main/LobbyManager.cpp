#include "precomp_dboclient.h"
#include "LobbyManager.h"

// core
#include "NtlDebug.h"

// client
#include "DboEventGenerator.h"
#include "DboGlobal.h"

CLobbyManager* CLobbyManager::m_pInstance = NULL;


CLobbyManager* CLobbyManager::GetInstance()
{
	return m_pInstance;
}

CLobbyManager::CLobbyManager()
:m_hSelectedServer(INVALID_SERVERFARMID)
,m_bWaitServerListInfo(FALSE)
,m_bWaitChannelInfo(FALSE)
{	
}

CLobbyManager::~CLobbyManager()
{	
}

RwBool CLobbyManager::CreateInstance()
{
	NTL_FUNCTION("CLobbyManager::CreateInstance");

	m_pInstance = NTL_NEW CLobbyManager;
	if( !m_pInstance )
	{
		DBO_FAIL("Fail create CLobbyManager instance");
		NTL_RETURN(FALSE);
	}	

	NTL_RETURN(TRUE);
}

VOID CLobbyManager::DestoryInstance()
{
	NTL_FUNCTION("CLobbyManager::DestoryInstance");

	if( !m_pInstance )
		NTL_RETURNVOID();

	m_pInstance->ClearLobby();

	NTL_DELETE(m_pInstance);

	NTL_RETURNVOID();
}

VOID CLobbyManager::ClearLobby()
{
	m_hSelectedServer		= INVALID_SERVERFARMID;


	ITER_LOBBY it_Lobby = m_mapLobby.begin();
	for( ; it_Lobby != m_mapLobby.end() ; ++it_Lobby )
	{
		CLobby* pLobby = it_Lobby->second;
		NTL_DELETE(pLobby);
	}

	m_mapLobby.clear();
}

VOID CLobbyManager::Update(RwReal fElapsed)
{
	ITER_LOBBY it_Lobby = m_mapLobby.begin();
	for( ; it_Lobby != m_mapLobby.end() ; ++it_Lobby )
	{
		CLobby* pLobby = it_Lobby->second;

		for( RwUInt8 i = 0 ; i < pLobby->GetCharacterCount() ; ++i )
		{
			sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter(i);
			if( !pLOBBY_CHARACTER )
				continue;

			if( pLOBBY_CHARACTER->bReservatedDelete )
			{
				pLOBBY_CHARACTER->fTime_util_Deletion -= fElapsed;
				if( pLOBBY_CHARACTER->fTime_util_Deletion <= 0.f )
					pLOBBY_CHARACTER->fTime_util_Deletion = 0.f;
			}
		}
	}
}

VOID CLobbyManager::SetServerInfo(sDBO_GAME_SERVER_FARM_INFO* pServerInfo)
{
	if( !pServerInfo )
	{
		DBO_FAIL("Invalid server farm info pointer");
		return;
	}

	CLobby* pLobby = GetLobby(pServerInfo->serverFarmId);
	if( !pLobby )
	{
		// 새로운 정보를 받았다
		pLobby = NTL_NEW CLobby;
		if( !pLobby->Create() )
		{
			pLobby->Destroy();
			NTL_DELETE(pLobby);
			return;
		}
	}


	pLobby->SetServerInfo(pServerInfo);

	m_mapLobby[pServerInfo->serverFarmId] = pLobby;
}

VOID CLobbyManager::CleanUp_Deleted_Character()
{
	ITER_LOBBY it_Lobby = m_mapLobby.begin();
	for( ; it_Lobby != m_mapLobby.end() ; ++it_Lobby )
	{
		SERVER_HANDLE	hServer	= it_Lobby->first;
		CLobby*			pLobby	= it_Lobby->second;

		for( RwUInt8 i = (NTL_MAX_COUNT_USER_CHAR_SLOT-1) ; ( 0 <= i && i < NTL_MAX_COUNT_USER_CHAR_SLOT )  ; --i )
		{
			sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter(i);
			if( !pLOBBY_CHARACTER )
				continue;

			if( pLOBBY_CHARACTER->bReservatedDelete )
			{
				if( pLOBBY_CHARACTER->fTime_util_Deletion <= 0.f )
				{
					pLobby->RemoveCharacter(pLOBBY_CHARACTER->tSummary.charId);
				}
			}
		}
	}
}

CLobby* CLobbyManager::GetLobby(SERVER_HANDLE hServer)
{
	ITER_LOBBY it_Lobby = m_mapLobby.find(hServer);
	if( it_Lobby == m_mapLobby.end() )
		return NULL;

	return it_Lobby->second;
}

RwBool CLobbyManager::IsFistAcceptServer()
{
	SConnectData *pConData = GetDboGlobal()->GetConnectData();

	// request character & channel info
	if(pConData->byLastServerFarmID == INVALID_SERVERFARMID )
		return TRUE;

	return FALSE;
}