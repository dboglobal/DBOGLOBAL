#include "precomp_trigger.h"
#include "DboTSActObjWPS.h"


/**
	Object WPS
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActObjWPS, CNtlTSAction )


CDboTSActObjWPS::CDboTSActObjWPS( void )
: m_uiScriptID( 0xffffffff )
{
}

void CDboTSActObjWPS::SetObjWPS( unsigned int uiScriptID )
{
	m_uiScriptID = uiScriptID;
}

void CDboTSActObjWPS::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "sid" ) )
	{
		SetObjWPS( atoi( clProperty.GetValue( "sid" ).c_str() ) );
	}
}

void CDboTSActObjWPS::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetObjWPS() );
	clProperty.m_defProperty["sid"] = g_NtlTSString;
}
