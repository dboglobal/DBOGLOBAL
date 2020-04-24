#ifndef __NTL_NAVI_DATA_MNG_H__
#define __NTL_NAVI_DATA_MNG_H__


#include "NtlNaviData.h"


class CNtlNaviDataMng
{
public:
	class CAutoCS
	{
	protected:
		CRITICAL_SECTION*			m_pCS;

	public:
		CAutoCS( CRITICAL_SECTION* pCS ) { m_pCS = pCS; EnterCriticalSection( m_pCS ); }
		~CAutoCS( void ) { LeaveCriticalSection( m_pCS ); }
	};

public:
	typedef std::list< CNtlNaviInfo* > listdef_NAVI_INFO_LIST;

protected:
	CRITICAL_SECTION				m_cs;

	std::string						m_strPathName;

	// Export 용 변수
	CNtlNaviInfo*					m_pCreatedWorldInfo;
	listdef_NAVI_INFO_LIST			m_defCreatedObjList;

	// Import 용 변수
	CNtlNaviWorldInfo*				m_pLoadedWorldInfo;
	listdef_NAVI_INFO_LIST			m_defLoadedObjList;

public:
	CNtlNaviDataMng( void );
	~CNtlNaviDataMng( void );

public:
	bool							Create( const char* pPathName );
	void							Delete( void );

	//////////////////////////////////////////////////////////////////////////
	//
	//	Export
	//
	//////////////////////////////////////////////////////////////////////////

	// Info entity 생성
	CNtlNaviWorldOutDoorInfo*		Create_WorldOutDoor( void );
	CNtlNaviWorldInDoorInfo*		Create_WorldInDoor( void );
	CNtlNaviGroupOutDoorInfo*		Create_GroupOutDoor( void );
	CNtlNaviGroupInDoorInfo*		Create_GroupInDoor( void );
	CNtlNaviPropOutDoorInfo*		Create_PropOutDoor( void );
	CNtlNaviPropInDoorInfo*			Create_PropInDoor( void );

	// Info entity 제거
	void							Delete_NaviInfo( CNtlNaviInfo*& pNaviInfo );
	void							Delete_AllNaviInfoExceptWorldInfo( void );
	void							Delete_AllNaviInfo( void );

	// 현재 생성된 모든 데이터를 파일에 저장한다
	bool							Flush( void );

	//////////////////////////////////////////////////////////////////////////
	//
	//	Import
	//
	//////////////////////////////////////////////////////////////////////////

	CNtlNaviWorldInfo*				GetLoadedWorld( void );

	CNtlNaviWorldInfo*				Load_World( void );
	CNtlNaviWorldOutDoorInfo*		Load_WorldOutDoor( void );
	CNtlNaviWorldInDoorInfo*		Load_WorldInDoor( void );
	CNtlNaviGroupOutDoorInfo*		Load_GroupOutDoor( unsigned int uiGroupID );
	CNtlNaviGroupInDoorInfo*		Load_GroupInDoor( void );
	CNtlNaviPropOutDoorInfo*		Load_PropOutDoor( unsigned int uiFieldID, const char* pPath = NULL );
	CNtlNaviPropInDoorInfo*			Load_PropInDoor( unsigned int uiBlockID, const char* pPath = NULL );

	void							Unload_NaviInfo( CNtlNaviInfo*& pNaviInfo );
	void							Unload_AllNaviInfo( void );
};


inline CNtlNaviWorldInfo* CNtlNaviDataMng::GetLoadedWorld( void )
{
	return m_pLoadedWorldInfo;
}


#endif