#include "stdafx.h"
#include "NtlNaviDataMng.h"
#include "UserDefinedAssert.h"


CNtlNaviDataMng::CNtlNaviDataMng( void )
{
	InitializeCriticalSection( &m_cs );

	m_pCreatedWorldInfo = NULL;
	m_pLoadedWorldInfo = NULL;
}

CNtlNaviDataMng::~CNtlNaviDataMng( void )
{
	Delete();

	DeleteCriticalSection( &m_cs );
}

bool CNtlNaviDataMng::Create( const char* pPathName )
{
	Delete();

	CreateDirectory( pPathName, NULL );

	m_strPathName = pPathName;

	return true;
}

void CNtlNaviDataMng::Delete( void )
{
	Delete_AllNaviInfo();
	Unload_AllNaviInfo();
}

CNtlNaviWorldOutDoorInfo* CNtlNaviDataMng::Create_WorldOutDoor( void )
{
	CAutoCS clCS( &m_cs );

	Assert( NULL == m_pCreatedWorldInfo );
	m_pCreatedWorldInfo = new CNtlNaviWorldOutDoorInfo;
	return (CNtlNaviWorldOutDoorInfo*)m_pCreatedWorldInfo;
}

CNtlNaviWorldInDoorInfo* CNtlNaviDataMng::Create_WorldInDoor( void )
{
	CAutoCS clCS( &m_cs );

	Assert( NULL == m_pCreatedWorldInfo );
	m_pCreatedWorldInfo = new CNtlNaviWorldInDoorInfo;
	return (CNtlNaviWorldInDoorInfo*)m_pCreatedWorldInfo;
}

CNtlNaviGroupOutDoorInfo* CNtlNaviDataMng::Create_GroupOutDoor( void )
{
	CAutoCS clCS( &m_cs );

	CNtlNaviGroupOutDoorInfo* pInfoObj = new CNtlNaviGroupOutDoorInfo;
	m_defCreatedObjList.push_back( pInfoObj );
	return pInfoObj;
}

CNtlNaviGroupInDoorInfo* CNtlNaviDataMng::Create_GroupInDoor( void )
{
	CAutoCS clCS( &m_cs );

	CNtlNaviGroupInDoorInfo* pInfoObj = new CNtlNaviGroupInDoorInfo;
	m_defCreatedObjList.push_back( pInfoObj );
	return pInfoObj;
}

CNtlNaviPropOutDoorInfo* CNtlNaviDataMng::Create_PropOutDoor( void )
{
	CAutoCS clCS( &m_cs );

	CNtlNaviPropOutDoorInfo* pInfoObj = new CNtlNaviPropOutDoorInfo;
	m_defCreatedObjList.push_back( pInfoObj );
	return pInfoObj;
}

CNtlNaviPropInDoorInfo* CNtlNaviDataMng::Create_PropInDoor( void )
{
	CAutoCS clCS( &m_cs );

	CNtlNaviPropInDoorInfo* pInfoObj = new CNtlNaviPropInDoorInfo;
	m_defCreatedObjList.push_back( pInfoObj );
	return pInfoObj;
}

void CNtlNaviDataMng::Delete_NaviInfo( CNtlNaviInfo*& pNaviInfo )
{
	CAutoCS clCS( &m_cs );

	listdef_NAVI_INFO_LIST::iterator it = m_defCreatedObjList.begin();
	for ( ; it != m_defCreatedObjList.end(); )
	{
		if ( *it == pNaviInfo )
		{
			delete *it;
			it = m_defCreatedObjList.erase( it );
			pNaviInfo = NULL;
		}
		else
		{
			++it;
		}
	}
}

void CNtlNaviDataMng::Delete_AllNaviInfoExceptWorldInfo( void )
{
	CAutoCS clCS( &m_cs );

	listdef_NAVI_INFO_LIST::iterator it = m_defCreatedObjList.begin();
	for ( ; it != m_defCreatedObjList.end(); ++it )
	{
		delete *it;
	}
	m_defCreatedObjList.clear();
}

void CNtlNaviDataMng::Delete_AllNaviInfo( void )
{
	Delete_AllNaviInfoExceptWorldInfo();

	EnterCriticalSection( &m_cs );

	if ( m_pCreatedWorldInfo )
	{
		delete m_pCreatedWorldInfo;
		m_pCreatedWorldInfo = NULL;
	}

	LeaveCriticalSection( &m_cs );
}

bool CNtlNaviDataMng::Flush( void )
{
	EnterCriticalSection( &m_cs );

	if ( m_pCreatedWorldInfo && !m_pCreatedWorldInfo->Export( m_strPathName.c_str() ) )
	{
		return false;
	}

	listdef_NAVI_INFO_LIST::iterator it = m_defCreatedObjList.begin();
	for ( ; it != m_defCreatedObjList.end(); ++it )
	{
		CNtlNaviInfo* pNaviInfo = *it;

		if ( pNaviInfo && !pNaviInfo->Export( m_strPathName.c_str() ) )
		{
			return false;
		}
	}

	LeaveCriticalSection( &m_cs );

	Delete_AllNaviInfoExceptWorldInfo();

	return true;
}

