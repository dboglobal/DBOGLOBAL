#include "precomp_trigger.h"
#include "NtlTSTrigger.h"
#include "NtlTSCoreStatic.h"
#include "NtlTSGroup.h"
#include "NtlTSRecv.h"


/**
	Trigger entity
*/


NTL_TS_IMPLEMENT_RTTI( CNtlTSTrigger, CNtlTSEntity )


CNtlTSTrigger::CNtlTSTrigger( void )
: m_tID( NTL_TS_T_ID_INVALID ),
  m_bCanRepeat( false ),
  m_bCanShare( true ),
  m_bOutStateMsg( true )
{
}

CNtlTSTrigger::~CNtlTSTrigger( void )
{
}

NTL_TS_T_ID CNtlTSTrigger::SetID( NTL_TS_T_ID tID )
{
	NTL_TS_T_ID tOld = m_tID;
	m_tID = tID;
	return tOld;
}

void CNtlTSTrigger::SetRepeatQuest( bool bCanRepeat )
{
	m_bCanRepeat = bCanRepeat;
}

void CNtlTSTrigger::SetShareQuest( bool bCanShare )
{
	m_bCanShare = bCanShare;
}

void CNtlTSTrigger::SetOutStateMsg( bool bOutStateMsg )
{
	m_bOutStateMsg = bOutStateMsg;
}

CNtlTSGroup* CNtlTSTrigger::GetGroup( NTL_TS_TG_ID tgId )
{
	hashdef_GroupList::iterator it = m_GroupList.find( tgId );
	if ( it == m_GroupList.end() ) return 0;
	return it->second;
}

void CNtlTSTrigger::AttachGroup( CNtlTSGroup* pGroup )
{
	if ( 0 == pGroup )
	{
		printf( "Can not set the group. ParamInfo[%s]. [%s]", "NULL", TS_CODE_TRACE() );
		return;
	}

	if ( GetGroup( pGroup->GetID() ) )
	{
		printf( "Is exists already. ParamInfo[%d]. [%s]", pGroup->GetID(), TS_CODE_TRACE() );
		return;
	}

	m_GroupList[pGroup->GetID()] = pGroup;
}

void CNtlTSTrigger::SetTitle( unsigned int uiTitle )
{
	m_uiTitle = uiTitle;
}

void CNtlTSTrigger::SetQC( int intQC )
{
	m_uiQuestCategory = intQC;
}

NTL_TSRESULT CNtlTSTrigger::SearchAll( CNtlTSRecv* pTSRecv, void* pParam )
{
	NTL_TSRESULT tsResult = 0;

	tsResult |= pTSRecv->Search( this, pParam );

	hashdef_GroupList::iterator it = m_GroupList.begin();
	for ( ; it != m_GroupList.end(); ++it )
	{
		tsResult |= it->second->SearchAll( pTSRecv, pParam );
	}

	return tsResult;
}

NTL_TSRESULT CNtlTSTrigger::SearchTarget( NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CNtlTSRecv* pTSRecv, void* pParam )
{
	NTL_TSRESULT tsResult = 0;

	tsResult |= pTSRecv->Search( this, pParam );

	CNtlTSGroup* pGroup = GetGroup( tgID );
	if ( 0 == pGroup ) return NTL_TSRESULT_TYPE_ERROR;

	tsResult |= pGroup->SearchTarget( tcID, pTSRecv, pParam );

	return tsResult;
}

NTL_TSRESULT CNtlTSTrigger::RunTarget( NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CNtlTSRecv* pTSRecv, void* pParam )
{
	NTL_TSRESULT tsResult = 0;

	CNtlTSGroup* pGroup = GetGroup( tgID );
	if ( 0 == pGroup ) return NTL_TSRESULT_TYPE_ERROR;

	tsResult |= pGroup->RunTarget( tcID, pTSRecv, pParam );

	return tsResult;
}

CNtlTSScrObject* CNtlTSTrigger::BeginChildForScript( void )
{
	m_itGroupList = m_GroupList.begin();
	if ( m_itGroupList == m_GroupList.end() ) return 0;

	return m_itGroupList->second;
}

CNtlTSScrObject* CNtlTSTrigger::NextChildForScript( void )
{
	++m_itGroupList;
	if ( m_itGroupList == m_GroupList.end() ) return 0;

	return m_itGroupList->second;
}

void CNtlTSTrigger::AttachChildForScript( CNtlTSScrObject* pChild )
{
	if ( 0 == pChild )
	{
		printf( "Can not attach the script object. ParamInfo[%s]. [%s]", "NULL", TS_CODE_TRACE() );
		return;
	}

	if ( pChild->IsDerivedClass( "CNtlTSGroup" ) )
	{
		AttachGroup( (CNtlTSGroup*)pChild );
	}
	else
	{
		printf( "Can not do type cast from CNtlTSScrObject to CNtlTSGroup. ParamInfo[%s]. [%s]", pChild->GetClassName(), TS_CODE_TRACE() );
	}
}

void CNtlTSTrigger::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEntity::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "tid" ) )
	{
		SetID( (NTL_TS_T_ID)clProperty.GetValueAsInt( "tid" ) );
	}
	if ( clProperty.IsExist( "rq" ) )
	{
		SetRepeatQuest( clProperty.GetValueAsInt( "rq" ) ? true : false );
	}
	if ( clProperty.IsExist( "sq" ) )
	{
		SetShareQuest( clProperty.GetValueAsInt( "sq" ) ? true : false );
	}
	if ( clProperty.IsExist( "sm" ) )
	{
		SetOutStateMsg( clProperty.GetValueAsInt( "sm" ) ? true : false );
	}
	if ( clProperty.IsExist( "title" ) )
	{
		SetTitle( clProperty.GetValueAsInt( "title" ) );
	}
	if ( clProperty.IsExist( "qc" ) )
	{
		SetQC( clProperty.GetValueAsInt( "qc" ) );
	}
}

void CNtlTSTrigger::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEntity::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetID() );
	clProperty.m_defProperty["tid"] = std::string( g_NtlTSString );

	sprintf_s( g_NtlTSString, "%d", IsRepeatQuest() ? 1 : 0 );
	clProperty.m_defProperty["rq"] = std::string( g_NtlTSString );

	sprintf_s( g_NtlTSString, "%d", IsShareQuest() ? 1 : 0 );
	clProperty.m_defProperty["sq"] = std::string( g_NtlTSString );

	sprintf_s( g_NtlTSString, "%d", IsOutStateMsg() ? 1 : 0 );
	clProperty.m_defProperty["sm"] = std::string( g_NtlTSString );

	sprintf_s( g_NtlTSString, "%d", GetTitle() );
	clProperty.m_defProperty["title"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetQC() );
	clProperty.m_defProperty["qc"] = g_NtlTSString;
}

void CNtlTSTrigger::MakeAllObjectList( std::vector<CNtlTSScrObject*>& defObjList )
{
	defObjList.push_back( this );

	hashdef_GroupList::iterator it = m_GroupList.begin();
	for ( ; it != m_GroupList.end(); ++it )
	{
		it->second->MakeAllObjectList( defObjList );
	}
}

