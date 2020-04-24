#include "precomp_trigger.h"
#include "NtlTSGroup.h"
#include "NtlTSCont.h"
#include "NtlTSCoreStatic.h"
#include "NtlTSRecv.h"


/**
	Group entity
*/


NTL_TS_IMPLEMENT_RTTI( CNtlTSGroup, CNtlTSEntity )


CNtlTSGroup::CNtlTSGroup( void )
{
	m_tgID = NTL_TS_TG_ID_INVALID;
}

CNtlTSGroup::~CNtlTSGroup( void )
{
}

NTL_TS_TG_ID CNtlTSGroup::SetID( NTL_TS_TG_ID uID )
{
	NTL_TS_TG_ID tgOld = m_tgID;
	m_tgID = uID;
	return tgOld;
}

CNtlTSCont* CNtlTSGroup::GetChildCont( NTL_TS_TC_ID tcID )
{
	mapdef_ContList::iterator it = m_defChildContList.find( tcID );
	if ( it == m_defChildContList.end() ) return 0;
	return it->second;
}

CNtlTSCont* CNtlTSGroup::GetBeginChildCont( void )
{
	if ( m_defChildContList.empty() ) return 0;
	m_itChildContList = m_defChildContList.begin();
	return m_itChildContList->second;
}

CNtlTSCont* CNtlTSGroup::GetNextChildCont( void )
{
	++m_itChildContList;
	if ( m_itChildContList == m_defChildContList.end() ) return 0;
	return m_itChildContList->second;
}

void CNtlTSGroup::AttachChildCont( CNtlTSCont* pChildCont )
{
	if ( 0 == pChildCont )
	{
		printf( "Can not attach the container. ParamInfo[%s]. [%s]", "NULL", TS_CODE_TRACE() );
		return;
	}

	m_defChildContList[pChildCont->GetID()] = pChildCont;
}

CNtlTSCont* CNtlTSGroup::DetachChildCont( NTL_TS_TC_ID tcID )
{
	mapdef_ContList::iterator it = m_defChildContList.find( tcID );

	if ( it != m_defChildContList.end() )
	{
		CNtlTSCont* pContObj = it->second;
		m_defChildContList.erase( it );
		return pContObj;
	}
	else
	{
		printf( "Can not find the container. ParamInfo[%d]. [%s]", tcID, TS_CODE_TRACE() );
	}

	return 0;
}

CNtlTSCont* CNtlTSGroup::DetachChildCont( CNtlTSCont* pChildCont )
{
	if ( 0 == pChildCont )
	{
		printf( "Can not detach the container. ParamInfo[%s]. [%s]", "NULL", TS_CODE_TRACE() );
		return 0;
	}

	mapdef_ContList::iterator it = m_defChildContList.find( pChildCont->GetID() );

	if ( it != m_defChildContList.end() )
	{
		m_defChildContList.erase( it );
		return pChildCont;
	}
	else
	{
		printf( "Can not find the container. ParamInfo[%d]. [%s]", pChildCont->GetID(), TS_CODE_TRACE() );
	}

	return 0;
}

NTL_TSRESULT CNtlTSGroup::SearchAll( CNtlTSRecv* pTSRecv, void* pParam )
{
	NTL_TSRESULT tsResult = 0;

	tsResult |= pTSRecv->Search( this, pParam );

	mapdef_ContList::iterator itChildCont = m_defChildContList.begin();
	for ( ; itChildCont != m_defChildContList.end(); ++itChildCont )
	{
		tsResult |= itChildCont->second->SearchAll( pTSRecv, pParam );
	}

	return tsResult;
}

NTL_TSRESULT CNtlTSGroup::SearchTarget( NTL_TS_TC_ID tcID, CNtlTSRecv* pTSRecv, void* pParam )
{
	NTL_TSRESULT tsResult = 0;

	tsResult |= pTSRecv->Search( this, pParam );

	CNtlTSCont* pTSChildCont = GetChildCont( tcID );
	if ( 0 != pTSChildCont )
	{
		tsResult |= pTSChildCont->SearchAll( pTSRecv, pParam );
	}
	else
	{
		tsResult |= NTL_TSRESULT_TYPE_ERROR;
	}

	return tsResult;
}

NTL_TSRESULT CNtlTSGroup::RunTarget( NTL_TS_TC_ID tcID, CNtlTSRecv* pTSRecv, void* pParam )
{
	NTL_TSRESULT tsResult = 0;

	CNtlTSCont* pTSChildCont = GetChildCont( tcID );
	if ( 0 != pTSChildCont )
	{
		tsResult |= pTSChildCont->RunAll( pTSRecv, pParam );
	}
	else
	{
		tsResult |= NTL_TSRESULT_TYPE_ERROR;
	}

	return tsResult;
}

CNtlTSScrObject* CNtlTSGroup::BeginChildForScript( void )
{
	if ( m_defChildContList.empty() ) return 0;
	m_itChildContList = m_defChildContList.begin();
	return m_itChildContList->second;
}

CNtlTSScrObject* CNtlTSGroup::NextChildForScript( void )
{
	++m_itChildContList;
	if ( m_itChildContList == m_defChildContList.end() ) return 0;
	return m_itChildContList->second;
}

void CNtlTSGroup::AttachChildForScript( CNtlTSScrObject* pChild )
{
	if ( 0 == pChild )
	{
		printf( "Can not attach the script object. ParamInfo[%s]. [%s]", "NULL", TS_CODE_TRACE() );
		return;
	}

	if ( pChild->IsDerivedClass( "CNtlTSCont" ) )
	{
		m_defChildContList[((CNtlTSCont*)pChild)->GetID()] = (CNtlTSCont*)pChild;
	}
	else
	{
		printf( "Can not do type cast from CNtlTSScrObject to CNtlTSCont. ParamInfo[%s]. [%s]", pChild->GetClassName(), TS_CODE_TRACE() );
	}
}

void CNtlTSGroup::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEntity::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "gid" ) )
	{
		SetID( (NTL_TS_TG_ID)clProperty.GetValueAsInt( "gid" ) );
	}
}

void CNtlTSGroup::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEntity::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetID() );
	clProperty.m_defProperty["gid"] = g_NtlTSString;
}

void CNtlTSGroup::MakeAllObjectList( std::vector<CNtlTSScrObject*>& defObjList )
{
	defObjList.push_back( this );

	mapdef_ContList::iterator itChildCont = m_defChildContList.begin();
	for ( ; itChildCont != m_defChildContList.end(); ++itChildCont )
	{
		itChildCont->second->MakeAllObjectList( defObjList );
	}
}
