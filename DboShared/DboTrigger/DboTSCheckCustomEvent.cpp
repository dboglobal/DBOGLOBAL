#include "precomp_trigger.h"
#include "DboTSCheckCustomEvent.h"


/**
	Check custom event condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckCustomEvent, CNtlTSCond )


CDboTSCheckCustomEvent::CDboTSCheckCustomEvent( void )
: m_tQuestID( NTL_TS_T_ID_INVALID )
{
}

void CDboTSCheckCustomEvent::SetQuestID( NTL_TS_T_ID tQuestID )
{
	m_tQuestID = tQuestID;
}

void CDboTSCheckCustomEvent::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "qid" ) )
	{
		SetQuestID((NTL_TS_T_ID)clProperty.GetValueAsInt( "qid" ) );
	}
}

void CDboTSCheckCustomEvent::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetQuestID() );
	clProperty.m_defProperty["qid"] = g_NtlTSString;
}
