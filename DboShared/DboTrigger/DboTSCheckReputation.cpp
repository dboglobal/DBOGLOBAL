#include "precomp_trigger.h"
#include "DboTSCheckReputation.h"


/**
	Check reputation condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckReputation, CNtlTSCond )


CDboTSCheckReputation::CDboTSCheckReputation( void )
: m_uiMinRT( 0 ),
  m_uiMaxRT( 0 )
{
}

void CDboTSCheckReputation::SetMinReputation( unsigned int uiRT )
{
	m_uiMinRT = uiRT;
}

void CDboTSCheckReputation::SetMaxReputation( unsigned int uiRT )
{
	m_uiMaxRT = uiRT;
}

void CDboTSCheckReputation::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "minrt" ) )
	{
		SetMinReputation( clProperty.GetValueAsInt( "minrt" ) );
	}

	if ( clProperty.IsExist( "maxrt" ) )
	{
		SetMaxReputation( clProperty.GetValueAsInt( "maxrt" ) );
	}
}

void CDboTSCheckReputation::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetMinReputation() );
	clProperty.m_defProperty["minrt"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetMaxReputation() );
	clProperty.m_defProperty["maxrt"] = g_NtlTSString;
}
