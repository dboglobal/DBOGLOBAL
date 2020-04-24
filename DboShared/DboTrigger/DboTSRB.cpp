#include "precomp_trigger.h"
#include "DboTSRB.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSRB, CNtlTSEvent )


CDboTSRB::CDboTSRB( void )
: m_uiRBTblIdx( 0xffffffff ),
  m_uiRBCondition( eRB_COND_FLAG_INVALID )
{
}

void CDboTSRB::SetRBTblIdx( unsigned int uiRBTblIdx )
{
	m_uiRBTblIdx = uiRBTblIdx;
}

void CDboTSRB::SetRBCondition( unsigned int uiRBCondition )
{
	m_uiRBCondition = uiRBCondition;
}

void CDboTSRB::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "idx" ) )
	{
		SetRBTblIdx( clProperty.GetValueAsInt( "idx" ) );
	}

	if ( clProperty.IsExist( "cond" ) )
	{
		SetRBCondition( clProperty.GetValueAsInt( "cond" ) );
	}
}

void CDboTSRB::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetRBTblIdx() );
	clProperty.m_defProperty["idx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetRBCondition() );
	clProperty.m_defProperty["cond"] = g_NtlTSString;
}
