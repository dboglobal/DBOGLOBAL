#include "precomp_trigger.h"
#include "DboTSActSkipCont.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSActSkipCont, CNtlTSAction )


CDboTSActSkipCont::CDboTSActSkipCont( void )
: m_tSkipQuestID( NTL_TS_T_ID_INVALID ),
  m_tcSkipContID( NTL_TS_TC_ID_INVALID )
{
}

void CDboTSActSkipCont::SetSkipQuestID( NTL_TS_T_ID tQuestID )
{
	m_tSkipQuestID = tQuestID;
}

void CDboTSActSkipCont::SetSkipContID( NTL_TS_TC_ID tcContID )
{
	m_tcSkipContID = tcContID;
}

void CDboTSActSkipCont::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "stid" ) )
	{
		SetSkipQuestID( (NTL_TS_T_ID)clProperty.GetValueAsInt( "stid" ) );
	}

	if ( clProperty.IsExist( "stcid" ) )
	{
		SetSkipContID( (NTL_TS_TC_ID)clProperty.GetValueAsInt( "stcid" ) );
	}
}

void CDboTSActSkipCont::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetSkipQuestID() );
	clProperty.m_defProperty["stid"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetSkipContID() );
	clProperty.m_defProperty["stcid"] = g_NtlTSString;
}
