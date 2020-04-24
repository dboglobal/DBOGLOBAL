/*****************************************************************************
* File			: NtlPathSeamlessLoadingManager.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2009. 03. 19
* Abstract		: Pathengine seamless loading manager
*****************************************************************************
* Desc:
*
*****************************************************************************/

#ifndef _NTL_PATH_SEAMLESS_LOADING_MANAGER_H_
#define _NTL_PATH_SEAMLESS_LOADING_MANAGER_H_

#include "NtlPathCS.h"
#include "NtlPathSeamlessLoader.h"

#include <list>

class CNtlSeamlessMesh;
class CNtlPathSeamlessLoadingManager
{
public:
	enum { eMAX_THREAD_CNT = 1 };

	typedef std::map < std::string, CNtlSeamlessMesh* > mapdef_seamMesh;
	typedef std::list < CNtlSeamlessMesh* > listdef_seamMesh;

protected:
	CNtlPathSeamlessLoadingManager();

public:
	~CNtlPathSeamlessLoadingManager();

	static CNtlPathSeamlessLoadingManager* GetInstance()
	{
		static CNtlPathSeamlessLoadingManager instance;
		return &instance;
	}

	bool	Create();
	void	Update( float fElapsed );
	void	Destroy();

	// 로딩 예약을 한다.
	void	LoadReservation( CNtlSeamlessMesh* pMesh );

	// 가장 먼저 로딩 예약 된 것을 가져온다.
	CNtlSeamlessMesh*	GetLoadReservation();

	// 로딩 예약되어 있는 것들 중에 거리에서 벗어난 Tile들을 삭제한다.
	void	RemoveReservationByRangeOver( cHorizontalRange& range );

	// 완료가 되었다는 것을 알린다.
	void	Loaded( CNtlSeamlessMesh* pMesh );
	
	// 가장 먼저 로딩 완료된 것을 가져온다.
	CNtlSeamlessMesh*	GetLoadedMesh();

	HANDLE				GetEvent();
	void				ResetThreadEvent();
	void				SetThreadEvent();

	void	SetExit(void);
	bool	IsExit(void);

protected:
	CNtlPathCS			m_ReservationCS;
	mapdef_seamMesh		m_mapLoadReservation;
	listdef_seamMesh	m_listLoadReservation;	// 순서를 저장하기 위해

	CNtlPathCS			m_LoadedCS;
	mapdef_seamMesh		m_mapLoaded;
	listdef_seamMesh	m_listLoaded;

	// Thread를 제어하기 위한 이벤트

	CNtlPathCS			m_EventCS;
	HANDLE				m_hEventForThread;

	// Thread의 Handle
	HANDLE				m_ahThread[eMAX_THREAD_CNT];
	DWORD				m_dwThreadID[eMAX_THREAD_CNT];

	// Thread가 생성되었는지 확인하기 위한 부울
	bool				m_bCreatedThread;
	
	// 이 객체가 Destroy 가 되었다는 것을 알림
	CNtlPathCS			m_ExitCS;
	bool				m_bExit;

	// ThreadLoader
	CNtlPathSeamlessLoader	m_ThreadLoader;
};

static CNtlPathSeamlessLoadingManager* GetNtlPathEngineLoadingManager()
{
	return CNtlPathSeamlessLoadingManager::GetInstance();
}

#endif