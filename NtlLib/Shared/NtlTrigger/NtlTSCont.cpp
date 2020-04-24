#include "precomp_trigger.h"
#include "NtlTSCont.h"
#include "NtlTSCoreStatic.h"
#include "NtlTSEntity.h"
#include "NtlTSEvent.h"
#include "NtlTSCond.h"
#include "NtlTSAction.h"
#include "NtlTSRecv.h"


/**
	Container entity
*/


NTL_TS_IMPLEMENT_RTTI( CNtlTSCont, CNtlTSEntity )


CNtlTSCont::CNtlTSCont( void )
: m_tcID( NTL_TS_TC_ID_INVALID )
{
}

CNtlTSCont::~CNtlTSCont( void )
{
}

NTL_TS_TC_ID CNtlTSCont::SetID( NTL_TS_TC_ID tcID )
{
	NTL_TS_TC_ID hOld = tcID;
	m_tcID = tcID;
	return hOld;
}

CNtlTSEntity* CNtlTSCont::FindChildEntity( NTL_TS_TA_ID taId )
{
	if ( NTL_TS_TA_ID_INVALID == taId ) return 0;

	int nSize = (int)m_defChildEntityList.size();
	for ( int i = 0; i < nSize; ++i )
	{
		if ( m_defChildEntityList[i]->IsDerivedClass( "CNtlTSAction" ) )
		{
			if ( ((CNtlTSAction*)m_defChildEntityList[i])->GetActionId() == taId ) return m_defChildEntityList[i];
		}
	}

	return 0;
}

CNtlTSEntity* CNtlTSCont::GetEventEntity( void )
{
	unsigned int uiEntityType;
	int nSize = (int)m_defChildEntityList.size();
	for ( int i = 0; i < nSize; ++i )
	{
		uiEntityType = (unsigned int)m_defChildEntityList[i]->GetEntityType();
		if ( NTL_TSENTITY_TYPE_EVENT_BEGIN < uiEntityType &&
			 uiEntityType < NTL_TSENTITY_TYPE_EVENT_END )
		{
			return m_defChildEntityList[i];
		}
	}

	return NULL;
}