CNtlNaviWorldInfo* CNtlNaviDataMng::Load_World( void )
{
	CAutoCS clCS( &m_cs );

	m_pLoadedWorldInfo = new CNtlNaviWorldOutDoorInfo;

	if ( m_pLoadedWorldInfo->Import( m_strPathName.c_str() ) )
	{
		return m_pLoadedWorldInfo;
	}

	delete m_pLoadedWorldInfo;

	m_pLoadedWorldInfo = new CNtlNaviWorldInDoorInfo;

	if ( m_pLoadedWorldInfo->Import( m_strPathName.c_str() ) )
	{
		return m_pLoadedWorldInfo;
	}

	delete m_pLoadedWorldInfo;

	m_pLoadedWorldInfo = NULL;

	return m_pLoadedWorldInfo;
}

CNtlNaviWorldOutDoorInfo* CNtlNaviDataMng::Load_WorldOutDoor( void )
{
	CAutoCS clCS( &m_cs );

	m_pLoadedWorldInfo = new CNtlNaviWorldOutDoorInfo;

	if ( !m_pLoadedWorldInfo->Import( m_strPathName.c_str() ) )
	{
		delete m_pLoadedWorldInfo;
		m_pLoadedWorldInfo = NULL;
	}

	return (CNtlNaviWorldOutDoorInfo*)m_pLoadedWorldInfo;
}

CNtlNaviWorldInDoorInfo* CNtlNaviDataMng::Load_WorldInDoor( void )
{
	CAutoCS clCS( &m_cs );

	m_pLoadedWorldInfo = new CNtlNaviWorldInDoorInfo;

	if ( !m_pLoadedWorldInfo->Import( m_strPathName.c_str() ) )
	{
		delete m_pLoadedWorldInfo;
		m_pLoadedWorldInfo = NULL;
	}

	return (CNtlNaviWorldInDoorInfo*)m_pLoadedWorldInfo;
}

CNtlNaviGroupOutDoorInfo* CNtlNaviDataMng::Load_GroupOutDoor( unsigned int uiGroupID )
{
	CAutoCS clCS( &m_cs );

	CNtlNaviGroupOutDoorInfo* pNaviObj = new CNtlNaviGroupOutDoorInfo;

	pNaviObj->SetGroupID( uiGroupID );

	if ( !pNaviObj->Import( m_strPathName.c_str() ) )
	{
		delete pNaviObj;
		return NULL;
	}

	m_defLoadedObjList.push_back( pNaviObj );

	return pNaviObj;
}

CNtlNaviGroupInDoorInfo* CNtlNaviDataMng::Load_GroupInDoor( void )
{
	CAutoCS clCS( &m_cs );

	CNtlNaviGroupInDoorInfo* pNaviObj = new CNtlNaviGroupInDoorInfo;

	if ( !pNaviObj->Import( m_strPathName.c_str() ) )
	{
		delete pNaviObj;
		return NULL;
	}

	m_defLoadedObjList.push_back( pNaviObj );

	return pNaviObj;
}

CNtlNaviPropOutDoorInfo* CNtlNaviDataMng::Load_PropOutDoor( unsigned int uiFieldID, const char* pPath /*= NULL*/ )
{
	CAutoCS clCS( &m_cs );

	std::string strPath;
	
	if ( pPath )
	{
		strPath = pPath;
	}
	else
	{
		strPath = m_strPathName;
	}

	CNtlNaviPropOutDoorInfo* pNaviObj = new CNtlNaviPropOutDoorInfo;

	pNaviObj->SetFieldID( uiFieldID );

	if ( !pNaviObj->Import( strPath.c_str() ) )
	{
		delete pNaviObj;
		return NULL;
	}

	m_defLoadedObjList.push_back( pNaviObj );

	return pNaviObj;
}

CNtlNaviPropInDoorInfo* CNtlNaviDataMng::Load_PropInDoor( unsigned int uiBlockID, const char* pPath /*= NULL*/ )
{
	CAutoCS clCS( &m_cs );

	std::string strPath;

	if ( pPath )
	{
		strPath = pPath;
	}
	else
	{
		strPath = m_strPathName;
	}

	CNtlNaviPropInDoorInfo* pNaviObj = new CNtlNaviPropInDoorInfo;

	pNaviObj->SetBlockID( uiBlockID );

	if ( !pNaviObj->Import( strPath.c_str() ) )
	{
		delete pNaviObj;
		return NULL;
	}

	m_defLoadedObjList.push_back( pNaviObj );

	return pNaviObj;
}

void CNtlNaviDataMng::Unload_NaviInfo( CNtlNaviInfo*& pNaviInfo )
{
	CAutoCS clCS( &m_cs );

	if ( m_pLoadedWorldInfo == pNaviInfo )
	{
		delete m_pLoadedWorldInfo;

		m_pLoadedWorldInfo = NULL;

		pNaviInfo = NULL;

		return;
	}

	listdef_NAVI_INFO_LIST::iterator it = m_defLoadedObjList.begin();
	for ( ; it != m_defLoadedObjList.end(); )
	{
		if ( *it == pNaviInfo )
		{
			delete pNaviInfo;

			it = m_defLoadedObjList.erase( it );

			pNaviInfo = NULL;
		}
		else
		{
			++it;
		}
	}
}

void CNtlNaviDataMng::Unload_AllNaviInfo( void )
{
	CAutoCS clCS( &m_cs );

	if ( m_pLoadedWorldInfo )
	{
		delete m_pLoadedWorldInfo;
		m_pLoadedWorldInfo = NULL;
	}

	listdef_NAVI_INFO_LIST::iterator it = m_defLoadedObjList.begin();
	for ( ; it != m_defLoadedObjList.end(); ++it )
	{
		delete *it;
	}
	m_defLoadedObjList.clear();
}

