/******************************************************************************
* File			: LobbyManager.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 8. 6
* Abstract		: 
* Update		: 
*****************************************************************************
* Desc			: 실제 본 게임전까지의 로비를 관리한다
*****************************************************************************/

#pragma once

// share
#include "NtlCharacter.h"

// client
#include "Lobby.h"


struct sDBO_GAME_SERVER_FARM_INFO;

typedef std::map<SERVER_HANDLE, CLobby*>					MAP_LOBBY;
typedef std::map<SERVER_HANDLE, CLobby*>::iterator			ITER_LOBBY;


class CLobbyManager
{
public:
	static CLobbyManager*	GetInstance(void);
	static RwBool			CreateInstance();
	static VOID				DestoryInstance();

	CLobbyManager();
	virtual ~CLobbyManager();

	VOID			Update(RwReal fElapsed);

	VOID			ClearLobby();
	VOID			CleanUp_Deleted_Character();


	VOID			SetWaitServerInfo(RwBool bWait);
	VOID			SetWaitChannelInfo(RwBool bWait);
	VOID			SetSelectedServerHandle(SERVER_HANDLE hServer);
	VOID			SetServerInfo(sDBO_GAME_SERVER_FARM_INFO* pServerInfo);
	VOID			SetReservedRename(const WCHAR* pwcName);


	ITER_LOBBY		GetLobbyBegin();
	ITER_LOBBY		GetLobbyEnd();

	SERVER_HANDLE	GetSelectedServerHandle();
	CLobby*			GetLobby(SERVER_HANDLE hServer);
	RwUInt32		GetLobbyCount();
	const WCHAR*	GetReservedRename();


	RwBool			IsWaitingServerListInfo();
	RwBool			IsWaitingChannelInfo();	
	RwBool			IsFistAcceptServer();				//< t: 유저가 접속했던 서버가 없다


protected:
	static CLobbyManager* m_pInstance;

	SERVER_HANDLE	m_hSelectedServer;
	RwBool			m_bWaitServerListInfo;
	RwBool			m_bWaitChannelInfo;

	std::wstring	m_wstrReservedRename;

	MAP_LOBBY		m_mapLobby;
};

inline CLobbyManager* GetLobbyManager()
{
	return CLobbyManager::GetInstance(); 
}

inline VOID CLobbyManager::SetWaitServerInfo(RwBool bWait)
{
	m_bWaitServerListInfo = bWait;
}

inline VOID CLobbyManager::SetWaitChannelInfo(RwBool bWait)
{
	m_bWaitChannelInfo = bWait;
}

inline VOID CLobbyManager::SetSelectedServerHandle(SERVER_HANDLE hServer)
{
	m_hSelectedServer = hServer;
}

inline VOID CLobbyManager::SetReservedRename(const WCHAR* pwcName)
{
	if( !pwcName )
		return;

	m_wstrReservedRename = pwcName;
}

inline ITER_LOBBY CLobbyManager::GetLobbyBegin()
{
	return m_mapLobby.begin();
}

inline ITER_LOBBY CLobbyManager::GetLobbyEnd()
{
	return m_mapLobby.end();
}

inline SERVER_HANDLE CLobbyManager::GetSelectedServerHandle()
{
	return m_hSelectedServer;
}

inline RwUInt32 CLobbyManager::GetLobbyCount()
{
	return m_mapLobby.size();
}

inline const WCHAR* CLobbyManager::GetReservedRename()
{
	return m_wstrReservedRename.c_str();
}

inline RwBool CLobbyManager::IsWaitingServerListInfo()
{
	return m_bWaitServerListInfo;
}

inline RwBool CLobbyManager::IsWaitingChannelInfo()
{
	return m_bWaitChannelInfo;
}