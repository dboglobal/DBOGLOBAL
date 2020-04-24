#include "precomp_trigger.h"
#include "DboTSCheckLvl.h"


/**
	Check level condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckLvl, CNtlTSCond )


CDboTSCheckLvl::CDboTSCheckLvl( void )
: m_nMinLvl( 0 ),
  m_nMaxLvl( 0 )
{
}

void CDboTSCheckLvl::SetMinLvl( int nLvl )
{
	m_nMinLvl = nLvl;
}

void CDboTSCheckLvl::SetMaxLvl( int nLvl )
{
	m_nMaxLvl = nLvl;
}

void CDboTSCheckLvl::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "minlvl" ) )
	{
		SetMinLvl( clProperty.GetValueAsInt( "minlvl" ) );
	}

	if ( clProperty.IsExist( "maxlvl" ) )
	{
		SetMaxLvl( clProperty.GetValueAsInt( "maxlvl" ) );
	}
}

void CDboTSCheckLvl::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetMinLvl() );
	clProperty.m_defProperty["minlvl"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetMaxLvl() );
	clProperty.m_defProperty["maxlvl"] = g_NtlTSString;
}