bool CNtlTSCont::AttachChildEntity( CNtlTSEntity* pEntity )
{
	if ( 0 == pEntity )
	{
		printf( "Can not find the entity. ParamInfo[%s]. [%s]", pEntity ? pEntity->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	m_defChildEntityList.push_back( pEntity );

	return true;
}

void CNtlTSCont::DetachChildEntity( CNtlTSEntity* pEntity )
{
	vecdef_EntityList::iterator it;

	it = std::find( m_defChildEntityList.begin(), m_defChildEntityList.end(), pEntity );

	if ( it != m_defChildEntityList.end() )
	{
		m_defChildEntityList.erase( it );
	}
	else
	{
		printf( "Can not find the entity. ParamInfo[%s]. [%s]", pEntity ? pEntity->GetClassName() : "NULL", TS_CODE_TRACE() );
	}
}

NTL_TS_TC_ID CNtlTSCont::BeginPrevLink( void )
{
	m_itLink = m_defPrevLink.begin();
	if ( m_itLink == m_defPrevLink.end() ) return 0;
	return *m_itLink;
}

NTL_TS_TC_ID CNtlTSCont::NextPrevLink( void )
{
	++m_itLink;
	if ( m_itLink == m_defPrevLink.end() ) return 0;
	return *m_itLink;
}

bool CNtlTSCont::AttachPrevLink( NTL_TS_TC_ID tcId )
{
	if ( NTL_TS_TC_ID_INVALID == tcId )
	{
		printf( "Can not attach the invalid container. ParamInfo[%d]. [%s]", tcId, TS_CODE_TRACE() );
		return false;
	}

	m_defPrevLink.push_back( tcId );

	return true;
}

void CNtlTSCont::DetachPrevLink( NTL_TS_TC_ID tcId )
{
	if ( NTL_TS_TC_ID_INVALID == tcId )
	{
		printf( "Can not detach the invalid container. ParamInfo[%d]. [%s]", tcId, TS_CODE_TRACE() );
		return;
	}

	listdef_ContList::iterator it = std::find( m_defPrevLink.begin(), m_defPrevLink.end(), tcId );
	if ( it != m_defPrevLink.end() )
	{
		m_defPrevLink.erase( it );
	}
	else
	{
		printf( "Can not find the container. ParamInfo[%d]. [%s]", tcId, TS_CODE_TRACE() );
	}
}

NTL_TSRESULT CNtlTSCont::SearchAll( CNtlTSRecv* pTSRecv, void* pParam )
{
	NTL_TSRESULT rsResult = 0;

	rsResult |= pTSRecv->Search( this, pParam );

	CNtlTSEntity* pEntity;
	vecdef_EntityList::iterator it = m_defChildEntityList.begin();
	for ( ; it != m_defChildEntityList.end(); ++it )
	{
		pEntity = *it;
		rsResult |= pEntity->SearchSelf( pTSRecv, pParam );
	}

	return rsResult;
}

NTL_TSRESULT CNtlTSCont::RunAll( CNtlTSRecv* pTSRecv, void* pParam )
{
	NTL_TSRESULT rsResult = 0;
	CNtlTSEntity* pEntity;

	vecdef_EntityList::iterator it = m_defChildEntityList.begin();
	for ( ; it != m_defChildEntityList.end(); ++it )
	{
		pEntity = *it;
		rsResult |= pEntity->RunSelf( pTSRecv, pParam );
	}

	return rsResult;
}

CNtlTSScrObject* CNtlTSCont::BeginChildForScript( void )
{
	if ( m_defChildEntityList.empty() ) return 0;
	m_itChildEntityList = m_defChildEntityList.begin();
	return *m_itChildEntityList;
}

CNtlTSScrObject* CNtlTSCont::NextChildForScript( void )
{
	++m_itChildEntityList;
	if ( m_itChildEntityList == m_defChildEntityList.end() ) return 0;
	return *m_itChildEntityList;
}

void CNtlTSCont::AttachChildForScript( CNtlTSScrObject* pChild )
{
	if ( 0 == pChild )
	{
		printf( "Can not attach the script object. ParamInfo[%s]. [%s]", "NULL", TS_CODE_TRACE() );
		return;
	}

	if ( pChild->IsDerivedClass( "CNtlTSEntity" ) )
	{
		m_defChildEntityList.push_back( (CNtlTSEntity*)pChild );
	}
	else
	{
		printf( "Can not do type cast from CNtlTSScrObject to CNtlTSEntity. ParamInfo[%s]. [%s]", pChild ? pChild->GetClassName() : "NULL", TS_CODE_TRACE() );
	}
}

void CNtlTSCont::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEntity::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "cid" ) )
	{
		m_tcID = (NTL_TS_TC_ID)clProperty.GetValueAsInt( "cid" );
	}

	if ( clProperty.IsExist( "prelnk" ) )
	{
		std::string strPreLnk = clProperty.GetValue( "prelnk" );
		ParsingLinkInfo( strPreLnk );
	}
}

void CNtlTSCont::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEntity::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetID() );
	clProperty.m_defProperty["cid"] = g_NtlTSString;

	if ( !m_defPrevLink.empty() )
	{
		std::string strPreLnk;
		listdef_ContList::iterator it = m_defPrevLink.begin();
		for ( ; it != m_defPrevLink.end(); ++it )
		{
			sprintf_s( g_NtlTSString, "%d", *it );
			strPreLnk += g_NtlTSString;
			strPreLnk += ";";
		}

		clProperty.m_defProperty["prelnk"] = strPreLnk;
	}
}

void CNtlTSCont::MakeAllObjectList( std::vector<CNtlTSScrObject*>& defObjList )
{
	defObjList.push_back( this );

	vecdef_EntityList::iterator itEntity = m_defChildEntityList.begin();
	for ( ; itEntity != m_defChildEntityList.end(); ++itEntity )
	{
		(*itEntity)->MakeAllObjectList( defObjList );
	}
}

bool CNtlTSCont::ParsingLinkInfo( const std::string& strContents )
{
	std::string strNum;

	std::string::const_iterator cit = strContents.begin();
	for ( ; cit != strContents.end(); ++cit )
	{
		if ( ';' == *cit )
		{
			m_defPrevLink.push_back( (NTL_TS_TC_ID)atoi( strNum.c_str() ) );
			strNum.clear();
		}
		else if ( ' ' == *cit )
		{
			continue;
		}
		else if ( '0' > *cit || '9' < *cit )
		{
			m_defPrevLink.clear();

			printf( "The link info must have to be NUMERIC" );

			return false;
		}
		else
		{
			strNum += *cit;
		}
	}

	if ( !strNum.empty() )
	{
		m_defPrevLink.push_back( (NTL_TS_TC_ID)atoi( strNum.c_str() ) );
	}

	return true;
}
