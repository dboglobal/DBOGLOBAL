#include "precomp_trigger.h"
#include "DboTSCheckProgQuest.h"


/**
	Check progress quest condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckProgQuest, CNtlTSCond )


CDboTSCheckProgQuest::CDboTSCheckProgQuest( void )
: m_tQuestID( NTL_TS_T_ID_INVALID ), m_tNOT(FALSE)
{
}

void CDboTSCheckProgQuest::SetQuestID( NTL_TS_T_ID tQID )
{
	m_tQuestID = tQID;
}

void CDboTSCheckProgQuest::SetNOT( BOOL tNOT )
{
	m_tNOT = tNOT;
}

void CDboTSCheckProgQuest::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "qid" ) )
	{
		SetQuestID( (NTL_TS_T_ID)clProperty.GetValueAsInt( "qid" ) );
	}
	if ( clProperty.IsExist( "not" ) )
	{
		SetNOT( (BOOL)clProperty.GetValueAsInt( "not" ) );
	}
}

void CDboTSCheckProgQuest::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetQuestID() );
	clProperty.m_defProperty["qid"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetNOT() );
	clProperty.m_defProperty["not"] = g_NtlTSString;
}
