/*****************************************************************************
* File			: NtlPathEngineHelper.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2009. 03. 16
* Abstract		: NTL Pathengine seamless loading/pathfinding module
*****************************************************************************
* Desc:
*
* 현재 agent position을 기준으로 가장 가까운 위치의 Tile 들 부터 로드하여 list에 넣어 놓는다.
* 03. 16 기준 현재 작성중
*****************************************************************************/

#ifndef _NTL_PATH_SEAMLESS_H_
#define _NTL_PATH_SEAMLESS_H_

#include "../PathEngine/i_pathengine.h"
#include "NtlSeamlessMesh.h"
#include "NtlSimpleDOM.h"
#include "NtlPathSeamlessLoadingManager.h"

class CNtlMeshFederation;
class CNtlMeshFederationContainer;

class CNtlPathSeamlessManager
{
public:
	CNtlPathSeamlessManager();
	~CNtlPathSeamlessManager();

	bool	Create();
	void	Update( float fElapsed );
	void	Destroy();

	void	ClearMeshFederationContainer( void );
	void	ClearSeamlessMesh( void );

	void	AddAgentSize( int nAgnetSize );
	void	GetAgentList( std::vector< int >& listAgent );

	void	Start( int nWorldIndex );
	void	Stop( void );

	// Change World
	void	ChangeWorld( int nWorldIndex );
	bool	IsLoadedWorld( int nWorldIndex );
	int		GetLoadedWorldSize();

	// Mesh 관련
	bool	IsLoadedMesh( CNtlSeamlessMesh* pMesh );
	bool	IsLoadedMesh( int nWorldIndex, int nFieldIndex, int nTileIndex );

	bool	IsSeamlessRangeOver( CNtlSeamlessMesh* pMesh );

	bool	UpdatePosition( int nPositionX, int nPositionY, int nPositionZ = 0);
	
	// Path 요청 함수
	iPath*	FindShortestPath( int nAgentSize, int nSrcPositionX, int nSrcPositionY,
		int nSrcPositionZ, int nDestPositionX, int nDestPositionY , int nDestPositionZ,
		std::list<cPosition>& listPath);

protected:
	CNtlSeamlessMesh*	LoadSeamlessMesh( int nWorldIndex, int nFieldIndex, int nTileIndex, cHorizontalRange& rangeAtWorld, cHorizontalRange& rangeAtLocal, int nCenterAtWorldX, int nCenterAtWorldY );
	CNtlSeamlessMesh*	CreateSeamlessMesh( int nWorldIndex, int nFieldIndex, int nTileIndex, cHorizontalRange& rangeAtWorld, cHorizontalRange& rangeAtLocal, int nCenterAtWorldX, int nCenterAtWorldY );

	CNtlMeshFederationContainer* CreateMeshFederationContainer( int nWorldIndex );
	bool				LoadWorldMeshFederation( int nWorldIndex );
	bool				LoadWorldMeshFederationByInfoScript( int nWorldIndex, const CNtlSimpleDOM& worldinfoScript );

	void				FindOverlappedMeshFederation( int nPositionX, int nPositionY, int nPositionZ,
		std::list< CNtlMeshFederation* >& listOverlapped );

	void				FindOverlappedMeshFederationByRange( cHorizontalRange range, std::list < CNtlMeshFederation* >& listOverlapped );

	static bool				SortSeamlessMeshCompare( CNtlSeamlessMesh* pLeft, CNtlSeamlessMesh* pRight );

protected:
	typedef std::map< int, CNtlMeshFederationContainer* > mapdef_WorldMeshFederation;
	mapdef_WorldMeshFederation		m_mapWorldPath;

	typedef std::list < CNtlSeamlessMesh* > listdef_NtlSeamlessMesh;
	listdef_NtlSeamlessMesh			m_listMesh;

	int								m_nCurrentWorldIndex;

	int								m_nSeamlessRange;
	int								m_nCurrentPositionX;
	int								m_nCurrentPositionY;
	int								m_nCurrentPositionZ;

	// 구동되고 있는 PathManager의 AgentSize들
	typedef std::vector< int >		vecdef_AgentSize;
	vecdef_AgentSize				m_vecAgentSize;

	// 현재 Seamless가 구동되고 있는가?
	bool							m_bIsStart;
};

#endif