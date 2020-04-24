#include "precomp_trigger.h"
#include "DboTSActETimerS.h"


/**
	Timer action
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActETimerS, CNtlTSAction )


CDboTSActETimerS::CDboTSActETimerS( void )
: m_eExceptTimerSort( eEXCEPT_TIMER_SORT_LIMIT_TIMER ),
  m_uiTime( 0xffffffff ),
  m_tgId( NTL_TS_TG_ID_INVALID )
{
}

void CDboTSActETimerS::SetTimerSort( eEXCEPT_TIMER_SORT eExceptTimerSort )
{
	m_eExceptTimerSort = eExceptTimerSort;
}

void CDboTSActETimerS::SetTime( unsigned int uiTime )
{
	m_uiTime = uiTime;
}

void CDboTSActETimerS::SetTGId( NTL_TS_TG_ID tgId )
{
	m_tgId = tgId;
}

void CDboTSActETimerS::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "sort" ) )
	{
		SetTimerSort( (eEXCEPT_TIMER_SORT)clProperty.GetValueAsInt( "sort" ) );
	}

	if ( clProperty.IsExist( "time" ) )
	{
		SetTime( clProperty.GetValueAsInt( "time" ) );
	}

	if ( clProperty.IsExist( "tgid" ) )
	{
		SetTGId( (NTL_TS_TG_ID)clProperty.GetValueAsInt( "tgid" ) );
	}
}

void CDboTSActETimerS::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetTimerSort() );
	clProperty.m_defProperty["sort"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetTime() );
	clProperty.m_defProperty["time"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetTGId() );
	clProperty.m_defProperty["tgid"] = g_NtlTSString;
}
