#include "precomp_trigger.h"
#include "DboTSContStart.h"


/**
	Start container
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSContStart, CDboTSContGCond )


CDboTSContStart::CDboTSContStart( void )
{
	m_uiSTDiag = 0xffffffff;
}

void CDboTSContStart::SetSTDiag( unsigned int uiSTDiag )
{
	m_uiSTDiag = uiSTDiag;
}

void CDboTSContStart::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CDboTSContGCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "stdiag" ) )
	{
		SetSTDiag( clProperty.GetValueAsInt( "stdiag" ) );
	}
}

void CDboTSContStart::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CDboTSContGCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetSTDiag() );
	clProperty.m_defProperty["stdiag"] = g_NtlTSString;
}
